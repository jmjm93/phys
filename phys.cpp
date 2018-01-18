// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader.hpp"
//#include "common/texture.hpp"
#include "controls.hpp"

glm::vec3 rayDown;


double dotProduct(glm::vec3 a, glm::vec3 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

double modulus(glm::vec3 a)
{
	return sqrt(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2));
}

// calculates the distance towards a collision between the user and a plane, planenormal and planeposition being the normal vector and position of the plane, position and direction being the position and direction vectors of the mobile user
// the return value will be 0 if there won't be any collision ever, 1 if otherwise, and the distance will be stored on lambda
int calculateIntersectionPlane(glm::vec3 planenormal, glm::vec3 planeposition, glm::vec3 position, glm::vec3 direction, double& lambda, double& alpha)
{
	double DotProduct = dotProduct(direction, planenormal);
	double l2;

	
	if ((DotProduct < 1.0e-8) && (DotProduct > -1.0e-8)) return 0;
	l2 = dotProduct(planenormal, planeposition - position) / DotProduct;
	if (l2 < -1.0e-8) return 0;
	glm::vec3 length = position - planeposition;
	//double lenmod = sqrt(pow(length.x, 2) + pow(length.y, 2) + pow(length.z, 2));
	lambda = l2;
	alpha = -1;
	// TODO: ray projection to calculate the exact coordinates of the collision
/*	//lambda = modulus(position-planeposition);
	if (l2 < 10) {
		glm::vec3 rayProjection = position;
		double lastDistance = modulus(planeposition - rayProjection);
		while (modulus(planeposition - rayProjection) <= lastDistance)
		{
			lastDistance = modulus(planeposition - rayProjection);
			rayProjection += direction;
		}
		l2 = lastDistance;
		alpha = l2;
	} */
	return 1;
}


int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Physics Engine test", NULL, NULL);
	setWindow(window);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
    glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

	// background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "TransformVertexShader.vertexshader");

	// Get a handle for our "MVP" matrix
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

		

	float xOffset = 0.0f;
	float zOffset = 0.0f;

	// size of the world
	const int height = 100;
	const int width =100;  // BORDERS: [0,0] [0,100] [100,0] [100,100]
	// array of ground vertices
	GLfloat ans[height*width*3*6] = {0.0f};
	int i = 0;
	// generates the world
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			ans[i] = xOffset + col; ans[i + 1] = -5.0f; ans[i + 2] = zOffset + row;
			i += 3;
			ans[i] = xOffset + col + 1; ans[i + 1] = -5.0f; ans[i + 2] = zOffset + row;
			i += 3;
			ans[i] = xOffset + col + 1; ans[i + 1] = -5.0f; ans[i + 2] = zOffset + row + 1;
			i += 3; 
			ans[i] = xOffset + col; ans[i + 1] = -5.0f; ans[i + 2] = zOffset + row + 1;
			i += 3;

		}
	}

	// the coordinates for the position and normal of the ground plane, to be used for collision calculation
	glm::vec3 planeposition = glm::vec3(50.0f, -5.0f, 50.0f);
	glm::vec3 planenormal = glm::vec3(0.0f, 1.0f, 0.0f);
	// initialization for the downward ray
	rayDown = glm::vec3(0.0f, -1.0f, 0.0f);


	GLuint vertexbuffer;
	glGenBuffersARB(1, &vertexbuffer);
	glBindBufferARB(GL_ARRAY_BUFFER, vertexbuffer);


	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(ans), ans, GL_DYNAMIC_DRAW);
	
	// initial pos
	glm::vec3 pos = glm::vec3(0.0f, 0.7f, 0.0f);
	int fps = 0;
	bool needRefresh = false;
	// eternal loop as long as the user doesn't press escape
	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		pos = getCurrentPosition();
		glm::vec3 dir = getCurrentDirection();
		fps++;
		glm::vec3 speed = getCurrentSpeed();
		

		stop_collision_down;
		GLfloat anscol;
		double downalpha;
		double len;
		anscol = calculateIntersectionPlane(planenormal, planeposition, pos, rayDown, len, downalpha);
		if (len < 0.7 && len > 0) {
			set_collision_down(1, planenormal);
		}
		if (fps % 600 == 0) {
			fprintf(stderr, "POSITION %f,%f,%f\n ", pos.x, pos.y, pos.z);
			fprintf(stderr, "SPEED %f,%f,%f\n ", speed.x, speed.y, speed.z);
			fprintf(stderr, "%f, %d\n", len, anscol);
		}

		if (pos.x - xOffset > 60) {
			xOffset += 10;
			needRefresh = true;
		}
		else if (pos.x - xOffset < 40) {
				xOffset -= 10;
				needRefresh = true;
			}
		if (pos.z - zOffset > 60) {
			zOffset += 10;
			needRefresh = true;
		}
		else if (pos.z - zOffset < 40) {
			zOffset -= 10;
			needRefresh = true;
		}
		int i = 0;
		if (needRefresh) {
			for (int row = 0; row < height; row++)
			{
				for (int col = 0; col < width; col++)
				{
					ans[i] = xOffset + col; ans[i + 1] = -5.0f; ans[i + 2] = zOffset + row;
					i += 3;
					ans[i] = xOffset + col + 1; ans[i + 1] = -5.0f; ans[i + 2] = zOffset + row;
					i += 3; 
					ans[i] = xOffset + col; ans[i + 1] = -5.0f; ans[i + 2] = zOffset + row;
					i += 3;
					ans[i] = xOffset + col; ans[i + 1] = -5.0f; ans[i + 2] = zOffset + row + 1;
					i += 3;
				}
			}
			needRefresh = false;
		}
		glBufferDataARB(GL_ARRAY_BUFFER, sizeof(ans), ans, GL_DYNAMIC_DRAW);
		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		); 

	

		glDrawArrays(GL_LINES, 0, height*width*6*3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffersARB(1, &vertexbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}


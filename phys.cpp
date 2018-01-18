// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <list>

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
#include "collision.hpp"

#include "world.hpp"

std::list<asset> world;

// lists shouldn't be used for performance purposes



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
	GLfloat ans[height*width*3*4] ={0.0f};
	int i = 0;
	// generates the world
	asset ground = generateGround(width,height,xOffset,zOffset,ans);
	world.push_front(ground);


	GLuint vertexbuffer;
	glGenBuffersARB(1, &vertexbuffer);
	glBindBufferARB(GL_ARRAY_BUFFER, vertexbuffer);


	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(ans), ans, GL_DYNAMIC_DRAW);
	
	// initial pos
	glm::vec3 pos = glm::vec3(0.0f, 5.0f, 0.0f);
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
	

		runProjections(pos,dir,world);	
		if (fps % 150 == 0) {
		//	fprintf(stderr, "POSITION %f,%f,%f\n ", pos.x, pos.y, pos.z);
		//	fprintf(stderr, "SPEED %f,%f,%f\n ", speed.x, speed.y, speed.z);
		//	fprintf(stderr, "DIRECTION %f,%f,%f\n ", dir.x, dir.y, dir.z);
		}
		updateGround(pos,ans);
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


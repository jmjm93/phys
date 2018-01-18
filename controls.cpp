// Include GLFW
#include <GLFW/glfw3.h>
#include <stdio.h>
GLFWwindow* _window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

bool jumping = false;

#include "controls.hpp"

float gravity = 0.98f / 2.0f;

glm::vec3 movSpeed = glm::vec3(0.0f,0.0f,0.0f);//glm::vec3(6.0f,6.0f,6.0f);
glm::vec3 accelSpeed = glm::vec3(10.0f, 5.0f, 10.0f);

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}

glm::vec3 getCurrentSpeed()
{
	return movSpeed;
}

short collision_forward_detected = 0;
short collision_right_detected = 0;
short collision_left_detected = 0;
short collision_back_detected = 0;
short collision_down_detected = 0;
glm::vec3 collision_normal;

// Initial position : on +Z
glm::vec3 position = glm::vec3( 25.6361f, -4.78f, 24.7702f ); 
glm::vec3 direction = glm::vec3(-0.212081, -0.0f, 0.452621f);//glm::vec3(0.376466f,-0.0f,-0.924798f);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float mouseSpeed = 0.005f;

void set_collision(int id, glm::vec3 norm)
{
	collision_normal = norm;
	collision_forward_detected = id;
}

void stop_collision()
{
	collision_forward_detected = 0;
}

void set_collision_right(int coll, glm::vec3 norm)
{
	collision_normal = norm;
	collision_right_detected = coll;
}
void stop_collision_right()
{
	collision_right_detected = 0;
}

void set_collision_left(int coll, glm::vec3 norm)
{
	collision_normal = norm;
	collision_left_detected = coll;
}

void stop_collision_left()
{
	collision_left_detected = 0;
}

void set_collision_back(int coll, glm::vec3 norm)
{
	collision_normal = norm;
	collision_back_detected = coll;
}

void stop_collision_back()
{
	collision_back_detected = 0;
}

void set_collision_down(int coll, glm::vec3 norm)
{
	collision_normal = norm;
	collision_down_detected = coll;
}

void stop_collision_down()
{
	collision_down_detected = 0;
}

glm::vec3 getCurrentPosition() {
	return position;
}

glm::vec3 getCurrentDirection() {
	return direction;
}


void setWindow(GLFWwindow* win)
{
	_window = win;
}



void computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(_window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(_window, 1024/2, 768/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	verticalAngle   += mouseSpeed * float( 768/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle)*0.5f, 
		sin(verticalAngle)*0.5f,
		cos(verticalAngle) * cos(horizontalAngle)*0.5f
	);
	 
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	right /= 2.0f;
	// Up vector
	glm::vec3 up = glm::cross( right, direction );
	// Move forward
	if (glfwGetKey( _window, GLFW_KEY_W ) == GLFW_PRESS){
		
		/*position.x += direction.x * deltaTime * movSpeed.x;
		position.y += direction.y * deltaTime * movSpeed.y;
		position.z += direction.z * deltaTime * movSpeed.z;
		if (collision_forward_detected) {
			position -= collision_normal*direction * deltaTime * movSpeed;
		}
		if (collision_down_detected) {
			position -= collision_normal*direction * deltaTime * movSpeed;
			} */
		movSpeed.x += direction.x * deltaTime * accelSpeed.x;
		movSpeed.z += direction.z * deltaTime * accelSpeed.z;
		if (collision_forward_detected) {
			movSpeed.x -= collision_normal.x*direction.x * deltaTime * accelSpeed.x;
			movSpeed.z -= collision_normal.z*direction.z * deltaTime * accelSpeed.z;
		}
	/*	if (collision_down_detected) {
			movSpeed -= collision_normal*direction * deltaTime * accelSpeed;
			}*/
		}
	// Move backward
	if (glfwGetKey( _window, GLFW_KEY_S ) == GLFW_PRESS){
	/*	position.x -= direction.x * deltaTime * movSpeed.x;
		position.y -= direction.y * deltaTime * movSpeed.y;
		position.z -= direction.z * deltaTime * movSpeed.z;
		if (collision_back_detected) {
			position += collision_normal*direction * deltaTime * movSpeed;
		}
		if (collision_down_detected) {
			position += collision_normal*direction * deltaTime * movSpeed;
		}*/
		movSpeed.x -= direction.x * deltaTime * accelSpeed.x;
		movSpeed.z -= direction.z * deltaTime * accelSpeed.z;
		if (collision_back_detected) {
			movSpeed.x += collision_normal.x*direction.x * deltaTime * accelSpeed.x;
			movSpeed.z += collision_normal.z*direction.z * deltaTime * accelSpeed.z;
		}
		/*if (collision_down_detected) {
			movSpeed += collision_normal*direction * deltaTime * accelSpeed;
		}*/
	}
	// Strafe right
	if (glfwGetKey( _window, GLFW_KEY_D ) == GLFW_PRESS){
	/*	position.x += right.x * deltaTime * movSpeed.x;
		position.y += right.y * deltaTime * movSpeed.y;
		position.z += right.z * deltaTime * movSpeed.z;
		if (collision_right_detected) {
			position -= collision_normal*right * deltaTime * movSpeed;
		}*/
		movSpeed.x += right.x * deltaTime * accelSpeed.x;
		movSpeed.z += right.z * deltaTime * accelSpeed.z;
		if (collision_right_detected) {
			movSpeed -= collision_normal*right * deltaTime * accelSpeed;
		}
	}
	// Strafe left
	if (glfwGetKey( _window, GLFW_KEY_A ) == GLFW_PRESS){
/*		position.x -= right.x * deltaTime * movSpeed.x;
		position.y -= right.y * deltaTime * movSpeed.y;
		position.z -= right.z * deltaTime * movSpeed.z;
		if (collision_left_detected) {
			position += collision_normal*right * deltaTime * movSpeed;
		} */
		movSpeed.x -= right.x * deltaTime * accelSpeed.x;
		movSpeed.z -= right.z * deltaTime * accelSpeed.z;
		if (collision_right_detected) {
			movSpeed += collision_normal*right * deltaTime * accelSpeed;
		}
	}
	if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS && !jumping && collision_down_detected) {
	/*	position.y += deltaTime * movSpeed.y * 5; */
		movSpeed.y += deltaTime * accelSpeed.y;
		jumping = true;
	}
	if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_RELEASE) jumping = false;
	if (!collision_down_detected) {
		movSpeed.y -= gravity * 0.5f * deltaTime;
	}
	else if (movSpeed.y < 0) movSpeed.y = 0;
	collision_forward_detected = false;
	collision_back_detected = false;
	collision_right_detected = false;
	collision_left_detected = false;
	collision_down_detected = false;
	position += movSpeed;
	if (movSpeed.x != 0.0f) movSpeed.x /= 10;
	if (movSpeed.z != 0.0f) movSpeed.z /= 10;
	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

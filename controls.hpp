#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void set_collision(int coll, glm::vec3 norm);
void stop_collision();
void set_collision_right(int coll, glm::vec3 norm);
void stop_collision_right();
void set_collision_left(int coll, glm::vec3 norm);
void stop_collision_left();
void set_collision_back(int coll, glm::vec3 norm);
void stop_collision_back();
void set_collision_down(int coll, glm::vec3 norm);
void stop_collision_down();
void computeMatricesFromInputs();
void setWindow(GLFWwindow* win);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::vec3 getCurrentPosition();
glm::vec3 getCurrentDirection();
glm::vec3 getCurrentSpeed();
#endif

#ifndef WORLD_HPP
#define WORLD_HPP
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <list>
struct asset {
glm::vec3 normal;
glm::vec3 position;
};


asset generateGround(int width, int height,float xOffset, float zOffset, GLfloat vertices[]);

void updateGround(glm::vec3 pos, GLfloat vertices[]);
#endif /* WORLD_HPP */

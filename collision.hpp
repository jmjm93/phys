#ifndef COLLISION_HPP
#define COLLISION_HPP
#include "world.hpp"

double dotProduct(glm::vec3 a, glm::vec3 b);
double modulus(glm::vec3 a);
int calculateIntersectionPlane(glm::vec3 planenormal, glm::vec3 planeposition, glm::vec3 position, glm::vec3 direction, double& lambda, double& alpha);

void runProjections(glm::vec3 pos, glm::vec3 dir, std::list<asset> world);

#endif /* COLLISION_HPP */

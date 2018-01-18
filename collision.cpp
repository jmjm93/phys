#include <glm/glm.hpp>
#include <list>
#include "world.hpp"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "controls.hpp"


using namespace glm;

glm::vec3 _front,_back,_left,_right;
glm::vec3 _down = glm::vec3(0.0f,-1.0f,0.0f);
glm::vec3 _up = glm::vec3(0.0f,1.0f,0.0f);


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

/*      //lambda = modulus(position-planeposition);
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





void runProjections(glm::vec3 pos, glm::vec3 dir, std::list<asset> world)
{
	glm::vec3 _front = dir;
	glm::vec3 _back = dir*glm::vec3(-1.0f,1.0f,-1.0f);
	glm::vec3 _left = glm::cross(dir,glm::vec3(0.0f,-1.0f,0.0f));
	glm::vec3 _right = glm::cross(dir,glm::vec3(0.0f,1.0f,0.0f));
	stop_collision_down();
	for(std::list<asset>::iterator it = world.begin();
	it != world.end();
	it++)
	{
		double lambda, alpha;
		calculateIntersectionPlane((*it).normal,(*it).position, pos, _down, lambda, alpha);	
		if(lambda < 0.7 && lambda > 0){
			 set_collision_down(1, (*it).normal);
		}
		calculateIntersectionPlane((*it).normal,(*it).position, pos, _front, lambda, alpha);	
		if(lambda < 0.7 && lambda > 0){
			 set_collision(1, (*it).normal);
		}
		calculateIntersectionPlane((*it).normal,(*it).position, pos, _back, lambda, alpha);	
		if(lambda < 0.7 && lambda > 0){
			 set_collision_back(1, (*it).normal);
		}
		calculateIntersectionPlane((*it).normal,(*it).position, pos, _left, lambda, alpha);	
		if(lambda < 0.7 && lambda > 0){
			 set_collision_left(1, (*it).normal);
		}
		calculateIntersectionPlane((*it).normal,(*it).position, pos, _right, lambda, alpha);	
		if(lambda < 0.7 && lambda > 0){
			 set_collision_right(1, (*it).normal);
		}
		
	}
	

}





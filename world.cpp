#include <list>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stdio.h>
struct asset {
glm::vec3 normal;
glm::vec3 position;
};

int xOffset = 0;
int zOffset = 0;

int _w;
int _h;

asset generateGround(int width, int height, float xOffset, float zOffset, GLfloat vertices[])
{
	_w = width;
	_h = height;
        int i = 0;
        // generates the world
        for (int row = 0; row < height; row++)
        {
                for (int col = 0; col < width; col++)
                {
                        vertices[i] = xOffset + col; vertices[i + 1] = -5.0f; vertices[i + 2] = zOffset + row;
			i += 3;
                        vertices[i] = xOffset + col + 1; vertices[i + 1] = -5.0f; vertices[i + 2] = zOffset + row;
                        i += 3;
                        vertices[i] = xOffset + col; vertices[i + 1] = -5.0f; vertices[i + 2] = zOffset + row;
                        i += 3;
                        vertices[i] = xOffset + col; vertices[i + 1] = -5.0f; vertices[i + 2] = zOffset + row + 1;
                        i += 3;

                }
        }
        asset ground;
        ground.position = glm::vec3(50.0f+xOffset,-5.0f,50.0f+zOffset);
        ground.normal = glm::vec3(0.0f,1.0f,0.0f);
	return ground;
}

void updateGround(glm::vec3 pos, GLfloat vertices[])
{
	bool needRefresh = false;
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
                        generateGround(_w,_h,xOffset,zOffset,vertices);
                        needRefresh = false;
                }
}

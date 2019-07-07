#include <stdio.h>
#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define NumDiscPoint 16
#define NumDiscTriangle 16
#define NumDiscIndice 3*NumDiscTriangle


using namespace glm;
extern unsigned int cone_vao;
const double PI = 3.1415926;


void createCone();
void renderCone();



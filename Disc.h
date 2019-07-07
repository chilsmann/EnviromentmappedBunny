#include <stdio.h>
#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define NumDiscPoints 16
#define NumDiscTriangles 16
#define NumDiscIndices 3*NumDiscTriangles


using namespace glm;
extern unsigned int disc_vao;
const double kPI = 3.1415926;


void createDisc();
void renderDisc();



#include <GL/glew.h>
#include <GL/freeglut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <iostream>

#include <math.h>
#include <vector>
#include <map>
#include <set>

#define GLM_FORCE_RADIANS 
#define TWOPI 2*3.1415926535897932384626433832795

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


#include "shader.h"
#include "skybox.h"
#include "objloader.h"
#include "Cube.h"
#include "Cone.h"
#include "Disc.h"
#include "Hexagon.h"
#include "Pyramid.h"

using namespace glm;
using namespace std;
int setObj = 1;
mat4 view;
mat4 model;
mat4 projection;
mat4 model_view;

//vector<vec3> vertices;
//vector<vec3> normals;
vector<vec2> textures;
//vector<int> indices;
int numverts;

vector<vec3> tempVertices;
vector<vec3> tempNormals;
float fade = 1.0f;

GLuint program;
float aspect = 0.0;
GLfloat angle = 0.0;
GLuint vao;
unsigned int vbo[3];
GLuint ebo;
mat4 translateMatrix;
std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals; // Won't be used at the moment.
std::vector<glm::vec2> uvs;
vector<int> indices;
GLuint bunnyTexID;
SkyBox* sky;
static const double mPI = 3.1415926535897932384626433832795;
static int gViewportWidth, gViewportHeight;
static double gCameraScale = 1.0;
static double gCameraTranslationX = 0;
static double gCameraTranslationY = 0;
static bool gIsRotatingCamera = false;
static bool gIsScalingCamera = false;
static bool gIsTranslatingCamera = false;
static int gLastMouseX, gLastMouseY;
mat4 gCameraRotation;
GLfloat timeUpdate = 0.0;
vec3 cameraPos;
bool reflective = false;
float reflectFactor = 1.0f;
Cube* cube;

void Initialize();
void Display(void);
void makeIdentity();
void Reshape(int width, int height);
void glutMouse(int button, int state, int x, int y);
void glutMotion(int x, int y);
double projectToTrackball(double radius, double x, double y);
void Delay(int n);
void updateTime();
void setMatrices();
void varySkybox();
unsigned int loadCubeMapTexture(std::string posX, std::string negX, std::string posY, std::string negY, std::string posZ, std::string negZ);

/********************************************************************************/
unsigned int loadCubeMapTexture(std::string posX, std::string negX, std::string posY, std::string negY, std::string posZ, std::string negZ) {

	ILboolean success;
	unsigned int imageID;
	GLuint textureID;
	GLuint faceTarget[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};
	std::string files[6];

	files[0] = posX;
	files[1] = negX;
	files[2] = posY;
	files[3] = negY;
	files[4] = posZ;
	files[5] = negZ;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &textureID); /* Texture name generation */
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	// Load Textures for Cube Map

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	ilGenImages(1, &imageID);
	ilBindImage(imageID); /* Binding of DevIL image name */

	for (int i = 0; i < 6; ++i) {
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
		success = ilLoadImage((ILstring)files[i].c_str());

		if (!success) {
			std::cout << "Couldn't load texture file." << std::endl;
			// The operation was not sucessfull hence free image and texture 
			ilDeleteImages(1, &imageID);
		}

		/* Convert image to RGBA */
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		/* Create and load textures to OpenGL */
		glTexImage2D(faceTarget[i], 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

		std::cout << "Texture Loaded: " << files[i].c_str() << std::endl;
	}
	std::cout << "Cube Map Loaded Successfully." << std::endl;


	//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	/* Because we have already copied image data into texture data we can release memory used by image. */
	ilDeleteImages(1, &imageID);

	// add information to the log

	return textureID;
}


/****************************************************************************/
void Initialize(void){
	// Create the program for rendering the model

	objloader loader;

	bool loadfile = loader.load("bunnyTex.obj");
	vertices = loader.getVertices();
	normals = loader.getNormals();
	indices = loader.getVertexIndices();
	textures = loader.getTexCoordinates();
	numverts = normals.size();

	tempVertices = loader.getVertices();
	tempNormals = loader.getNormals();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint offset = 0;
	glGenBuffers(1, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3) + normals.size() * sizeof(glm::vec3) + textures.size() * sizeof(glm::vec2), NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, offset, vertices.size() * sizeof(glm::vec3), &vertices[0]);//for vertices
	offset += vertices.size() * sizeof(glm::vec3);
	glBufferSubData(GL_ARRAY_BUFFER, offset, normals.size() * sizeof(glm::vec3), &normals[0]);//for normals
	offset += normals.size() * sizeof(glm::vec3);
	glBufferSubData(GL_ARRAY_BUFFER, offset, textures.size() * sizeof(glm::vec2), &textures[0]);//for textures

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(vertices.size() * sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);  // Vertex normal

	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(vertices.size() * sizeof(glm::vec3) + normals.size() * sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);  // Vertex texture

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);


	cube = new Cube();//load bunny obj
	sky = new SkyBox();

	createPyramid();
	createDisc();
	createHexagon();
	createCone();
	
	// Create and compile our GLSL program from the shaders
	program = LoadShaders("cubemap_reflect.vs", "cubemap_reflect.fs");
	
	// Use our shader
	glUseProgram(program);
	loadCubeMapTexture("images/pos_x.tga", "images/neg_x.tga", "images/pos_y.tga", "images/neg_y.tga", "images/pos_z.tga", "images/neg_z.tga");
	glUniform1i(glGetUniformLocation(program, "CubeMapTex"), 0);

	cameraPos = vec3(3.0f * cos(angle), 2.0f, 3.0f * sin(angle));
	
	glClearColor(1.0, 1.0, 1.0, 1.0);

}
/************************************************************************************************/
void Display(void){
	
	// Clear

	updateTime();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
    glUseProgram(program);
	view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = glm::perspective(radians(70.0f), aspect, 0.3f, 100.0f);
	glUniform3fv(glGetUniformLocation(program, "WorldCameraPosition"), 1, (GLfloat*)&cameraPos[0]);
	model = mat4(1.0f);
	glUniform1i(glGetUniformLocation(program, "DrawSkyBox"), true);
	setMatrices();
	sky->render();
	
	glUniform1i(glGetUniformLocation(program, "DrawSkyBox"), reflective);
	vec4 material_color(1.0f, 0.5f, 0.5f, 1.0f);
	glUniform4fv(glGetUniformLocation(program, "MaterialColor"), 1, (GLfloat*)&material_color[0]);
	glUniform1f(glGetUniformLocation(program, "ReflectFactor"), reflectFactor);

	model = mat4(1.0f);

	mat4 scaled1 = glm::scale(glm::mat4(1.0f), glm::vec3(gCameraScale, gCameraScale, gCameraScale));
	mat4 translated1 = glm::translate(glm::mat4(1.0f), glm::vec3(gCameraTranslationX, gCameraTranslationY, 0.0));
	mat4 transformation_matrix = translated1*gCameraRotation*scaled1;
	model *= transformation_matrix;

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, (GLfloat*)&model[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (GLfloat*)&view[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, (GLfloat*)&projection[0]);

	
	setMatrices();
	//cube->render();//render bunny?
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	

	if (setObj == 1)
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
	else if (setObj == 2)
		cube->render();
	else if (setObj == 3)
		renderCone();
	else if (setObj == 4)
		renderDisc();
	else if (setObj == 5)
		renderHexagon();
	else if (setObj == 6)
		renderPyramid();
	

	glBindVertexArray(0);

	glutSwapBuffers();
}

/*******************************************************************************/
void setMatrices()
{
	mat4 mv = view * model;
	mat4 mvp = projection*mv;
	glUniformMatrix4fv(glGetUniformLocation(program, "ModelMatrix"), 1, GL_FALSE, (GLfloat*)&mv[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
}

/*****************************************************************************/

void Reshape(int width, int height){
	
	gViewportWidth = width;
	gViewportHeight = height;
	glViewport(0, 0, width, height);
	
	aspect = float(width) / float(height);
}
/**************************************************************************************/
void Delay(int n){  
	if (reflectFactor >= 1.0f)
		fade = -1.0f;
	if (reflectFactor <= 0.0f)
		fade = 1.0f;
		
		reflectFactor += 0.1f*fade;
		glutPostRedisplay();
		glutTimerFunc(100, Delay, n);
}

/****************************************************************************************/
void updateTime(){
		angle += 0.1f;

}

/**************************************************************************************/
void keyboard(unsigned char key, int x, int y){
	switch (key){
	case 'q':case 'Q':
		exit(EXIT_SUCCESS);
		break;

	case 's':
	case 'S':
		varySkybox();
		break;

	case '1':
		setObj = 1;
		break;
	case '2':
		setObj = 2;
		break;
	case '3':
		setObj = 3;
		break;
	case '4':
		setObj = 4;
		break;
	case '5':
		setObj = 5;
		break;
	case '6':
			setObj = 6;
			break;
	
	case 'r':
	case 'R':
		makeIdentity();
		break;
	}

	glutPostRedisplay();
}
void varySkybox() {
	reflective = !reflective;
}

/*********************************************************************************************/
void makeIdentity(){

	gCameraScale = 1.0;
	gCameraTranslationX = 0;
	gCameraTranslationY = 0;
	gCameraRotation = mat4(1.0f);


}
/************************************************************************************************/

double projectToTrackball(double radius, double x, double y)
{
	static const double Radius = sqrt(2.0);
	double z;

	double dist = sqrt(x * x + y * y);
	if (dist < radius * Radius / 2.0)
	{
		// Solve for sphere case.
		z = sqrt(radius * radius - dist * dist);
	}
	else
	{
		// Solve for hyperbolic sheet case.
		double t = radius / Radius;
		z = t * t / dist;
	}

	return z;
}

/*******************************************************************************/
/******************************************************************************
GLUT callback for responding to mouse button presses. Detecst whether to
initiate a point snapping, view rotation or view scale.
******************************************************************************/
void glutMouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			gIsRotatingCamera = true;
		}
		else if (button == GLUT_MIDDLE_BUTTON)
		{
			gIsTranslatingCamera = true;
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			
			gIsScalingCamera = true;
		}

		gLastMouseX = x;
		gLastMouseY = y;
	}
	else
	{
		gIsRotatingCamera = false;
		gIsScalingCamera = false;
		gIsTranslatingCamera = false;
	}
}

/*******************************************************************************/

void glutMotion(int x, int y)
{
	if (gIsRotatingCamera)
	{
		static const double kTrackBallRadius = 0.8;
		vec3 lastPos;
		vec3 currPos;

		lastPos.x = gLastMouseX * 2.0 / gViewportWidth - 1.0;
		lastPos.y = (gViewportHeight - gLastMouseY) * 2.0 / gViewportHeight - 1.0;
		lastPos.z = projectToTrackball(kTrackBallRadius, lastPos[0], lastPos[1]);

		currPos.x = x * 2.0 / gViewportWidth - 1.0;
		currPos.y = (gViewportHeight - y) * 2.0 / gViewportHeight - 1.0;
		currPos.z = projectToTrackball(kTrackBallRadius, currPos[0], currPos[1]);
		
		currPos = normalize(currPos);
		lastPos = normalize(lastPos);

		vec3 rotateVecCross;

		rotateVecCross = cross(vec3(lastPos), vec3(currPos)) ;
		
		double sin_rotate_angle = sqrt(rotateVecCross.x * rotateVecCross.x + rotateVecCross.y * rotateVecCross.y + rotateVecCross.z * rotateVecCross.z);
		double angle = asin(sqrt(rotateVecCross.x * rotateVecCross.x + rotateVecCross.y * rotateVecCross.y + rotateVecCross.z * rotateVecCross.z));
		double cos_rotate_angle = cos(angle);
		
		rotateVecCross = normalize(rotateVecCross);

		mat4 deltaRotation(1.0f);

		deltaRotation[0][0] = cos_rotate_angle + rotateVecCross[0] * rotateVecCross[0] * (1 - cos_rotate_angle);
		deltaRotation[0][1] = rotateVecCross[0] * rotateVecCross[1] * (1 - cos_rotate_angle) + rotateVecCross[2] * sin_rotate_angle;
		deltaRotation[0][2] = rotateVecCross[0] * rotateVecCross[2] * (1 - cos_rotate_angle) - rotateVecCross[1] * sin_rotate_angle;
		deltaRotation[0][3] = 0.0;
		deltaRotation[1][0] = rotateVecCross[0] * rotateVecCross[1] * (1 - cos_rotate_angle) - rotateVecCross[2] * sin_rotate_angle;
		deltaRotation[1][1] = cos_rotate_angle + rotateVecCross[1] * rotateVecCross[1] * (1 - cos_rotate_angle);
		deltaRotation[1][2] = rotateVecCross[2] * rotateVecCross[1] * (1 - cos_rotate_angle) + rotateVecCross[0] * sin_rotate_angle;
		deltaRotation[1][3] = 0.0;
		deltaRotation[2][0] = rotateVecCross[0] * rotateVecCross[2] * (1 - cos_rotate_angle) + rotateVecCross[1] * sin_rotate_angle;
		deltaRotation[2][1] = rotateVecCross[1] * rotateVecCross[2] * (1 - cos_rotate_angle) - rotateVecCross[0] * sin_rotate_angle;
		deltaRotation[2][2] = cos_rotate_angle + rotateVecCross[2] * rotateVecCross[2] * (1 - cos_rotate_angle);
		

		mat4 tempMatrix;

		
		if (abs(angle) > DBL_EPSILON*20.0){
			tempMatrix = gCameraRotation*deltaRotation;
			gCameraRotation = tempMatrix;
		}
	}
	if (gIsTranslatingCamera)
	{
		gCameraTranslationX += 2 * double(x - gLastMouseX) / gViewportWidth;
		gCameraTranslationY -= 2 * double(y - gLastMouseY) / gViewportHeight;

	}
	else if (gIsScalingCamera)
	{
		float y1 = gViewportHeight - gLastMouseY;
		float y2 = gViewportHeight - y;

		gCameraScale *= 1 + (y1 - y2) / gViewportHeight;
	}

	glutPostRedisplay();

	gLastMouseX = x;
	gLastMouseY = y;
}
/************************************************************************************************************************************************************/

/*******************************************************************************
*******************************************************************************/

/*********/
int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);

	glutCreateWindow("Environment mapping");

	if (glewInit()){
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
	}

	ilInit();
	Initialize();
	std::cout << glGetString(GL_VERSION) << std::endl;
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(glutMotion);
	glutTimerFunc(100, Delay, 0);
	glutMouseFunc(glutMouse);
	glutReshapeFunc(Reshape);
	glutMainLoop();
	return 0;
}



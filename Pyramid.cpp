#include "Pyramid.h"

unsigned int pry_vao;


void createPyramid()
{
	// position of the vertices in the rectangle

	GLfloat vertices[] = {
		
			//front

			0.0f, 1.0f, 0.0f, 1.0f,	//0
			-0.75f, -0.75f, 0.75f, 1.0f,   //1
			0.75f, -0.75f, 0.75f, 1.0f, //2

			//eft

			0.0f, 1.0f, 0.0f, 1.0f,   //3
			-0.75f, -0.75f, -0.75f, 1.0f,    //4
			-0.75f, -0.75f, 0.75f, 1.0f, //5 

			//right

			0.0f, 1.0f, 0.0f, 1.0f,   //6
			0.75f, -0.75f, 0.75f, 1.0f,  //7
			0.75f, -0.75f, -0.75f, 1.0f,  //8

			//back face

			0.0f, 1.0f, 0.0f, 1.0f,  //9
			0.75f, -0.75f, -0.75f, 1.0f,   //10
			-0.75f, -0.75f, -0.75f, 1.0f,   //11

			//bottom

			-0.75f, -0.75f, -0.75f, 1.0f,  //12
			0.75f, -0.75f, -0.75f, 1.0f,   //13
			0.75f, -0.75f, 0.75f, 1.0f,  //14
			-0.75f, -0.75f, 0.75f, 1.0f  //15
			

		

	};

	float normals[] = {
		//front
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		//left
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		//right
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		//backface
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		//bottom
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,


	};

	// Indices for the hexagon strips
	const GLushort indices[] =
	{


		0,1,2,
		3,4,5,
		6,7,8,
		9,10,11,
		12,13,14,
		12,14,15


	};

	float textCoordinates[16 * 2] = {
		//front

			0.5f, 1.0f,	//0
			0.0f, 0.0f, //1
			1.0f, 0.0f, //2

			//left

			0.5f, 1.0f,
			1.0f, 0.0f,
			0.0f,0.0f,

			//right
			0.5f, 1.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,

			//back face
			0.5f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
		
		//bottom
		0.0f,0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f


		
	};


	glGenVertexArrays(1, &pry_vao);
	glBindVertexArray(pry_vao);

	unsigned int handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);// vertex normal

								 //texture
	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, 16 * 2 * sizeof(float), textCoordinates, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices), indices, GL_STATIC_DRAW);

	

	glBindVertexArray(0);



}

void renderPyramid() {
	glBindVertexArray(pry_vao);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, 0);
}



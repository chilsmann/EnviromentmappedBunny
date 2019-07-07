#include "Hexagon.h"

unsigned int hex_vao;

void createHexagon()
{
	// position of the vertices in the rectangle

	GLfloat vertices[] = {
		0.0f,0.0f,0.0f, 1.0f, // 0
		-0.45f, -0.75f, 0.0f, 1.0f,//1
		0.45f, -0.75f, 0.0f, 1.0f,//2

		//0.0f,0.0f,0.0f, 1.0, //0
		//0.45f, -0.75f, 0.0f, 1.0f, //2
		0.90f, 0.0f, 0.0f, 1.0f, //3

		//0.0f, 0.0f, 0.0f, 1.0f, //0
		//0.90f, 0.0f, 0.0f, 1.0f,//3
		0.45f, 0.75f, 0.0f, 1.0f,//4

		//0.0f, 0.0f, 0.0f, 1.0f,//0
		//0.45f, 0.75f, 0.0f, 1.0f,//4
		-0.45f, 0.75f, 0.0f, 1.0f,//5

		//0.0f, 0.0f, 0.0f, 1.0f,//0
		//-0.45f, 0.75f, 0.0f, 1.0f,//5
		-0.90f, 0.0f, 0.0f, 1.0f,//6

		//0.0f, 0.0f, 0.0f, 1.0f,//0
		//-0.45f, -0.75f, 0.0f, 1.0f,//6
		//-0.90f, 0.0f, 0.0f, 1.0f//1


	};

	float normals[] = {
		//front
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 1.0f/*,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f */

	};

	 GLfloat hex_colors[] = {
		0.0f, 1.0f, 0.0f, 1.0f, // v0,v5,v6,v1 (top)
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
	

		0.0f, 1.0f, 0.0f, 1.0f, // v0,v5,v6,v1 (top)
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,


		0.0f, 1.0f, 0.0f, 1.0f, // v0,v5,v6,v1 (top)
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,


		0.0f, 1.0f, 0.0f, 1.0f, // v0,v5,v6,v1 (top)
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		0.0f, 1.0f, 0.0f, 1.0f, // v0,v5,v6,v1 (top)
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,

		0.0f, 1.0f, 0.0f, 1.0f, // v0,v5,v6,v1 (top)
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f 
	 
	 };

	// Indices for the hexagon strips
	const GLushort indices[] =
	{
			0,1,2,
			0,2,3,
			0,3,4,
			0,4,5,
			0,5,6,
			0,6,1
			/*
			3,4,5,
			6,7,8,
			9,10,11,
			12,13,14,
			15,16,17, */

	};

	float textCoords[18 * 2] = {
		/*
		0.5f,0.5f,
		1.0f, 0.5f,
		(1.35f/1.8f),1.0f,

		0.5f, 0.5,
		(1.35f / 1.8f),1.0f,
		(0.45f/1.8f), 1.0f,

		0.5f, 0.5f,
		(0.45f / 1.8f), 1.0f,
		0.0f, 0.5f,

		0.5f, 0.5f,
		0.0f, 0.5f,
		-(0.45f / 1.8f), 1.0f,
		//-------------------
		0.05f, 0.5f,
		(1.35f / 1.8f),0.0f,
		(0.45f / 1.8f), 0.0f,
		//0.33f, 0.0f,
		//0.66f, 0.0f,
		1.0f, 0.5f,
		(1.35f / 1.8f),1.0f,
		(0.45f / 1.8f), 1.0f,
		//0.33f, 1.0f,
		//0.66f, 1.0f,
		0.0f, 0.5f
		*/
		0.05f, 0.5f,
		0.33f, 0.0f,
		0.66f, 0.0f,

		1.0f, 0.5f,
		0.66f, 1.0f,
		0.33f, 1.0f,
		0.5f,0.5f

		
	};


	glGenVertexArrays(1, &hex_vao);
	glBindVertexArray(hex_vao);

	unsigned int handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);//didn't exsist
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);// vertex normal

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, 7 * 2 * sizeof(float), textCoords, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	/*
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hex_colors), hex_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Vertex position */

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);



}

void renderHexagon() {
	glBindVertexArray(hex_vao);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, 0);
}

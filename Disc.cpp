#include "Disc.h"

unsigned int disc_vao;

GLuint indices[NumDiscIndices];
vec4 points[NumDiscPoints + 1];
vec3 normals[NumDiscPoints + 1];
float angle_interval = 360.0f / NumDiscPoints;


void createDisc()
{
	
	int index = 0;
	int tIndices = 0;
	float theta;
	points[0][0] = 0.0;
	points[0][1] = 0.0;
	points[0][2] = 0.0;
	points[0][3] = 1.0;
	
	int r = 1;
	vec2 tex[NumDiscPoints + 1];

	tex[index][0] = 0.5;
	tex[index][1] = 0.5;

	index++;

	// position of the vertices in the rectangle
	for (int i = 0; i < NumDiscPoints; i++) {
		theta = i*(angle_interval)*kPI / 180.0f;

		points[index][0] = r*cos(theta);
		points[index][1] = r*sin(theta); //negative is anti-clockwise
		points[index][2] = 0.0;
		points[index][3] = 1.0;

		tex[index][0] = ((1 + cos(theta)) / 2);
		tex[index][1] = ((1 + sin(theta)) / 2);
		

		if (i <= (NumDiscPoints - 2)) {
			indices[tIndices] = 0; 
			tIndices++;
			indices[tIndices] = index; 
			tIndices++;
			indices[tIndices] = index +1; 
			tIndices++;
			

		}
		else {
			indices[tIndices] = 0; 
			tIndices++;
			indices[tIndices] = index; 
			tIndices++;
			indices[tIndices] = 1; 
			tIndices++;
		}

		index++;
	}

	for (int y = 0; y < NumDiscIndices; y += 3) {
		vec4 p1 = points[indices[y]];
		vec4 p2 = points[indices[y + 1]];
		vec4 p3 = points[indices[y + 2]];

		vec3 normal = normalize(cross(vec3(p2 - p1), vec3(p3 - p1)));// triangle normal

		normals[indices[y]] += normal;
		normals[indices[y + 1]] += normal;
		normals[indices[y + 2]] += normal;
	}
	for (int x = 0; x < NumDiscPoints; x++) {
		normals[x] = normalize(vec3(normals[x]));
	}



	glGenVertexArrays(1, &disc_vao);
	glBindVertexArray(disc_vao);

	unsigned int handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(tex), tex, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);



}

void renderDisc() {
	glBindVertexArray(disc_vao);
	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);
}



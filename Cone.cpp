#include "Cone.h"

unsigned int cone_vao;

GLuint indice[NumDiscIndice];
vec4 point[NumDiscPoint + 1];
vec3 normalz[NumDiscPoint + 1];
float angleC = 360.0f / NumDiscPoint;


void createCone()
{

	int index = 0;
	int tIndices = 0;
					 //Center point 
	float theta;
	point[0][0] = 0.0;
	point[0][1] = 0.0;
	point[0][2] = 2.0;
	point[0][3] = 1.0;

	
	
	int r = 1;
	vec2 tex[NumDiscPoint + 1];

	tex[index][0] = 0.5;
	tex[index][1] = 0.5;

	index++;

	// position of the vertices in the rectangle
	for (int i = 0; i < NumDiscPoint; i++) {
		theta = i*(angleC)*PI / 180.0f;

		point[index][0] = r*cos(theta);
		point[index][1] = r*sin(theta); //negative is anti-clockwise
		point[index][2] = 0.0;
		point[index][3] = 1.0;

		tex[index][0] = (1 + cos(theta))/2;
		tex[index][1] = (1 + sin(theta))/2;


		if (i <= (NumDiscPoint - 2)) {
			indice[tIndices] = 0;
			tIndices++;
			indice[tIndices] = index;
			tIndices++;
			indice[tIndices] = index + 1;
			tIndices++;


		}
		else {
			indice[tIndices] = 0;
			tIndices++;
			indice[tIndices] = index;
			tIndices++;
			indice[tIndices] = 1;
			tIndices++;
		}

		index++;
	}
	
	for (int y = 0; y < NumDiscIndice; y += 3) {
		vec4 p1 = point[indice[y]];
		vec4 p2 = point[indice[y + 1]];
		vec4 p3 = point[indice[y + 2]];

		vec3 normal = normalize(cross(vec3(p2 - p1), vec3(p3 - p1)));// triangle normal

		normalz[indice[y]] += normal;
		normalz[indice[y + 1]] += normal;
		normalz[indice[y + 2]] += normal;
	}
	for (int x = 0; x <= NumDiscPoint; x++) {
		normalz[x] = normalize(vec3(normalz[x]));
	}
	


	glGenVertexArrays(1, &cone_vao);
	glBindVertexArray(cone_vao);

	unsigned int handle[4]; 
	glGenBuffers(4, handle); 

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position
								   
	
	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normalz), normalz, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex), tex, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indice), indice, GL_STATIC_DRAW);

	glBindVertexArray(0);



}

void renderCone() {
	glBindVertexArray(cone_vao);
	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);
}
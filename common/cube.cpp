/* cube.cpp
Exactly the same as the cube given to us in class however I removed drawmode.
*/

#include "cube.h"

/* I don't like using namespaces in header files but have less issues with them in
seperate cpp files */
using namespace std;

/* Define the vertex attributes for vertex positions and normals.
Make these match your application and vertex shader
You might also want to add colours and texture coordinates */
Cube::Cube()
{
	attribute_v_coord = 0;
	attribute_v_normal = 1;
	attribute_v_colours = 3;	
	numvertices = 12;
	this->drawmode = drawmode;
}


Cube::~Cube()
{
}


/* Make a cube from hard-coded vertex positions and normals  */
void Cube::makeCube()
{
	/* Define vertices for a cube in 12 triangles */
	GLfloat vertexPositions[] =
	{
		-0.25f, 0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f, 0.25f, -0.25f,
		-0.25f, 0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, -0.25f,

		0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,
		0.25f, 0.25f, -0.25f,

		0.25f, -0.25f, 0.25f,
		-0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, 0.25f,
		-0.25f, 0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, 0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, -0.25f,
		-0.25f, 0.25f, -0.25f,
		-0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, 0.25f,
		0.25f, -0.25f, 0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, 0.25f,

		-0.25f, 0.25f, -0.25f,
		0.25f, 0.25f, -0.25f,
		0.25f, 0.25f, 0.25f,

		0.25f, 0.25f, 0.25f,
		-0.25f, 0.25f, 0.25f,
		-0.25f, 0.25f, -0.25f,
	};

	/* Manually specified colours for our cube */
	float vertexColours[] = {
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,

		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,

		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,

		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,

		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,

		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
		1.0f, 0.9f, 0.9f, 1.0f,
	};

	/* Manually specified normals for our cube */
	GLfloat normals[] =
	{
		0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
		0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
		1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
		1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
		0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
		0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
		-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
		-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
		0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
		0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
		0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
		0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
	};

	//Create vertex array
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	/* Create the vertex buffer for the cube */
	glGenBuffers(1, &positionBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create the colours buffer for the cube */
	glGenBuffers(1, &colourObject);
	glBindBuffer(GL_ARRAY_BUFFER, colourObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColours), vertexColours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create the normals  buffer for the cube */
	glGenBuffers(1, &normalsBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


/* Draw the cube by bining the VBOs and drawing triangles */
void Cube::drawCube(Shader shader)
{
	glBindVertexArray(this->vao);
	shader.use();

	/* Bind cube vertices. Note that this is in attribute index attribute_v_coord */
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(attribute_v_coord);
	glVertexAttribPointer(attribute_v_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind cube colours. Note that this is in attribute index attribute_v_colours */
	glBindBuffer(GL_ARRAY_BUFFER, colourObject);
	glEnableVertexAttribArray(attribute_v_colours);
	glVertexAttribPointer(attribute_v_colours, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind cube normals. Note that this is in attribute index attribute_v_normal */
	glEnableVertexAttribArray(attribute_v_normal);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
	glVertexAttribPointer(attribute_v_normal, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Draw Cube
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, numvertices * 3);

}
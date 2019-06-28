/* cube.h
Exactly the same as the cube given to us in class however I removed drawmode.
*/

#pragma once

#include "wrapper_glfw.h"
#include "shader.h"
#include <vector>
#include <glm/glm.hpp>

class Cube
{
public:
	Cube();
	~Cube();

	void makeCube();
	void drawCube(Shader);

	int numvertices;
	int drawmode;

private:
	// Define vertex buffer object names (e.g as globals)
	GLuint vao;

	GLuint positionBufferObject;
	GLuint colourObject;
	GLuint normalsBufferObject;

	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_v_colours;
};

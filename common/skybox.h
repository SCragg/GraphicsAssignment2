#pragma once

#include "wrapper_glfw.h"
#include "shader.h"
#include <vector>
#include <glm/glm.hpp>

#include "SOIL.h"

class Skybox
{
public:
	Skybox();
	~Skybox();

	void makeCube(Shader);
	void drawCube(Shader);

	int numvertices;
	int drawmode;

private:
	// Define vertex buffer object names (e.g as globals)
	GLuint vao;

	GLuint positionBufferObject;

	GLuint attribute_v_coord;

	GLuint cubemapTextureID;
	unsigned int loadCubemap(vector<std::string> faces);
};

#pragma once
/* terrain_object.h
   Example class to show how to create and render a height map
   Iain Martin November 2018
*/

#pragma once

#include "wrapper_glfw.h"
#include <vector>
#include <random>
#include <glm/glm.hpp>

class terrain_object
{
public:
	terrain_object(int octaves, GLfloat freq, GLfloat scale);
	~terrain_object();

	void calculateNoise();
	void createTerrain(GLuint xp, GLuint yp, GLfloat xs, GLfloat ys);
	void calculateNormals();
	void stretchToRange(GLfloat min, GLfloat max);
	void calculateColours();

	void createObject();
	void drawObject(int drawmode);

	glm::vec3 *vertices;
	glm::vec3 *normals;
	glm::vec4 *colours;
	std::vector<GLuint> elements;
	GLfloat* noise;

	GLuint vbo_mesh_vertices;
	GLuint vbo_mesh_normals;
	GLuint vbo_mesh_colours;
	GLuint ibo_mesh_elements;
	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_v_colour;

	GLuint xsize;
	GLuint zsize;
	GLfloat width;
	GLfloat height;
	GLuint perlin_octaves;
	GLfloat perlin_freq;
	GLfloat perlin_scale;
	GLfloat height_scale;

	float height_min, height_max;	// range of terrain heights
};


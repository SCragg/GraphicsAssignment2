/*

Adapted from the point sprite class given to us in the labs

*/
#pragma once

#include <glm/glm.hpp>
#include <random>
#include "wrapper_glfw.h"

class Fireflies
{
public:
	Fireflies( GLint reso, GLfloat siz, GLfloat speed);
	~Fireflies();

	void create();
	void draw();
	void animate(GLint counter);
	void updateParams(GLfloat dist, GLfloat sp);

	glm::vec3 *vertices;
	glm::vec4 *colours;
	glm::vec3 *velocity;

	GLuint numpoints;		// Number of particles
	GLuint vao;
	GLuint vertex_buffer;
	GLuint colour_buffer;

	//Size
	GLfloat size;

	//Radius
	GLint resolution;

	// Particle speed
	GLfloat speed;

	// Particle max distance fomr the origin before we change direction back to the centre
	GLfloat maxdist;
};
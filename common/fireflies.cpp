/*
I adapted the point sprite class given to us in class.

I changed the create and animate functions to be more suitable to represent fireflies
 */

#include "fireflies.h"
#include "glm/gtc/random.hpp"

 /* Constructor, set initial parameters*/
Fireflies::Fireflies(GLint reso, GLfloat siz, GLfloat speed)
{
	resolution = reso;
	size = siz;
	this->speed = speed;
}

Fireflies::~Fireflies()
{
	delete[] colours;
	delete[] vertices;
}

void Fireflies::updateParams(GLfloat dist, GLfloat sp)
{
	maxdist = dist;
	speed = sp;
}

/*
	Creates a plane of points using the given size and resolution uses a random number generator to move each point from the grid in all directions
*/
void  Fireflies::create()
{
	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	vertices = new glm::vec3[resolution * resolution];
	colours = new glm::vec4[resolution * resolution];
	velocity = new glm::vec3[resolution * resolution];

	/* Define starting position and size of step */
	GLfloat startpos = -size / 2;
	GLfloat step = size / resolution;
	std::mt19937 generator(1);
	//Define random distributions
	std::uniform_real_distribution<double> distribution(-0.03, 0.03);
	std::uniform_real_distribution<double> distributioncol(0.0, 0.2);
	std::uniform_real_distribution<double> distributiony(-1, 1);

	GLint counter = 0;
	for (int i = 0; i < resolution; i++)
	{
		for (int j = 0; j < resolution; j++)
		{
			vertices[counter] = glm::vec3((startpos + i*step) + distribution(generator), distributiony(generator), (startpos + j*step) + distribution(generator));
			colours[counter] = glm::vec4(distributioncol(generator) + 0.6, distributioncol(generator) + 0.8, 0.4, 0.9);
			velocity[counter] = glm::vec3(0);
			counter++;
		}
	}

	/* Create the vertex buffer object */
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, resolution * resolution * sizeof(glm::vec3), vertices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &colour_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, colour_buffer);
	glBufferData(GL_ARRAY_BUFFER, resolution * resolution * sizeof(glm::vec4), colours, GL_STATIC_DRAW);
}


void Fireflies::draw()
{
	glBindVertexArray(vao);

	/* Bind vertices. Note that this is in attribute index 0 */
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind cube colours. Note that this is in attribute index 1 */
	glBindBuffer(GL_ARRAY_BUFFER, colour_buffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* Draw our points*/
	glDrawArrays(GL_POINTS, 0, resolution * resolution);
}

/*
Uses additive sin functions to create a smooth yet random animation for each firefly this is the same method for creating the grass animations.
I ensure each particle has the same period and offset the input value to prevent all the particles from hitting the null point at the same time.
*/
void Fireflies::animate(GLint counter)
{
	for (int i = 0; i < (resolution * resolution); i++)
	{
		std::mt19937 generator(i);
		std::uniform_real_distribution<double> distribution(0.0, 0.1);
		std::uniform_int_distribution<int> distributionint(0, 1799);


		GLint rand_offset = distributionint(generator);

		//xyx random velocities
		GLfloat velox = speed * (glm::sin(glm::sin(distribution(generator)*(glm::sin(distribution(generator)*(glm::sin(glm::radians((counter + rand_offset)*0.2))))))));
		GLfloat veloy = speed * (glm::sin(glm::sin(distribution(generator)*(glm::sin(distribution(generator)*(glm::sin(glm::radians((counter + rand_offset)*0.4))))))));
		GLfloat veloz = speed * (glm::sin(glm::sin(distribution(generator)*(glm::sin(distribution(generator)*(glm::sin(glm::radians((counter + rand_offset)*0.2))))))));

		velocity[i] = glm::vec3(velox, veloy, veloz);

		// Add velocity to the vertices q
		vertices[i] += velocity[i];

		/*
		Alter transparency of particles over time, gives impression of fireflies fading in and out
		over time, once again ofset the counter to ensure they don't all fade in and out in unison.
		*/
		GLfloat alpha_time = glm::sin(glm::radians((counter + rand_offset)*0.4));
		colours[i] = glm::vec4(colours[i].r, colours[i].g, colours[i].b, alpha_time);
		
	}

	// Update the vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, resolution * resolution * sizeof(glm::vec3), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colour_buffer);
	glBufferData(GL_ARRAY_BUFFER, resolution * resolution * sizeof(glm::vec4), colours, GL_STATIC_DRAW);
}




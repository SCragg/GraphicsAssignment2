/* 
Adapted from cube.h file given to us in class. I used https://learnopengl.com/Advanced-OpenGL/Cubemaps
to help me implement the skybox/ The function used to load the cubemap texture and to draw the skybox
came from there.
*/

#include "skybox.h"

/* I don't like using namespaces in header files but have less issues with them in
seperate cpp files */
using namespace std;

/* Define the vertex attributes for vertex positions and normals.
Make these match your application and vertex shader
You might also want to add colours and texture coordinates */
Skybox::Skybox()
{
	attribute_v_coord = 0;
	numvertices = 12;
	this->drawmode = drawmode;
}


Skybox::~Skybox()
{
}


/* Make a cube from hard-coded vertex positions and normals  */
void Skybox::makeCube(Shader shader)
{
	/* Define vertices for a cube in 12 triangles */
	GLfloat vertexPositions[] =
	{
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,

		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,

		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,

		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,

		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
	};

	//Create vertex array
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	/* Create the vertex buffer for the cube */
	glGenBuffers(1, &positionBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	vector<std::string> faces
	{
		"Textures/right.png",
		"Textures/left.png",
		"Textures/up.png",
		"Textures/down.png",
		"Textures/front.png",
		"Textures/back.png",
	};

	glActiveTexture(GL_TEXTURE0);
	cubemapTextureID = loadCubemap(faces);
}


/* Draw the cube by bining the VBOs and drawing triangles. I used https://learnopengl.com/Advanced-OpenGL/Cubemaps
to help me properly draw the cube map.
*/
void Skybox::drawCube(Shader shader)
{

	glBindVertexArray(this->vao);
	glDepthMask(GL_FALSE);
	shader.use();

	/* Bind cube vertices. Note that this is in attribute index attribute_v_coord */
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(attribute_v_coord);
	glVertexAttribPointer(attribute_v_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Draw Cube
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, numvertices * 3);
	glDepthMask(GL_TRUE);
}

/*
I took this function from https://learnopengl.com/Advanced-OpenGL/Cubemaps to load my cube map texture
*/
unsigned int Skybox::loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = SOIL_load_image(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			SOIL_free_image_data(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			SOIL_free_image_data(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
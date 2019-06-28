/* shader.h
I abstracted the shader to its own class.
The functions used to load the shader are taken directly from the wrapper_glfw.

Some functions are also taken from https://learnopengl.com/Getting-started/Shaders
which are used within the mesh and model classes and for general functionality. 
*/


#ifndef SHADER_H //taken from https://learnopengl.com/Getting-started/Shaders
#define SHADER_H //taken from https://learnopengl.com/Getting-started/Shaders

// include glload to get all the required OpenGL headers
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

class Shader
{
public:
	// the program ID
	GLuint ID;

	// constructors
	Shader();
	//This will call loadShader in constructor
	Shader(const char* vertexPath, const char* fragmentPath);

	// use/activate the shader: taken from https://learnopengl.com/Getting-started/Shaders
	void use();

	//Load shaders in to the program - taken from wrapper_glfw.
	void LoadShader(const char *vertex_path, const char *fragment_path);
	void LoadShader(const char *vertex_path, const char *fragment_path, const char *geometry_path);

	//Uniform utilities: taken from https://learnopengl.com/Getting-started/Shaders
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;

private:
	/* Shader build and file read functions - taken from wrapper_GLFW*/
	GLuint BuildShader(GLenum eShaderType, const string &shaderText);
	string readFile(const char *filePath);
};

#endif

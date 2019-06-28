/* shader.cpp
I abstracted the shader to its own class.
The functions used to load the shader are taken directly from the wrapper_glfw.

I got the idea of seperating this in to its own class from https://learnopengl.com/Getting-started/Shaders.

Some functions are also taken from https://learnopengl.com/Getting-started/Shaders
which are used within the mesh and model classes and for general functionality.
*/

#include "shader.h"

Shader::Shader()
{

}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	LoadShader(vertexPath, fragmentPath);
}

//Use this shader for rendering: taken from https://learnopengl.com/Getting-started/Shaders
void Shader::use()
{
	glUseProgram(ID);
}

/* Load vertex and fragment shader and return the compiled program: taken from wrapper_GLFW */
void Shader::LoadShader(const char *vertex_path, const char *fragment_path)
{
	GLuint vertShader, fragShader;

	// Read shaders
	string vertShaderStr = readFile(vertex_path);
	string fragShaderStr = readFile(fragment_path);

	GLint result = GL_FALSE;
	int logLength;

	vertShader = BuildShader(GL_VERTEX_SHADER, vertShaderStr);
	fragShader = BuildShader(GL_FRAGMENT_SHADER, fragShaderStr);

	cout << "Linking program" << endl;
	ID = glCreateProgram();
	glAttachShader(ID, vertShader);
	glAttachShader(ID, fragShader);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &result);
	glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &logLength);
	vector<char> programError((logLength > 1) ? logLength : 1);
	glGetProgramInfoLog(ID, logLength, NULL, &programError[0]);
	cout << &programError[0] << endl;

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}

void Shader::LoadShader(const char *vertex_path, const char *fragment_path, const char *geometry_path)
{
	GLuint vertShader, fragShader, geomShader;

	// Read shaders
	string vertShaderStr = readFile(vertex_path);
	string fragShaderStr = readFile(fragment_path);
	string geomShaderStr = readFile(geometry_path);

	GLint result = GL_FALSE;
	int logLength;

	vertShader = BuildShader(GL_VERTEX_SHADER, vertShaderStr);
	fragShader = BuildShader(GL_FRAGMENT_SHADER, fragShaderStr);
	geomShader = BuildShader(GL_GEOMETRY_SHADER, geomShaderStr);

	cout << "Linking program" << endl;
	ID = glCreateProgram();
	glAttachShader(ID, vertShader);
	glAttachShader(ID, geomShader);
	glAttachShader(ID, fragShader);

	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &result);
	glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &logLength);
	vector<char> programError((logLength > 1) ? logLength : 1);
	glGetProgramInfoLog(ID, logLength, NULL, &programError[0]);
	cout << &programError[0] << endl;

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	glDeleteShader(geomShader);
}

//Uniform Functions: taken from https://learnopengl.com/Getting-started/Shaders
void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

/* Build shaders from strings containing shader source code: taken from wrapper_GLFW */ 
GLuint Shader::BuildShader(GLenum eShaderType, const string &shaderText)
{
	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = shaderText.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		// Output the compile errors

		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		cerr << "Compile error in " << strShaderType << "\n\t" << strInfoLog << endl;
		delete[] strInfoLog;

		throw exception("Shader compile exception");
	}

	return shader;
}

/* Read a text file into a string: taken from wrapper_GLFW*/
string Shader::readFile(const char *filePath)
{
	string content;
	ifstream fileStream(filePath, ios::in);

	if (!fileStream.is_open()) {
		cerr << "Could not read file " << filePath << ". File does not exist." << endl;
		return "";
	}

	string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}





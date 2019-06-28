/*
wrapper_glfw.h
The same wrapper_glfw file given to us in class,
however I have removed the shader loading functions and have put them in to their own class.
*/
#pragma once

#include <string>

/* Inlcude GL_Load and GLFW */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>
#include <GLFW/glfw3.h>

class GLWrapper {
private:

	int width;
	int height;
	const char *title;
	double fps;
	void(*error_callback)(int error, const char* description);
	void(*shader)();
	void(*renderer)();
	void(*reshape)(GLFWwindow* window, int w, int h);
	void(*keyCallBack)(GLFWwindow* window, int key, int scancode, int action, int mods);
	bool running;
	GLFWwindow* window;

public:
	GLWrapper(int width, int height, const char *title);
	~GLWrapper();

	void setFPS(double fps) {
		this->fps = fps;
	}

	void DisplayVersion();

	/* Callback registering functions */
	void setRenderer(void(*f)());
	void setReshapeCallback(void(*f)(GLFWwindow* window, int w, int h));
	void setKeyCallback(void(*f)(GLFWwindow* window, int key, int scancode, int action, int mods));
	void setErrorCallback(void(*f)(int error, const char* description));

	int eventLoop();
	GLFWwindow* getWindow();
};




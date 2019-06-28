/*
wrapper_glfw.cpp
The same wrapper_glfw file given to us in class,
however I have removed the load shader functions and have put them in to their own class.
*/

#include "wrapper_glfw.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

/* Constructor for wrapper object */
GLWrapper::GLWrapper(int width, int height, const char *title) {

	this->width = width;
	this->height = height;
	this->title = title;
	this->fps = 60;
	this->running = true;

	/* Initialise GLFW and exit if it fails */
	if (!glfwInit()) 
	{
		cout << "Failed to initialize GLFW." << endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef DEBUG
	glfwOpenWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	window = glfwCreateWindow(width, height, title, 0, 0);
	if (!window){
		cout << "Could not open GLFW window." << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	/* Obtain an OpenGL context and assign to the just opened GLFW window */
	glfwMakeContextCurrent(window);

	/* Initialise GLLoad library. You must have obtained a current OpenGL */
	if (!ogl_LoadFunctions())
	{
		cerr << "oglLoadFunctions() failed. Exiting" << endl;
		glfwTerminate();
		return;
	}

	/* Can set the Window title at a later time if you wish*/
	glfwSetWindowTitle(window, this->title);

	glfwSetInputMode(window, GLFW_STICKY_KEYS, true);

}

/* Terminate GLFW on destruvtion of the wrapper object */
GLWrapper::~GLWrapper() {
	glfwTerminate();
}

/* Returns the GLFW window handle, required to call GLFW functions outside this class */
GLFWwindow* GLWrapper::getWindow()
{
	return window;
}


// Print OpenGL Version

void GLWrapper::DisplayVersion()
{
	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MAJOR_VERSION, &minor);
	cout << "OpenGL Version = " << major << "." << minor << endl;

}


/*
GLFW_Main function normally starts the windows system, calls any init routines
and then starts the event loop which runs until the program ends
*/
int GLWrapper::eventLoop()
{
	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		
		// Call function to draw your graphics
		renderer();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}


/* Register an error callback function */
void GLWrapper::setErrorCallback(void(*func)(int error, const char* description))
{
	this->error_callback = func;
	glfwSetErrorCallback(func);
}

/* Register a display function that renders in the window */
void GLWrapper::setRenderer(void(*func)()) {
	this->renderer = func;
}

/* Register a callback that runs after the window gets resized */
void GLWrapper::setReshapeCallback(void(*func)(GLFWwindow* window, int w, int h)) {
	reshape = func;
	glfwSetFramebufferSizeCallback(window, reshape);
}

/* Register a callback to respond to keyboard events */
void GLWrapper::setKeyCallback(void(*func)(GLFWwindow* window, int key, int scancode, int action, int mods))
{	
	keyCallBack = func;
	glfwSetKeyCallback(window, keyCallBack);
}

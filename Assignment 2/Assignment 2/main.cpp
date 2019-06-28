/*

Sean Cragg Assignment 2: Grass.

I used my previous assignment as a starting point for this project.

The bulk of this scene is done in the geometry shader to produce the grass.

Performance intensive, I still need to work on optimizing it to help run smoother.

If you find that performance is an issue reduce particle density and heightfield size and resolution
in the init function.

*/

//Linking to libraries - from lab examples
#ifdef _DEBUG
#pragma comment(lib, "glfw3D.lib")
#pragma comment(lib, "glloadD.lib")
#else
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glload.lib")

#endif
#pragma comment(lib, "opengl32.lib")

/* Include the header to the GLFW wrapper class which
also includes the OpenGL extension initialisation*/
#include "wrapper_glfw.h"
#include <iostream>
#include <stack>

/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

// Include headers for our objects
#include "shader.h"
#include "cube.h"
#include "skybox.h"
#include "terrain_object.h"
#include "fireflies.h"

using namespace std;
using namespace glm;

//Variable Declarations
/* Position and view globals */
GLfloat angle_x, angle_inc_x, model_scale;
GLfloat angle_y, angle_inc_y, angle_z, angle_inc_z;
GLfloat move_x, move_y, move_z;

GLfloat aspect_ratio;		// Aspect ratio of the window defined in the reshape callback - from lab example
GLfloat lightx, lighty, lightz; //Light position values from positional light lab

//Flags for controls and counter for animations
GLboolean showlight;
GLboolean two_sided;
GLint grass_windcounter;

/* Uniforms*/
//For basic Shader
GLuint modelID, viewID, projectionID, normalmatrixID, lightposID, lightmodeID;
//For Skybox Shader
GLuint skyviewID, skyprojectionID;
//For Grass Shader
GLuint grassmodelID, grassviewID, grassprojectionID, grassnormalmatrixID, grasslightposID, grass_windcounterID, grass_twosidedID;
//For Particle Shader
GLuint fireflymodelID, fireflyviewID, fireflyprojectionID, fireflycolourmodeID, fireflypoint_sizeID;

/* Global instances of our objects */
Shader basicShader;
Shader skyboxShader;
Shader grassShader;
Shader fireflyShader;
Skybox skybox;
Cube aCube;

//Terrain variables
terrain_object *heightfield, *heightfield2;
int octaves;
GLfloat perlin_scale;
GLfloat perlin_frequency;
GLfloat land_size;
GLuint land_resolution, land_resolution2;

/* Firefly object and adjustable parameters */
Fireflies *firefly_part;
GLfloat speed;
GLint particledensity;
GLfloat particlearea;
GLfloat point_size;		// Used to adjust point size in the vertex shader

/*
This function is called before entering the main rendering loop. Initialisations.
*/
void init(GLWrapper *glw)
{
	/* Set the view transformation controls to their initial values -from lab exaplmes */
	angle_x = 7.6;
	angle_y = 112.8;
	angle_z = 0;
	angle_inc_x = angle_inc_y = angle_inc_z = 0;
	move_x = -0.36;
	move_y = -0.72;
	move_z = -3.96;

	model_scale = 1.0f;
	aspect_ratio = 1024.f / 768.f;	// Initial aspect ratio from window size - from lab examples

	//light position values
	lightx = -0.4;
	lighty = 0.22;
	lightz = -1;

	//initial iteration counter, used for wind and particle animations
	grass_windcounter = 0;
	showlight = false;
	two_sided = false
		;

	/* Load shaders in to shader objects */
	
	try
	{
		basicShader.LoadShader("..\\..\\shaders\\Basic.vert", "..\\..\\shaders\\Basic.frag");
	}
	catch (exception &e)
	{
		cout << "Caught exception: " << e.what() << endl;
		cin.ignore();
		exit(0);
	}

	try
	{
		skyboxShader.LoadShader("..\\..\\shaders\\Skybox.vert", "..\\..\\shaders\\Skybox.frag");
	}
	catch (exception &e)
	{
		cout << "Caught exception: " << e.what() << endl;
		cin.ignore();
		exit(0);
	}

	try
	{
		grassShader.LoadShader("..\\..\\shaders\\Grass.vert", "..\\..\\shaders\\Grass.frag", "..\\..\\shaders\\Grass.geom");
	}
	catch (exception &e)
	{
		cout << "Caught exception: " << e.what() << endl;
		cin.ignore();
		exit(0);
	}

	try
	{
		fireflyShader.LoadShader("..\\..\\shaders\\Fireflies.vert", "..\\..\\shaders\\Fireflies.frag");
	}
	catch (exception &e)
	{
		cout << "Caught exception: " << e.what() << endl;
		cin.ignore();
		exit(0);
	}

	/* Define uniforms to send to basic shader */
	modelID = glGetUniformLocation(basicShader.ID, "model");
	viewID = glGetUniformLocation(basicShader.ID, "view");
	projectionID = glGetUniformLocation(basicShader.ID, "projection");
	lightposID = glGetUniformLocation(basicShader.ID, "lightpos");
	normalmatrixID = glGetUniformLocation(basicShader.ID, "normalmatrix");
	lightmodeID = glGetUniformLocation(basicShader.ID, "lightmode");

	//Uniforms for skybox shader
	skyviewID = glGetUniformLocation(skyboxShader.ID, "view");
	skyprojectionID = glGetUniformLocation(skyboxShader.ID, "projection");

	//Uniform for grass shader
	grassmodelID = glGetUniformLocation(grassShader.ID, "model");
	grassviewID = glGetUniformLocation(grassShader.ID, "view");
	grassprojectionID = glGetUniformLocation(grassShader.ID, "projection");
	grasslightposID = glGetUniformLocation(grassShader.ID, "lightpos");
	grassnormalmatrixID = glGetUniformLocation(grassShader.ID, "normalmatrix");
	grass_windcounterID = glGetUniformLocation(grassShader.ID, "wind_counter");
	grass_twosidedID = glGetUniformLocation(grassShader.ID, "two_sided");

	//Uniform for particle shader
	fireflymodelID = glGetUniformLocation(fireflyShader.ID, "model");
	fireflycolourmodeID = glGetUniformLocation(fireflyShader.ID, "colourmode");
	fireflypoint_sizeID = glGetUniformLocation(fireflyShader.ID, "size");
	fireflyviewID = glGetUniformLocation(fireflyShader.ID, "view");
	fireflyprojectionID = glGetUniformLocation(fireflyShader.ID, "projection");

	//create cube object
	aCube.makeCube();

	//Create Skybox
	skybox.makeCube(skyboxShader);

	/* 
	Create the heightfield object - taken from lab example
	
	If your system cannot run this programe smoothly turn down size and resolution.
	*/
	octaves = 4;
	perlin_scale = 4.f;
	perlin_frequency = 1.5f;
	land_size = 5.f;
	land_resolution = 180;
	heightfield = new terrain_object(octaves, perlin_frequency, perlin_scale);
	heightfield->createTerrain(land_resolution, land_resolution, land_size, land_size);
	heightfield->createObject();

	land_resolution2 = 35;
	heightfield2 = new terrain_object(octaves, perlin_frequency, perlin_scale);
	heightfield2->createTerrain(land_resolution2, land_resolution2, land_size, land_size);
	heightfield2->createObject();

	/*
	Particle Parameters

	Reducing Particle Density will improve performance.
	*/
	speed = 0.1f;
	particledensity = 30;
	particlearea = 5;
	firefly_part = new Fireflies(particledensity, particlearea, speed);
	firefly_part->create();
	point_size = 30;

	// Enable gl_PointSize
	glEnable(GL_PROGRAM_POINT_SIZE);	

	//Display instructions
	cout << "Sean Cragg: Assignment 2: Grass" << endl << endl << "CONTROLS:" << endl << endl <<
		"Camera" << endl << "Rotate scene around x-axis: Q, W" << endl <<
		"Rotate scene around y-axis: E, R" << endl << "Move camera UP, DOWN, LEFT, RIGHT: Arrow keys" <<
		endl << "Move camera forwards and backwards: Right Shift, Right Ctrl" << endl << endl <<
		"Lighting" << endl << "Show/hide light source: B" << endl << "Move light source x-axis: O, P" << endl <<
		"Move light source y-axis: K, L" << endl << "Move light source z-axis N, M" << endl <<
		"Toggle between one-sided and two-sided lighting: V" << endl << endl;
}

/* Called to update the display. Note that this function is called in the event loop in the wrapper
class because we registered display as a callback function */
void display()
{
	/* Define the background colour */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Enable depth test  */
	glEnable(GL_DEPTH_TEST);

	/* Enable Blending for particles */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Projection matrix : 30° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units - from lab example
	mat4 projection = perspective(radians(30.0f), aspect_ratio, 0.1f, 100.0f);

	// Camera matrix
	mat4 view = lookAt(
		vec3(0, 0, 0), // Camera is at (0,0,0), in World Space
		vec3(0, 0, -1), // looks at 0,0,-1
		vec3(0, 1, 0)  // up is positive y.
	);

	view = translate(view, vec3(move_x, move_y, move_z));
	view = rotate(view, -radians(angle_x), vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	view = rotate(view, -radians(angle_y), vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	view = rotate(view, -radians(angle_z), vec3(0, 0, 1)); //rotating in clockwise direction around z-axis

	// Define the light position and transform by the view matrix - from lab example
	vec4 lightpos = view * vec4(lightx, lighty, lightz, 1.0);

	// Send our projection and view uniforms and light position to the main shader
	basicShader.use();
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
	glUniform4fv(lightposID, 1, value_ptr(lightpos));

	// Send our projection and view uniforms and light position to the grass shader
	grassShader.use();
	glUniformMatrix4fv(grassviewID, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(grassprojectionID, 1, GL_FALSE, &projection[0][0]);
	glUniform4fv(grasslightposID, 1, value_ptr(lightpos));

	//Send view and projection to particle shader
	fireflyShader.use();
	glUniformMatrix4fv(fireflyviewID, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(fireflyprojectionID, 1, GL_FALSE, &projection[0][0]);

	// Send View and Projection to skybox shader
	skyboxShader.use();
	view = glm::mat4(glm::mat3(view)); //Removes translation from view model ensures that skybox will remain same size
	glUniformMatrix4fv(skyviewID, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(skyprojectionID, 1, GL_FALSE, &projection[0][0]);

	//Declare lightmode variable for shaders using basicShader
	GLint lightmode;

	// Define our model transformation in a stack and 
	// push the identity matrix onto the stack
	stack<mat4> model;
	model.push(mat4(1.0f));

	// Declare the normal matrix
	mat3 normalmatrix;
	
	//This block draws the skybox
	skybox.drawCube(skyboxShader);

	//This block controls the grass
	model.push(model.top());
	{
		model.top() = translate(model.top(), vec3(0, 0, 0));
		model.top() = scale(model.top(), vec3(model_scale, model_scale, model_scale));

		//Enable face culling if using two sided lighting
		if (two_sided)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}

		//Send uniforms and draw grass
		grassShader.use();
		glUniform1i(grass_windcounterID, grass_windcounter);
		glUniform1i(grass_twosidedID, two_sided);
		glUniformMatrix4fv(grassmodelID, 1, GL_FALSE, &model.top()[0][0]);
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(grassnormalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
		heightfield->drawObject(0);

		//Disable face culling
		glDisable(GL_CULL_FACE);
	}
	model.pop();

	//This block terrain under the grass
	model.push(model.top());
	{
		model.top() = translate(model.top(), vec3(0.2, -0.05, 0));
		model.top() = scale(model.top(), vec3(model_scale, model_scale, model_scale));

		//set light mode to emit off
		lightmode = 0;

		//Send uniforms and draw ground
		basicShader.use();
		glUniform1i(lightmodeID, lightmode);
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &model.top()[0][0]);
		glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
		heightfield2->drawObject(1);
	}
	model.pop();

	if (showlight)
	{
		//This block controls the light cube
		model.push(model.top());
		{
			model.top() = translate(model.top(), vec3(lightx, lighty, lightz));
			model.top() = scale(model.top(), vec3(0.2, 0.2, 0.2));

			//set light mode to emitlight
			lightmode = 1;

			//Send uniforms and draw cube
			basicShader.use();
			glUniform1i(lightmodeID, lightmode);
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &model.top()[0][0]);
			aCube.drawCube(basicShader);
		}
		model.pop();
	}

	//This block controls particle animations
	model.push(model.top());
	{
		model.top() = scale(model.top(), vec3(model_scale, model_scale, model_scale));
		model.top() = translate(model.top(), vec3(0, 1, 0));

		fireflyShader.use();
		// Send our uniforms variables,
		glUniformMatrix4fv(fireflymodelID, 1, GL_FALSE, &model.top()[0][0]);
		glUniform1ui(fireflycolourmodeID, 1);
		glUniform1f(fireflypoint_sizeID, point_size);
		//draw particles
		firefly_part->draw();
		firefly_part->animate(grass_windcounter);
	}
	model.pop();

	//taken from class example, spins around the whole model.
	angle_x += angle_inc_x;
	angle_y += angle_inc_y;
	angle_z += angle_inc_z;

	//increment counter for grass and particle animations
	grass_windcounter += 1;
	if (grass_windcounter >= 1800) grass_windcounter = 0;
}

/* Called whenever the window is resized. The new window size is given, in pixels. taken from lab examples */ 
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// Store aspect ratio to use for our perspective projection
	aspect_ratio = float(w) / float(h);
}

/* change view angle, exit upon ESC - adapted from lab examples*/
static void keyCallback(GLFWwindow* window, int key, int s, int action, int mods)
{
	//Closes if escape is pressed, from lab example
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//Rotates camera around scene
	if (key == 'Q') angle_inc_x -= 0.1f;
	if (key == 'Q' && action == GLFW_RELEASE) angle_inc_x = 0;
	if (key == 'W') angle_inc_x += 0.1f;
	if (key == 'W' && action == GLFW_RELEASE) angle_inc_x = 0;
	if (key == 'E') angle_inc_y -= 0.1f;
	if (key == 'E' && action == GLFW_RELEASE) angle_inc_y = 0;
	if (key == 'R') angle_inc_y += 0.1f;
	if (key == 'R' && action == GLFW_RELEASE) angle_inc_y = 0;

	//Moves camera along x, y, z axes
	if (key == GLFW_KEY_UP) move_y -= 0.09;
	if (key == GLFW_KEY_DOWN) move_y += 0.09;
	if (key == GLFW_KEY_LEFT) move_x += 0.09;
	if (key == GLFW_KEY_RIGHT) move_x -= 0.09;
	if (key == GLFW_KEY_RIGHT_CONTROL) move_z -= 0.09;
	if (key == GLFW_KEY_RIGHT_SHIFT) move_z += 0.09;

	//Move light position
	if (key == 'O') lightx += 0.04f;
	if (key == 'P') lightx -= 0.04f;
	if (key == 'K') lighty += 0.04f;
	if (key == 'L') lighty -= 0.04f;
	if (key == 'N') lightz += 0.04f;
	if (key == 'M') lightz -= 0.04f;

	//Shows/hides positional light cube
	if (key == 'B' && action == GLFW_PRESS)
	{
		if (showlight == false) showlight = true;
		else showlight = false;
	}

	//Toggle between one sided or two sided lighting for the grass.
	if (key == 'V' && action == GLFW_PRESS)
	{
		if (two_sided == false)
		{
			two_sided = true;
			cout << "Using two sided lighting." << endl;
		}
		else
		{
			two_sided = false;
			cout << "Using one sided lighting." << endl;
		}
	}
}

/* Entry point of program */
int main(int argc, char* argv[])
{
	GLWrapper *glw = new GLWrapper(1024, 768, "Hilly Scene");;

	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}

	// Register the callback functions
	glw->setRenderer(display);
	glw->setKeyCallback(keyCallback);
	glw->setReshapeCallback(reshape);
	
	/* Output the OpenGL vendor and version */
	glw->DisplayVersion();

	init(glw);

	glw->eventLoop();

	delete(glw);
	return 0;
}

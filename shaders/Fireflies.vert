// Minimal vertex shader for Point Sprite object
// Adapted from Example 6.1 in the Redbook V4.3

#version 420

// These are the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;


// Uniform variables are passed in from the application
uniform mat4 model, view, projection;
uniform uint colourmode;

// Output the vertex colour - to be rasterized into pixel fragments
out vec4 fcolour;
uniform float size;

void main()
{
	vec4 colour_h = colour;
	vec4 pos = vec4(position, 1.0);
	vec4 pos2 = view * model * pos;
	
	// Pass through the vertex colour
	fcolour = colour;

	// Define the vertex position
	gl_Position = projection * view * model * pos;

	gl_PointSize = size / distance(pos2, vec4(0,0,0,0));
}


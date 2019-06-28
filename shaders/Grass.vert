//Grass vertex shader

#version 420

// These are the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 atexturecoord;
layout(location = 3) in vec4 colour;

// Uniform variables are passed in from the application
uniform mat4 model, view, projection;
uniform mat3 normalmatrix;
uniform vec4 lightpos;

// Output the vertex colour - to be rasterized into pixel fragments
out VS_OUT {
	vec3 wposition;
	vec4 fcolour;
	vec2 texturecoord;
	mat4 gview;
	mat4 gprojection;
	mat3 gnormalmatrix;
	vec3 fragL;
	vec3 fragV;
} vs_out;


void main()
{	//Declare Variables
	vec4 position_h = vec4(position, 1.0);
	float shininess = 8;

	//Define Model-View matrix
	mat4 mv_matrix = view * model;

	//Transform Vertex Position: P
	vec4 P = mv_matrix * position_h;

	// Define light direction: L
	vec4 transformed_lightpos = mv_matrix * lightpos;
	vec3 L = lightpos.xyz - P.xyz;

	//***** Specular Calculations *****
	vec3 V = normalize(-P.xyz);
	
	//Define Outputs
	vs_out.wposition = position;
	vs_out.gview = view;
	vs_out.gprojection = projection;
	vs_out.gnormalmatrix = normalmatrix;
	vs_out.fcolour = colour;
	vs_out.texturecoord = atexturecoord;
	vs_out.fragL = L;
	vs_out.fragV = V;

	// Define the vertex position
	gl_Position = model * position_h;


}
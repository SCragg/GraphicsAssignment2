//Basic vertex shader, for lightcube and terrain

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
out vec4 fcolour;
out vec3 fragN;
out vec3 fragL;
out vec3 fragR;

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

	//***** Diffuse Calculations *****
	//Transform normal
	vec3 N = normalmatrix * normal;

	//Normalise vectors
	L = normalize(L);
	N = normalize(N);

	//***** Specular Calculations *****
	vec3 V = normalize(-P.xyz);
	vec3 R = reflect(-L, N);
	
	//Define Outputs
	fcolour = colour;
	fragL = L;
	fragN = N;
	fragR = R;

	// Define the vertex position
	gl_Position = projection * view * model * position_h;


}
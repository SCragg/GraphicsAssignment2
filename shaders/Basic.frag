//Basic Fragment shader


#version 420

in vec4 fcolour;
in vec2 texturecoord;
in vec3 fragN;
in vec3 fragL;
in vec3 fragV;
in vec3 fragR;

out vec4 outputColor;

uniform int lightmode;

//Global variables
int shininess = 8;

void main()
{

	//Define lighting colours
	vec4 specular_col = vec4(1.0f, 0.9f, 0.9f, 1.0f);
	vec4 diffuse_col;

	//Chooses whether to use diffuse texture or diffuse value colour defined in uniform
	diffuse_col= fcolour;

	//Renormalise inputs from Vertex shader
	vec3 N = normalize(fragN);
	vec3 L = normalize(fragL);
	vec3 R = normalize(fragR);

	//ambient calculation
	vec4 ambient = 0.2 * diffuse_col;
	//diffuse calculation
	vec4 diffuse = (max(dot(N, L), 0)) * diffuse_col;
	vec3 specular;

	//Outputs
	if(lightmode == 0) outputColor = ambient + vec4(0.1*diffuse.rgb, 1); //applies low lighting
	else outputColor = fcolour; //Emits vertex colour

}
// Fragment shader
//Started off from the lab3 shader and moved lighting from vertex to frag as shown in the lab

#version 420

in vec4 fcolour;
in vec2 texturecoord;
in vec3 fL;
in vec3 fV;
in vec3 fR;
in vec3 normal;

out vec4 outputColor;

uniform bool two_sided;

//Global variables
int shininess = 200;

void main()
{
	//Define lighting colours
	vec4 specular_col = vec4(1.0f, 0.9f, 0.9f, 1.0f);
	vec4 diffuse_col;

	//Chooses whether to use diffuse texture or diffuse value colour defined in uniform
	diffuse_col = fcolour;	

	//Renormalise inputs from Vertex shader
	vec3 N = normalize(normal);
	vec3 L = normalize(fL);
	vec3 R = normalize(fR);
	vec3 V = normalize(fV);

	//ambient calculation
	vec4 ambient = 0.25 * diffuse_col;

	//diffuse calculation
	vec4 diffuse = (max(dot(N, L), 0)) * diffuse_col;
	//specular calculation
	float spec_val = pow(max(dot(R, V), 0.0), shininess);
	//vec3 specular;
	vec3 specular = specular_col.xyz * spec_val * 0.3;

	//Outputs
	outputColor = ambient + (diffuse * 0.25) + vec4(specular, 1);

}
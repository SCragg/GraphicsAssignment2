#version 420 core
layout (points) in;
layout (triangle_strip, max_vertices = 14) out;

in VS_OUT {
	vec3 wposition;
	vec4 fcolour;
	vec2 texturecoord;
	mat4 gview;
	mat4 gprojection;
	mat3 gnormalmatrix;
	vec3 fragL;
	vec3 fragV;
} gs_in[];

uniform int wind_counter;
uniform bool two_sided;

out vec4 fcolour;
out vec3 normal;
out vec3 fL;
out vec3 fR;
out vec3 fV;

//Function declarations
float rand(vec2 co);
void generate_grass();
void bend_grass(float grass);
void calculate_normals();
void draw_grass(bool reverse);

//*************Global variables******************
//Random values
float rand1 = rand(gs_in[0].wposition.xy);
float rand2 = rand(gs_in[0].wposition.xz);
float rand3 = rand(gs_in[0].wposition.yz);

//Grass Vertex Arrays
const int NUM_VERTICES = 7;
vec3 grass_vertex[NUM_VERTICES];
vec4 grass_colour[NUM_VERTICES];
vec3 grass_normals[NUM_VERTICES];

//Segment lengths
float length_seg1 = 0.1 + 0.3 * rand1;
float length_seg2 = (0.2 + 0.4 * rand1) - length_seg1;
float length_seg3 = (0.3 + 0.4 * rand1) - length_seg2 - length_seg1;

//Movement of grass from grid
vec3 blade_position = vec3((0.2 * rand2) -0.1, 0, (0.2 * rand3) - 0.1); 

//Random width
float grass_width = 0.002 + 0.011 * (rand(vec2(gs_in[0].wposition.x * 2.9, gs_in[0].wposition.y * 4.988)));

//Grass random rotations
float rotation_degrees = radians(360 * rand(vec2(gs_in[0].wposition.z * 3.65, gs_in[0].wposition.y * 4.375)));

//matrix to apply random rotation to blade of grass
mat3 rotation_matrix = mat3(
	cos(rotation_degrees), 0, sin(rotation_degrees),
	0, 1, 0,
	-sin(rotation_degrees), 0, cos(rotation_degrees)
	);

/*
this matrix was used to fix my weird lighting issue, rotates normal in opposite direction to vertices.
ensures normals are correct when grass is rotated.
*/
mat3 normal_rotation_matrix = mat3(
	cos(-rotation_degrees), 0, sin(-rotation_degrees),
	0, 1, 0,
	-sin(-rotation_degrees), 0, cos(-rotation_degrees)
	);

//Grass random bend variable
float rand_bend = 0.5 * rand(vec2(gs_in[0].wposition.x * 2.766, gs_in[0].wposition.y * 1.7766)) -0.5;

/*
Grass wind value - Animation for grass using FM of sine waves using random values to control individual waves
This ugly looking variable has 5 sine functions nested within each other each one varied by a random value and takes in 
the iteration counter as a uniform, makes each vlade have a random periodic function but ensures that they all share
the same period, I off set the counter by a random value for each blade so each blade does not meet the null point at 
the same time which causes a horrible effect. Took the power of the result of the sine FM as the bend grass function is more sensitive
at lower values.
*/
int rand_offset = int(1800 * rand(vec2((gs_in[0].wposition.x*9.78555),(gs_in[0].wposition.y*6.786455))));
float wind_val = pow(sin(rand(vec2(gs_in[0].wposition.x * 2.6544, gs_in[0].wposition.y * 1.694)) * 
				(sin((rand3 * 2) * (sin((rand2 * 2) *(sin((rand1 * 2) * (sin(radians(0.2 * (wind_counter + rand_offset))))))))))), 2);

void main() 
{   
	//Non Varying Outputs
	fL = gs_in[0].fragL;
	fV = gs_in[0].fragV;

	generate_grass();
	bend_grass((0.06 * wind_val) + rand_bend);
	calculate_normals();
	draw_grass(false);
	//If two sided lighting draw the second blade
	if(two_sided) draw_grass(true);
}

//I found this function here https://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl
float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

/* 
This function generates the initial vertex positions for the blade of grass
*/
void generate_grass()
{
	fcolour = vec4(0.0, 1, 0.0, 1.0);

    grass_vertex[0] = vec3(-grass_width, 0.0, 0.0); 
	grass_colour[0] = vec4(0.05* rand3, (0.2 + 0.2 * rand3), 0.2, 1.0);

    grass_vertex[1] = vec3(grass_width, 0.0, 0.0);
	grass_colour[1] = vec4(0.05* rand3, (0.2 + 0.2 * rand3), 0.2, 1.0);
    
	grass_vertex[2] = vec3(-0.8 * grass_width, (0.1 + 0.3 * rand1), 0.0);
	grass_colour[2] = vec4(0.05* rand3, (0.25 + 0.2 * rand3), 0.1, 1.0);

	grass_vertex[3] = vec3(0.8 * grass_width, (0.1 + 0.3 * rand1), 0.0);
	grass_colour[3] = vec4(0.05* rand3, (0.25 + 0.2 * rand3), 0.1, 1.0);

	grass_vertex[4] = vec3(-0.45 * grass_width, (0.2 + 0.4 * rand1), 0.0);
	grass_colour[4] = vec4(0.06* rand3, (0.3 + 0.2 * rand3), 0.05, 1.0);

	grass_vertex[5] = vec3(0.45 * grass_width, (0.2 + 0.4 * rand1), 0.0);
	grass_colour[5] = vec4(0.06* rand3, (0.3 + 0.2 * rand3), 0.05, 1.0);

	grass_vertex[6] = vec3(0, (0.3 + 0.4 * rand1), 0.0);
	grass_colour[6] = vec4(0.075* rand3, (0.3 + 0.2 * rand3), 0.05, 1.0);
}

/*
This function bends the grass while keeping the length the same, I use this so I can animate the grass without having the grass stretch and shrink when moving
I can improve this alot, perhaps it is a waste of computations to declare original vertices and just use this as performance doesn't allow many blades of grass at the moment
*/
void bend_grass(float bend)
{
	//declare bend vectors
	vec3 bend1;
	vec3 bend2;
	vec3 bend3;

	//calculate bend vectors
	bend1 = vec3(0.0, 0.0, bend);
	bend2 = 1.5 * bend1;
	bend3 = 2 * bend1;

	//adjust vertices
	grass_vertex[2] = normalize(grass_vertex[2] + bend1) * length_seg1;
	grass_vertex[3] = normalize(grass_vertex[3] + bend1) * length_seg1;
	grass_vertex[4] = (normalize(((grass_vertex[4] + vec3(0, 0, grass_vertex[2].z)) - grass_vertex[2]) + bend2) * length_seg2) + grass_vertex[2];
	grass_vertex[5] = (normalize(((grass_vertex[5] + vec3(0, 0, grass_vertex[3].z)) - grass_vertex[3]) + bend2) * length_seg2) + grass_vertex[3];
	grass_vertex[6] = (normalize(((grass_vertex[6] + vec3(0, 0, grass_vertex[4].z)) - grass_vertex[4]) + bend3) * length_seg3) + grass_vertex[4];
}

/*
This Function calculates normals for the vertices in the blade of grass
*/
void calculate_normals()
{
	//Calculate Face Normals
	vec3 face_normals[3];
	vec3 AB, AC, cross_product;

	//First Face
	AB = grass_vertex[1] - grass_vertex[0];
	AC = grass_vertex[2] - grass_vertex[0];
	face_normals[0] = normalize(cross(AB, AC));
	//Second Face
	AB = grass_vertex[3] - grass_vertex[2];
	AC = grass_vertex[4] - grass_vertex[2];
	face_normals[1] = normalize(cross(AB, AC));
	//Third Face
	AB = grass_vertex[5] - grass_vertex[4];
	AC = grass_vertex[6] - grass_vertex[4];
	face_normals[2] = normalize(cross(AB, AC));

	//Define vertex normals
	//Vertex 0, 1
	grass_normals[0] = grass_normals[1] = face_normals[0];
	//Vertex 2, 3
	grass_normals[2] = grass_normals[3] = normalize(face_normals[0] + face_normals[1]);
	//Vertex 4, 5
	grass_normals[4] = grass_normals[5] = normalize(face_normals[1] + face_normals[2]);
	//Vertex 6
	grass_normals[6] = face_normals[2];
}

/*
This functions send the appropriate information to the fragment shader to draw the grass.
I allowed it to take in an input to reverse the normals with the thought that drawing the same 
blade of grass with reversed normals would fix the lighting from one side issue but does not seem to work.
*/
void draw_grass(bool reverse)
{
	if(!reverse)
	{
		for (int i = 0; i < NUM_VERTICES; i++)
		{
			gl_Position = gs_in[0].gprojection * gs_in[0].gview * ((vec4(grass_vertex[i] * rotation_matrix, 0) + gl_in[0].gl_Position) + vec4(blade_position, 0));
			fcolour = grass_colour[i];
			//If two sided rotate normals, else do not rotate normals
			if(two_sided) normal = gs_in[0].gnormalmatrix * normal_rotation_matrix * grass_normals[i];
			else normal = gs_in[0].gnormalmatrix * grass_normals[i];

			fR = reflect(-gs_in[0].fragL, normal);
			EmitVertex();
		}
		EndPrimitive();
	}
	//Need to reverse vertex winding order for face culling
	else
	{
		for (int i = NUM_VERTICES - 1; i >= 0; i--)
			{
				gl_Position = gs_in[0].gprojection * gs_in[0].gview * ((vec4(grass_vertex[i] * rotation_matrix, 0) + gl_in[0].gl_Position) + vec4(blade_position, 0));
				fcolour = grass_colour[i];
				//Reverse normals for back face
				normal = gs_in[0].gnormalmatrix * -(normal_rotation_matrix * grass_normals[i]);

				fR = reflect(-gs_in[0].fragL, normal);
				EmitVertex();
			}
			EndPrimitive();
	}
}

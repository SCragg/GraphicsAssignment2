/*
Skybox frag shader. Taken from https://learnopengl.com/Advanced-OpenGL/Cubemaps
*/

#version 420 core
out vec4 outputcolor;

in vec3 texturecoord;

uniform samplerCube skybox;

void main()
{    
    outputcolor = texture(skybox, texturecoord);
}
// a simple shader
#version 440

layout( location = 0 ) in vec4 Position;

uniform mat4 projection;

void main() 
{
	gl_Position = projection * Position;
}
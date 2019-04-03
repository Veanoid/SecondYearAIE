// a simple shader
#version 440

layout( location = 0 ) in vec4 Position;
layout( location = 1 ) in vec4 Normal;
layout( location = 2 ) in vec2 TexCoord;

out vec3 vNormal;
out vec4 vPosition;
out vec2 vTexCoord;

uniform mat4 projection;

// we need this matrix to transform the normal
uniform mat3 NormalMatrix;

void main() 
{
	vTexCoord = TexCoord;
	vNormal = NormalMatrix * Normal.xyz;
	gl_Position = projection * Position;
}
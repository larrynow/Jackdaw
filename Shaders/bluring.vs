#version 420 core

layout(location = 0) in vec3 aPositions;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
	gl_Position = vec4(aPositions, 1.f);
	TexCoords = aTexCoords;
}
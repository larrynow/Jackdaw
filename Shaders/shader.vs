#version 420 core
layout (location=0) in vec3 aPos;
layout (location=0) in vec4 aColor;
layout (location=2) in vec3 aNormal;
layout (location=3) in vec2 aTexCoord;

out VS_OUT
{
	vec2 TexCoord;
	vec3 Color;
};

layout (std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

uniform mat4 model;

void main()
{
	//from right to left.
	gl_Position = projection * view * model * vec4(aPos, 1.0f);

	Color = vec3(1.0f, 1.0f, 1.0f);
	TexCoord = aTexCoord;
}
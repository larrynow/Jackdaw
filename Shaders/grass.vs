#version 420 core
layout (location = 0) in vec3 grassPos;

layout (std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

void main()
{
    gl_Position = projection * view * vec4(grassPos, 1.0f);
}
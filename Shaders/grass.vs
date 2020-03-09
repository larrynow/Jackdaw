#version 420 core
layout (location = 0) in vec3 aGrassPos;

layout (std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};
out VS_OUT
{
	vec3 grassPos;
};

void main()
{
    gl_Position = projection * view * vec4(aGrassPos, 1.0f);
	grassPos = aGrassPos;
}
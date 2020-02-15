#version 420 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec4 aColor;
layout (location=2) in vec3 aNormal;
layout (location=3) in vec2 aTexCoord;

layout (std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

uniform mat4 model;

out VS_OUT
{
	flat uint texture_no;
	vec2 TexCoord;
	vec3 Color;
};

void main()
{
	//from right to left.
	gl_Position = projection * view * model * vec4(aPos, 1.0f);

	Color = vec3(1.0f, 1.0f, 1.0f);
	if(aPos.y < 10)
	{
		texture_no = 0;
	}
	else if(aPos.y < 20)
	{
		texture_no = 1;
	}
	else
	{
		texture_no = 2;
	}
	TexCoord = aTexCoord;
}
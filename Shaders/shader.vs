#version 330 core
layout (location=0) in vec3 aPos;
layout (location=0) in vec4 aColor;
layout (location=2) in vec3 aNormal;
layout (location=3) in vec2 aTexCoord;

//out vec2 TexCoord;
out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//from right to left.
	gl_Position = projection * view * model * vec4(aPos, 1.0f);

	ourColor = vec3(1.0f, 1.0f, 1.0f);
	//TexCoord = aTexCoord;
}
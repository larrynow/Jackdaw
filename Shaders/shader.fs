#version 420 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 ourColor;

uniform sampler2D texture1;
//uniform sampler2D texture1;
//uniform sampler2D texture2;

void main()
{
	FragColor = texture(texture1, TexCoord);
	//FragColor = vec4(ourColor.x, ourColor.y, ourColor.z, 1.0f);
}
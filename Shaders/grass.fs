#version 420 core
layout(location = 0)out vec4 FragColor;
layout(location = 1)out vec4 BrightColor;

uniform sampler2D texture0;

in vec2 TexCoord;

void main()
{
	FragColor = texture(texture0, TexCoord);
	if(FragColor.a < 0.1)
		discard;
	BrightColor = vec4(0.0, 0.0, 0.0, 0.0);
}
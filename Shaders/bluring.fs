#version 420 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D color;
uniform bool horizontal;

uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{
	vec2 tex_offset = 1.f/textureSize(color, 0);
	vec3 result = texture(color, TexCoords).rgb * weight[0];

	if(horizontal)
	{
		for(int i=1;i<5;i++)
		{
			result += texture(color, TexCoords + 
				vec2(tex_offset.x, 0.0)).rgb * weight[i];
			result += texture(color, TexCoords - 
				vec2(tex_offset.x, 0.0)).rgb * weight[i];
		}
	}
	else
	{
		for(int i=1;i<5;i++)
		{
			result += texture(color, TexCoords + 
				vec2(0.0, tex_offset.y)).rgb * weight[i];
			result += texture(color, TexCoords - 
				vec2(0.0, tex_offset.y)).rgb * weight[i];
		}
	}
	FragColor = vec4(result, 1.0);
}
#version 420 core
in vec2 TexCoords;

out vec4 FragColor;

uniform bool hdr;
uniform float exposure;

uniform sampler2D colorBuffer;
uniform sampler2D bloomBlur;
uniform bool bloom;

void main()
{
	const float gamma = 2.2;
	vec3 color = texture(colorBuffer, TexCoords).rgb;
	if(bloom) color += texture(bloomBlur, TexCoords).rgb;
	vec3 result;
	if(hdr)
		result = vec3(1.0) - exp(-color * exposure);//Adjust with exposure(more exposure for more dark details).
		//result = color / (color + vec3(1.0));//Adjust with Richard.
	else
		result = color;
	
	result = pow(result, vec3(1.0/gamma));

	FragColor = vec4(result, 1.0);
}
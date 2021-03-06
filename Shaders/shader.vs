#version 420 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec4 aColor;
layout (location=2) in vec3 aNormal;
layout (location=3) in vec2 aTexCoord;
layout (location=4) in vec3 aTangent;
layout (location=5) in vec3 aBitangent;

layout (std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};
layout (std140, binding = 1) uniform VP{vec3 viewPos;};

uniform mat4 model;
uniform mat4 lightSpaceMatrix;
uniform int pointLightNum;

out VS_OUT
{
	vec3 FragPos;
	vec2 TexCoord;
	vec3 Color;
    vec3 Normal;
	vec4 FragPosLightSpace;//For directional light.

    mat3 TBN;
}vs_out;

void main()
{
	//from right to left.
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoord = aTexCoord;
	vs_out.Color = vec3(1.0f, 1.0f, 1.0f);
    vs_out.Normal = mat3(model) * aNormal;

	//For shadow.
	vs_out.FragPosLightSpace = lightSpaceMatrix * model * vec4(aPos, 1.0);

	//Tangent space.
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));

	vs_out.TBN = mat3(T, B, N);
}
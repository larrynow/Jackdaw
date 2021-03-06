#version 420 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec4 aColor;
layout (location=2) in vec3 aNormal;
layout (location=3) in vec2 aTexCoord;
layout (location=4) in vec3 aTangent;
layout (location=5) in vec3 aBitangent;

//3 bones for a vertex.
layout (location = 6) in ivec3 aBoneID;
layout (location = 7) in vec3 aBoneWeight;

layout (std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};
layout (std140, binding = 1) uniform VP{vec3 viewPos;};

uniform mat4 model;
uniform mat4 lightSpaceMatrix;

#define MAX_LIGHTS 16
uniform int pointLightNum;
uniform vec3 lightDir;
uniform vec3 lightPos[MAX_LIGHTS];

#define MAX_BONES 200
uniform bool bWithSkeletalAnimation;
uniform mat4 boneMatrices[MAX_BONES];

out VS_OUT
{
	vec3 FragPos;
	vec2 TexCoord;
	vec3 Color;
    vec3 Normal;
	vec4 FragPosLightSpace;//For directional light.
	vec3 viewPos;

	vec3 TangentLightDir;
	vec3 TangentLightPos[MAX_LIGHTS];

	vec3 TangentViewPos;
	vec3 TangentFragPos;
}vs_out;

mat4 CalcBoneMatrix() {
    mat4 boneMatrix = mat4(0.0);
    for (int i = 0; i < 3; i++) {
        boneMatrix += boneMatrices[aBoneID[i]] * aBoneWeight[i];
    }
    return boneMatrix;
};

void main()
{
	vec4 pos = vec4(aPos, 1.0);
	if(bWithSkeletalAnimation)
	{
		mat4 boneMatrix = CalcBoneMatrix();
		pos = boneMatrix * vec4(aPos, 1.0);
	}
	gl_Position = projection * view * model * pos;

	vs_out.FragPos = vec3(model * pos);
	vs_out.TexCoord = aTexCoord;
	vs_out.Color = vec3(aColor);
	vs_out.Normal = aNormal;

	vs_out.FragPosLightSpace = lightSpaceMatrix * model * pos;
	vs_out.viewPos = viewPos;

	vec3 T = normalize(mat3(model)*aTangent);
	vec3 B = normalize(mat3(model)*aBitangent);
	vec3 N = normalize(mat3(model)*aNormal);
	mat3 TBN = transpose(mat3(T, B, N));

	vs_out.TangentLightDir = TBN * lightDir;//Directional light.
	for(int i=0;i<pointLightNum;++i)	
		vs_out.TangentLightPos[i] = TBN * lightPos[i];//PointLight.

	vs_out.TangentViewPos = TBN * viewPos;
	vs_out.TangentFragPos = TBN * vs_out.FragPos;
}
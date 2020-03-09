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

out VS_OUT
{
	vec2 TexCoord;
    vec3 Normal;

    mat3 TBN;
}vs_out;

uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0); 
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.Normal = normalize(vec3(projection * vec4(normalMatrix * aNormal, 0.0)));
	vs_out.TexCoord = aTexCoord;

    //Tangent space.
	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    //vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    vec3 N = vs_out.Normal;

	vs_out.TBN = mat3(T, B, N);
}
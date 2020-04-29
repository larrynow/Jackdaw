#version 420 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec4 aColor;
layout (location=2) in vec3 aNormal;
layout (location=3) in vec2 aTexCoord;
layout (location=4) in vec3 aTangent;
layout (location=5) in vec3 aBitangent;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

uniform mat4 model;
layout (std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0f);
    FragPos = worldPos.xyz; 
    gl_Position = projection * view * worldPos;
    TexCoords = aTexCoord;
    
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    Normal = normalMatrix * aNormal;
    
    //Normal = vec3(model * vec4(aNormal, 1.0));
}
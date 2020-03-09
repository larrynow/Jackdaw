#version 420 core

layout(location=0)out vec4 FragColor;
layout(location=1)out vec4 HighColor;
in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
    HighColor = vec4(vec3(0.f), 1.f);
}
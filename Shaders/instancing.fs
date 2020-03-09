#version 420 core
layout(location=0)out vec4 FragColor;
layout(location=1)out vec4 HighColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture(texture_diffuse1, TexCoords);
    HighColor = vec4(vec3(0.f), 1.f);
}
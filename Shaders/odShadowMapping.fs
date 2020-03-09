#version 420 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main()
{
    // Get distance from fragment and light pos.
    float lightDistance = length(FragPos.xyz - lightPos);
    // Map to [0;1] range by dividing by far_plane.
    lightDistance = lightDistance / farPlane;

    gl_FragDepth = lightDistance;
}
#version 420 core

in vec4 lp;

void main()
{           
    // Only use depth value.
    gl_FragDepth = gl_FragCoord.z;
}
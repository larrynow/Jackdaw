#version 420 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

uniform int bUseNormal;
uniform sampler2D normalMap;

in VS_OUT
{
	vec2 TexCoord;
    vec3 Normal;

    mat3 TBN;
}gs_in[];

const float MAGNITUDE = 0.1;

void GenerateLine(int index)
{
    vec3 normal;
    if(bUseNormal==1){
        vec3 tanNorm = texture(normalMap, gs_in[index].TexCoord).rgb;
        tanNorm = normalize(tanNorm * 2.0 - 1.0);  
        normal = normalize(gs_in[index].TBN * tanNorm);
        //normal = normalize(tanNorm);
    }//else
        //normal = gs_in[index].Normal;


    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(normal, 0.0) * MAGNITUDE;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    GenerateLine(0); // 第一个顶点法线
    GenerateLine(1); // 第二个顶点法线
    GenerateLine(2); // 第三个顶点法线
}
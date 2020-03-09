#version 420 core
layout (points) in;
layout (triangle_strip, max_vertices = 12) out;// A grass with 5 stems.

int vertexCount = 12;
float _Width = 0.5;
float _Height = 0.5;

//uniform float time;

in VS_OUT
{
	vec3 grassPos;
}vs_out[];

out GS_OUT
{
	//flat uint texture_no;
	vec2 TexCoord;
	vec3 Color;
};

float rand(vec2 co){
 return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
 //return fract(sin(time) * 43758.5453);
}

void createGrass(vec4 pos, float scale, vec2 offset)
{
	for (int i = 0; i < vertexCount; i++)
    {
		float height = (scale+0.5)*_Width;
        if (mod(i , 2) == 0)
        { 
			gl_Position = pos+ vec4(-_Width, i/2*height, 0.0, 0.0); 
            TexCoord = vec2(0.0, 1.0-(i*1.0/(vertexCount-1)));
        }
        else
        { 
			gl_Position = pos + vec4(_Width, i/2*height, 0.0, 0.0); 
			TexCoord = vec2(1.0, 1.0-((i-1)*1.0/(vertexCount-1)));
        }
	    EmitVertex();

    }
	EndPrimitive();
}

void main()
{
	vec4 pos = gl_in[0].gl_Position;
	float s = rand(vs_out[0].grassPos.xy);
	vec2 offset;
	offset.x = rand(vs_out[0].grassPos.xz)-0.5;
	offset.y = rand(vs_out[0].grassPos.yz)-0.5;

	createGrass(pos, s, offset);
}
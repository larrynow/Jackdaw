#version 420 core
layout (points) in;
layout (line_strip, max_vertices = 12) out;// A grass with 5 stems.

int vertexCount = 12;
float _Width = 0.1;
float _Height = 1.0;

out vec2 TexCoord;

void main()
{
	for (int i = 0; i < vertexCount; i++)
    {
        //v[i].norm = float3(0, 0, 1);
		
        if (mod(i , 2) == 0)
        { 
			gl_Position = gl_in[0].gl_Position + vec4(-_Width, i/2*_Height, 0.0, 0.0); 
            //TexCoord = gs_in[0].TexCoord;
        }
        else
        { 
			gl_Position = gl_in[0].gl_Position + vec4(_Width, i/2*_Height, 0.0, 0.0); 
			//TexCoord = gs_in[1].TexCoord;
        }
	    EmitVertex();

    }
	EndPrimitive();
}
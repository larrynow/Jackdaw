#version 420 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT{
	vec2 TexCoord;
	vec3 Color;
} gs_in[];

out VS_OUT{
	vec2 TexCoord;
	vec3 Color;
};

//out vec2 TexCoord;

uniform float time;

vec4 Explode(vec4 position, vec3 normal)
{
	vec3 direction = normal * ((sin(time)+1.0)/2.0);
	return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

void main()
{
	vec3 normal = GetNormal();

	gl_Position = Explode(gl_in[0].gl_Position, normal);
	TexCoord = gs_in[0].TexCoord;
	EmitVertex();
	gl_Position = Explode(gl_in[1].gl_Position, normal);
	TexCoord = gs_in[1].TexCoord;
	EmitVertex();
	gl_Position = Explode(gl_in[2].gl_Position, normal);
	TexCoord = gs_in[2].TexCoord;
	EmitVertex();
	EndPrimitive();
}
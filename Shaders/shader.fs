#version 420 core
out vec4 FragColor;

in VS_OUT{
	flat uint texture_no;
	vec2 TexCoord;
	vec3 Color;
};

//in vec2 TexCoord;
//in vec3 ourColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	vec4 texColor;
	switch(texture_no)
	{
		case 0:
			texColor = texture(texture0, TexCoord);
			break;
		case 1:
			texColor = texture(texture1, TexCoord);
			break;
		default:
			texColor = texture(texture2, TexCoord);
			break;
	}
	//if(texColor.a < 0.01)
		//discard;
	FragColor = texColor;
	//FragColor = texture(texture0, TexCoord);
	//FragColor = texture(texture1, TexCoord);
	//FragColor = texture(texture2, TexCoord);

	//FragColor = texture(texture0, TexCoord);
	//FragColor = vec4(0.7, 0.7, 0.7, 1.0f);
}
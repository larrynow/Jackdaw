#include "BackendRendererGL.h"
#include "GLShader.h"

#pragma comment (lib, "opengl32.lib")

void jkBackendRendererGL::SetClearColor(COLOR3 color)
{
	glClearColor(color.x, color.y, color.z, 1.0f);
}

void jkBackendRendererGL::Clear()
{
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void jkBackendRendererGL::StartUp()
{
	
	
}

void jkBackendRendererGL::Render(RenderData data)
{
	if (data.vertexCount == 0) return;

	glShader shader("./Shaders/shader.vs", "./Shaders/shader.fs");
	shader.use();

	shader.setMat4("view", mViewMatrix);
	shader.setMat4("projection", mProjMatrix);
	shader.setMat4("model", data.modelMatrix);

	mPrepareRenderData(data);

	////initialize number as 1.
	//unsigned int diffuseNr = 1;
	//unsigned int specularNr = 1;
	//unsigned int reflectNr = 1;

	//for (unsigned int i = 0; i < _textures.size(); i++)
	//{
	//	glActiveTexture(GL_TEXTURE0 + i);//Strat with 0 is fixed, should be set outside(render).
	//	std::string number;
	//	std::string name = _textures[i].type;
	//	if (name == "texture_diffuse")
	//		number = std::to_string(diffuseNr++);
	//	else if (name == "texture_specular")
	//		number = std::to_string(specularNr++);
	//	else if (name == "texture_reflect")
	//		number = std::to_string(reflectNr++);

	//	//CMDPRINT((name + number).c_str());
	//	shader.setFloat((/*"material." + */name + number).c_str(), i);//set Sampler uniform texture id.
	//	glBindTexture(GL_TEXTURE_2D, _textures[i].id);// bind TEXTURE with index id and currently actived texture together.
	//}
	//glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, data.vertexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void jkBackendRendererGL::mPrepareRenderData(RenderData data)
{
	//glGen for VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.vertexCount * sizeof(Vertex), data.pVertexBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.vertexCount * sizeof(unsigned int), data.pIndexBuffer, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);//Vertex coords.

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));//Vertex normals.

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

	glBindVertexArray(0);//UnBind.
}

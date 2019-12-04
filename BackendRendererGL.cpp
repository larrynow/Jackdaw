#include "BackendRendererGL.h"
#include"Camera.h"
#include"ResourceManager.h"
#include"stb_image.h"
#pragma comment (lib, "opengl32.lib")

void jkBackendRendererGL::SetClearColor(COLOR3 color)
{
	glClearColor(color.x, color.y, color.z, 1.0f);
}

void jkBackendRendererGL::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void jkBackendRendererGL::StartUp()
{
	
	
}

void jkBackendRendererGL::DrawMesh(jkMesh* mesh)
{
	if (mesh->mIndexBuffer.size() == 0)
		return;

	//glGen for VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->mVertexBuffer.size() * sizeof(Vertex), &mesh->mVertexBuffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->mIndexBuffer.size() * sizeof(UINT), &mesh->mIndexBuffer[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);//Vertex coords.
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));//Vertex normals.
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));//Vertex normals.
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);//UnBind.

	if(!m_pCurrentShader)
		m_pCurrentShader = new glShader("./Shaders/shader.vs", "./Shaders/shader.fs");

	m_pCurrentShader->use();

	mModelMatrix = mesh->GetWorldMatrx();

	m_pCurrentShader->setMat4("model", mModelMatrix);
	m_pCurrentShader->setMat4("view", mViewMatrix);
	m_pCurrentShader->setMat4("projection", mProjMatrix);

	int tex_step = 0;
	unsigned int texID = 0;
	for (auto tex : mesh->mTextures)
	{
		texID = CreateTexture(tex);
		glActiveTexture(GL_TEXTURE0+ tex_step);
		glBindTexture(GL_TEXTURE_2D, texID);
		m_pCurrentShader->setInt((std::string("texture")+std::to_string(tex_step)).c_str(), tex_step++);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, mesh->mIndexBuffer.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteTextures(1, &texID);
}

void jkBackendRendererGL::DrawSkyBox(jkMesh* skyBoxMesh)
{
	//auto cubeMap = CreateTexture(skyBoxMesh);
}

UINT jkBackendRendererGL::CreateTexture(Texture* pTexture)
{
	UINT textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLenum format;
	if (pTexture->channels == 1)
		format = GL_RED;
	else if (pTexture->channels == 3)
		format = GL_RGB;
	else if (pTexture->channels == 4)
		format = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, format, pTexture->width, pTexture->height, 0, format, 
		GL_UNSIGNED_BYTE, pTexture->pImageData);

	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
}




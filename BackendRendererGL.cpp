#include "BackendRendererGL.h"
#include "GLShader.h"
#include"Camera.h"
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

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);//Vertex coords.

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));//Vertex normals.

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));//Vertex normals.

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

	glBindVertexArray(0);//UnBind.

	glShader shader("./Shaders/shader.vs", "./Shaders/shader.fs");
	shader.use();

	mModelMatrix = mesh->GetWorldMatrx();

	shader.setMat4("model", mModelMatrix);
	shader.setMat4("view", mViewMatrix);
	shader.setMat4("projection", mProjMatrix);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, mesh->mIndexBuffer.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


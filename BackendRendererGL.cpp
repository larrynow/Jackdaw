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

void jkBackendRendererGL::LoadMesh(jkMesh* mesh)
{
	if (mesh->mIndexBuffer.size()==0) return;

	GLRenderData* pRenderData = new GLRenderData();
	pRenderData->pOriginMesh = mesh;

	// TODO : Depends on mesh type, choose a shader. For one type only ceate shader once.
	pRenderData->pShader = new glShader("./Shaders/shader.vs", "./Shaders/shader.fs");

	glGenVertexArrays(1, &pRenderData->VAO);
	glGenBuffers(1, &pRenderData->VBO);
	glGenBuffers(1, &pRenderData->EBO);

	glBindVertexArray(pRenderData->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, pRenderData->VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->mVertexBuffer.size() * sizeof(Vertex), &mesh->mVertexBuffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pRenderData->EBO);
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

	int tex_step = 0;
	unsigned int texID = 0;
	for (auto tex : mesh->mTextures)
	{
		texID = mCreateTexture(tex);// Warning! : TexID is hung.
		glActiveTexture(GL_TEXTURE0 + tex_step);
		glBindTexture(GL_TEXTURE_2D, texID);
		pRenderData->pShader->setInt((std::string("texture") + std::to_string(tex_step)).c_str(), tex_step++);
	}

	mRenderDatas.push_back(pRenderData);
}

void jkBackendRendererGL::StartRender()
{
	int i = 0;
	while (i < mRenderDatas.size())
	//for (auto it = mRenderDatas.begin(); it!=mRenderDatas.end();++it)
	{
		GLRenderData* p_glRenderData = static_cast<GLRenderData*>(mRenderDatas.at(i));

		if (!p_glRenderData->pOriginMesh)// Origin mesh is deleted already.
		{
			delete p_glRenderData;
			mRenderDatas.at(i) = nullptr;

			// TODO : If mesh is sort with z values(from near to fa), the swap will destroy the order.
			std::swap(mRenderDatas.at(i), mRenderDatas.at(mRenderDatas.size()-1));
			mRenderDatas.pop_back();
		}
		else
		{
			if (p_glRenderData->pOriginMesh->m_bRenderable)
			{
				mRender(p_glRenderData);
			}
		}
		i++;
	}
}

void jkBackendRendererGL::DrawSkyBox(jkMesh* skyBoxMesh)
{
	//auto cubeMap = CreateTexture(skyBoxMesh);
}

UINT jkBackendRendererGL::mCreateTexture(Texture* pTexture)
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

void jkBackendRendererGL::mRender(GLRenderData* pData)
{
	pData->pShader->use();

	mModelMatrix = pData->pOriginMesh->GetWorldMatrx();

	pData->pShader->setMat4("model", mModelMatrix);
	pData->pShader->setMat4("view", mViewMatrix);
	pData->pShader->setMat4("projection", mProjMatrix);

	glBindVertexArray(pData->VAO);
	glDrawElements(GL_TRIANGLES, pData->pOriginMesh->mIndexBuffer.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}




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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void jkBackendRendererGL::StartUp()
{
	
	
}

RenderData* jkBackendRendererGL::mProcessMesh(jkMesh* mesh)
{
	if (mesh->mIndexBuffer.size()==0) return nullptr;

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

	glBindVertexArray(0);

	int tex_step = 0;
	unsigned int texID = 0;
	for (auto tex : mesh->mTextures)
	{
		texID = mCreateTexture(tex);// Warning! : TexID is hung.
		glActiveTexture(GL_TEXTURE0 + tex_step);
		glBindTexture(GL_TEXTURE_2D, texID);
		pRenderData->pShader->setInt((std::string("texture") + std::to_string(tex_step)).c_str(), tex_step++);
	}

	return pRenderData;
}

void jkBackendRendererGL::StartRender()
{
	glEnable(GL_DEPTH_TEST);

	UINT i = 0;
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

	if (mSkybox)
	{
		RenderSkybox();
	}
}

void jkBackendRendererGL::RenderSkybox()
{
	auto skybox = static_cast<GLCubeMapData*>(mSkybox);

	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skybox->pShader->use();

	skybox->pShader->setMat4("view", RemoveTranslation(mViewMatrix));
	skybox->pShader->setMat4("projection", mProjMatrix);

	glBindVertexArray(skybox->VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->TEXTURES.at(0));
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

CubeMapData* jkBackendRendererGL::mProcessCubeMap(std::vector<unsigned char*>& faces, 
	const ImageFormat& textureFormat)
{
	GLCubeMapData* glCmData = new GLCubeMapData();

	////////////////////////////////////////////////
	// Shader.

	glCmData->pShader = new glShader("./Shaders/skybox.vs", "./Shaders/skybox.fs");

	////////////////////////////////////////////////
	// Datas.

	float* cubeMapVertices = jkGeometry::CreateCubeVertices();
	unsigned int cubeMapVAO, cubeMapVBO;
	glGenVertexArrays(1, &cubeMapVAO);
	glGenBuffers(1, &cubeMapVBO);
	glBindVertexArray(cubeMapVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeMapVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*108, cubeMapVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	glCmData->VBO = cubeMapVBO;
	glCmData->VAO = cubeMapVAO;

	//delete cubeMapVertices;

	///////////////////////////////////////////////
	// Texture.

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	GLenum format;
	if (textureFormat.channels == 1)
		format = GL_RED;
	else if (textureFormat.channels == 3)
		format = GL_RGB;
	else if (textureFormat.channels == 4)
		format = GL_RGBA;

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, textureFormat.width, textureFormat.height, 0, format, GL_UNSIGNED_BYTE, faces.at(i));
		stbi_image_free(faces.at(i));
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glCmData->TEXTURES.push_back(textureID);

	glCmData->pShader->use();
	glCmData->pShader->setInt("skybox", 0);

	return glCmData;
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
	if (pTexture->TextureFormat.channels == 1)
		format = GL_RED;
	else if (pTexture->TextureFormat.channels == 3)
		format = GL_RGB;
	else if (pTexture->TextureFormat.channels == 4)
		format = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, format, pTexture->TextureFormat.width, pTexture->TextureFormat.height, 0, format,
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




#include "Jackdaw.h"
#include "BackendRendererGL.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "Light.h"
#include "stb_image.h"
//#pragma comment (lib, "opengl32.lib")

const UINT SHADOW_WIDTH = 1024;
const UINT SHADOW_HEIGHT = 1024;

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
	/////////////////////////////////
	// Uniform buffer obejects.

	// Initialize matrices ubo.
	glGenBuffers(1, &mMatrixUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, mMatrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(MAT4), NULL, GL_STATIC_DRAW);// Only reserve memory.
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, mMatrixUBO, 0, 2 * sizeof(MAT4));// Bind uboMatrices with binding point 0.

	// Initialize viewPos ubo.
	glGenBuffers(1, &mViewPosUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, mViewPosUBO);
	glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(float), NULL, GL_STATIC_DRAW);// Only reserve memory.
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, mViewPosUBO, 0, 4 * sizeof(float));// Bind uboMatrices with binding point 0.

	// Light pos ubo.
	glGenBuffers(1, &mLightPosUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, mLightPosUBO);
	glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(float) * (1 + MAXLIGHTSNUM + 1), NULL, GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, mLightPosUBO);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//////////////////////////
	//Gamma correction.

	//glEnable(GL_FRAMEBUFFER_SRGB);

	/////////////////////////////
	//Depth mapping.

	glGenFramebuffers(1, &mShadowFBO);

	///////////////////////////////////////////////
	// Depth mapping for directional light.

	//A depth component.
	glGenTextures(1, &mDepthMap);
	glBindTexture(GL_TEXTURE_2D, mDepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT,
		GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	//Set border color. Out of border will be border color.
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, mShadowFBO);

	glDrawBuffer(GL_NONE);//Do not render.
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	mDepthShader = new glShader("./Shaders/shadowMapping.vs", "./Shaders/shadowMapping.fs");

	//////////////////////////////////////////////////
	// Omnidirectional shadow mapping for pointLight.

	glGenTextures(1, &mDepthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mDepthCubemap);
	for (size_t i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);//Out of border will be edge value.

	glBindFramebuffer(GL_FRAMEBUFFER, mShadowFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		mDepthCubemap, 0);
	glDrawBuffer(GL_NONE);//Do not render.
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	mOMDepthShader = new glShader("./Shaders/odShadowMapping.vs", 
		"./Shaders/odShadowMapping.fs", "./Shaders/odShadowMapping.gs");

	mNormalShader = new glShader("./Shaders/normal.vs",
		"./Shaders/normal.fs", "./Shaders/normal.gs");

	/////////////////////////////////////
	// Create HDR frameBuffer for HDR rendering.
	
	glGenFramebuffers(1, &mHDRFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mHDRFBO);

	glGenTextures(2, mFloatColorBuffers);//Two float color buffers, one for color, one for high color.
	for (size_t i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, mFloatColorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F,
			jkContent::GetInstancePtr()->mWidth, jkContent::GetInstancePtr()->mHeight,
			0, GL_RGB, GL_FLOAT, NULL);//16 bits float color buffer(default as 8 bits).
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i,
			GL_TEXTURE_2D, mFloatColorBuffers[i], 0);//Bind color buffer.
	}
	GLenum color_attaches[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, color_attaches);

	UINT rboDepth;//Render buffer(Write-Only, better).
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
		jkContent::GetInstancePtr()->mWidth, jkContent::GetInstancePtr()->mHeight);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, rboDepth);//BInd render buffer.

	////////////////////////////////////////////////////////////
	// Create auxiliary frame buffers, each with a color buffer.

	glGenFramebuffers(2, mAuxFBOs);
	glGenTextures(2, mAuxColorBuffers);

	for (size_t i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mAuxFBOs[i]);
		glBindTexture(GL_TEXTURE_2D, mAuxColorBuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F,
			jkContent::GetInstancePtr()->mWidth, jkContent::GetInstancePtr()->mHeight,
			0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, mAuxColorBuffers[i], 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	mPostRenderingShader = new glShader("./Shaders/postRendering.vs", "./Shaders/postRendering.fs");

	mBluringShader = new glShader("./Shaders/bluring.vs", "./Shaders/bluring.fs");

	///////////////////////////////////////////////
	// Create squad VAO and VBO for post-rendering.

	glGenVertexArrays(1, &mQuadVAO);
	glGenBuffers(1, &mQuadVBO);

	float quadVertices[] = {
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,// Color and texture coords.
		-1.0f,-1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 0.0f, 1.0f, 0.0f
	};

	glBindVertexArray(mQuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices,
		GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

RenderData* jkBackendRendererGL::mProcessMesh(jkMesh* mesh)
{
	//if(!mesh->m_pShader) 
	GLRenderData* pRenderData = new GLRenderData();
	pRenderData->pOriginMesh = mesh;

	if(mesh->m_pShader)
		pRenderData->pShader = mesh->m_pShader;
	else
		pRenderData->pShader = jkResourceManager::GetShader("heightMapping");

	///////////////////////////////
	// For VAO, VBO.

	if (mesh->mIndexBuffer.size()==0) return nullptr;

	glGenVertexArrays(1, &pRenderData->VAO);
	glGenBuffers(1, &pRenderData->VBO);
	glGenBuffers(1, &pRenderData->EBO);

	glBindVertexArray(pRenderData->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, pRenderData->VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->mVertexBuffer.size() * sizeof(Vertex), &mesh->mVertexBuffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pRenderData->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->mIndexBuffer.size() * sizeof(UINT), &mesh->mIndexBuffer[0], GL_STATIC_DRAW);

	pRenderData->IndexSize = mesh->mIndexBuffer.size();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);//Vertex coords.
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));//Vertex color.
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));//Vertex normals.
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	glEnableVertexAttribArray(5);

	glBindVertexArray(0);

	////////////////////////////////////
	// Create textures on GPU.

	unsigned int texID = 0;

	if (mesh->mMaterial.diffuseMap)
	{
		texID = mCreateTexture(mesh->mMaterial.diffuseMap);
		pRenderData->difffuseTex = texID;

		if (mesh->mMaterial.specularMap)
		{
			texID = mCreateTexture(mesh->mMaterial.specularMap);
			pRenderData->specularTex = texID;
		}
		else//Use diffuse as specular.
		{
			texID = mCreateTexture(mesh->mMaterial.diffuseMap);
			pRenderData->specularTex = texID;
		}
	}

	if (mesh->mMaterial.normalMap)
	{
		texID = mCreateTexture(mesh->mMaterial.normalMap);
		pRenderData->normalTex = texID;
	}

	if (mesh->mMaterial.heightMap)
	{
		pRenderData->heightTex = mCreateTexture(mesh->mMaterial.heightMap);
	}

	////////////////////////////
	// Property.

	pRenderData->shininess = mesh->mMaterial.shininess;

	return pRenderData;
}

InstanceRenderData* jkBackendRendererGL::mProcessInstanceData(jkMesh* instanceMesh, std::vector<MAT4>& modelMatrices)
{
	if (instanceMesh->mIndexBuffer.size() == 0) return nullptr;

	GLInstanceRenderData* pInstanceRenderData = new GLInstanceRenderData();
	pInstanceRenderData->pOriginMesh = instanceMesh;
	pInstanceRenderData->pModelMatrices = modelMatrices;
	pInstanceRenderData->instanceNum = instanceMesh->mIndexBuffer.size();

	// TODO : Depends on mesh type, choose a shader. For one type only ceate shader once.
	pInstanceRenderData->pShader = new glShader("./Shaders/instancing.vs", "./Shaders/instancing.fs");

	glGenVertexArrays(1, &pInstanceRenderData->VAO);
	glGenBuffers(1, &pInstanceRenderData->VBO);
	glGenBuffers(1, &pInstanceRenderData->EBO);

	glBindVertexArray(pInstanceRenderData->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, pInstanceRenderData->VBO);
	glBufferData(GL_ARRAY_BUFFER, instanceMesh->mVertexBuffer.size() * sizeof(Vertex), &instanceMesh->mVertexBuffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pInstanceRenderData->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, instanceMesh->mIndexBuffer.size() * sizeof(UINT), &instanceMesh->mIndexBuffer[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);//Vertex coords.
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));//Vertex normals.
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));//Vertex texcoord.
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &pInstanceRenderData->InstanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pInstanceRenderData->InstanceVBO);
	glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(MAT4), &modelMatrices[0], GL_STATIC_DRAW);

	//Matrices.
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(VEC4), (void*)0);//  A data size equals vec4 maximium.
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(VEC4), (void*)(sizeof(VEC4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(VEC4), (void*)(2 * sizeof(VEC4)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(VEC4), (void*)(3 * sizeof(VEC4)));

	// Update model matrix for evey data point.
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

	unsigned int texID = 0;
	int shderTextureID = 1;
	//for (auto tex : instanceMesh->mTextures)
	//{
	//	texID = mCreateTexture(tex);// Warning! : TexID is hung.
	//	pInstanceRenderData->TexID.push_back(texID);
	//}

	texID = mCreateTexture(instanceMesh->mMaterial.diffuseMap);// Warning! : TexID is hung.
	pInstanceRenderData->difffuseTex = texID;

	return pInstanceRenderData;
}

void jkBackendRendererGL::StartRender()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	//Shadow mapping.

	if(mDirLight) mGetDepthMap();
	
	if (mPointLights.size())
		mGetDepthCubemap();//?

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, jkContent::GetInstance().mWidth, jkContent::GetInstance().mHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mUpdateViewPos();
	//mUpdateLightPos();
	mUpdateViewMatrix();
	mUpdateProjMatrix();

	UINT color_buffer = 0;//Color buffer for post rendering.

	//HDR frame buffer.
	bool HDR_rendering = true;
	if (HDR_rendering)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mHDRFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		color_buffer = mFloatColorBuffers[0];
	}//After HDR rendering got a color buffer and a high light cb.

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
				//mRenderHDR(p_glRenderData);
				mRender(p_glRenderData);
				//mRenderNormal(p_glRenderData);
			}
		}
		i++;
	}

	for (auto instance : mInstanceRenderDatas)
	{
		GLInstanceRenderData* p_glInsRenderData = static_cast<GLInstanceRenderData*>(instance);
		if (p_glInsRenderData->pOriginMesh->m_bRenderable)
		{
			mRenderInstance(p_glInsRenderData);
		}
	}

	for (auto surrounding : mSurroundingRenderDatas)
	{
		GLSurroundingRenderData* p_glSurroundingData = static_cast<GLSurroundingRenderData*>(surrounding);
		mRenderSurrounding(p_glSurroundingData);
	}

	if (mSkybox)
	{
		RenderSkybox();
	}

	mPostRendering(color_buffer);
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
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->difffuseTex);
	skybox->pShader->setInt("skybox", 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void jkBackendRendererGL::ChangeDataIndices(jkMesh* p_mesh, std::vector<UINT>& indices)
{
	GLRenderData* data = (GLRenderData*)mRenderDataMap.at(p_mesh);
	glBindVertexArray(data->VAO);

	data->IndexSize = indices.size();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *sizeof(UINT), &indices[0], GL_STATIC_DRAW);
	
	/*for (size_t i = 0; i < 30; i++)
	{
		std::cout << indices.at(i)<<" ";

	}
	std::cout << std::endl;*/
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_mesh->mIndexBuffer.size()/4*sizeof(UINT), &p_mesh->mIndexBuffer[0], GL_DYNAMIC_DRAW);

	glBindVertexArray(0);

}

void jkBackendRendererGL::ChangeInstances(jkMesh* p_ins_mesh, std::vector<MAT4>& model_matrices)
{
	GLInstanceRenderData* data = (GLInstanceRenderData*)mInstanceRenderDataMap.at(p_ins_mesh);
	if (!data) return;
	data->instanceNum = model_matrices.size();
	glBindVertexArray(data->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, data->InstanceVBO);
	glBufferData(GL_ARRAY_BUFFER, model_matrices.size() * sizeof(MAT4), &model_matrices[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void jkBackendRendererGL::ChangeSurrounding(std::vector<VEC3>& positions)
{
	auto glSRData = (GLSurroundingRenderData*)mSurroundingRenderDatas.at(0);
	glSRData->count = positions.size();
	glBindVertexArray(glSRData->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, glSRData->VBO);
	// Pass grass positions.
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(VEC3), &positions[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void jkBackendRendererGL::mPostRendering(UINT color_buffer)
{
	// Render color buffer into Default framebuffer.

	mPostRenderingShader->use();

	// Bloom effect.
	float bloom = true;
	if (bloom)
	{
		// Blur high light color.
		UINT bloom_blur_cbo = mBlurRendering(mFloatColorBuffers[1]);
		mPostRenderingShader->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bloom_blur_cbo);
		mPostRenderingShader->setInt("bloomBlur", 0);
		mPostRenderingShader->setBool("bloom", true);
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, color_buffer);
	mPostRenderingShader->setInt("colorBuffer", 1);

	mPostRenderingShader->setBool("hdr", true);
	mPostRenderingShader->setFloat("exposure", 1.f);//TODO : set exposure.

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mRenderQuad();
}

UINT jkBackendRendererGL::mBlurRendering(UINT color_buffer)
{
	// Back-forward bluring a color buffer with two aux fbo.
	// Return the final color buffer.
	mBluringShader->use();

	UINT times = 20;
	bool horizontal = true;
	//Back-foward.
	for (size_t i = 0; i < times; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mAuxFBOs[i % 2]);
		mBluringShader->setBool("horizontal", horizontal);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,
			i == 0 ? color_buffer ://First from high color to aux[0].
			mAuxColorBuffers[(i+1) % 2]);//Then from aux[0] to aux [1].
		mBluringShader->setInt("color", 0);
		mRenderQuad();
		horizontal = !horizontal;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return mAuxColorBuffers[1];
}

void jkBackendRendererGL::mRenderQuad()
{
	// Render color buffer from frame buffer into a quad.
	glBindVertexArray(mQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void jkBackendRendererGL::mRenderSurrounding(GLSurroundingRenderData* surroundingData)
{
	mModelMatrix = MAT4();

	//////////////////////////////
	// Render.

	surroundingData->pShader->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, surroundingData->TexID.at(0));
	surroundingData->pShader->setInt("texture0", 0);
	
	glBindVertexArray(surroundingData->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, surroundingData->VBO);
	glDrawArrays(GL_POINTS, 0, surroundingData->count);
	glBindVertexArray(0);

	//int shaderTexID = 0;
	//unsigned int texID = 0;
	//Texture* tex;

	//texID = mCreateTexture(tex);// Warning! : TexID is hung.
	//pGrassData->TEXTURES.push_back(texID);
	
}

void jkBackendRendererGL::mGetDepthMap()
{
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, mDepthMap, 0);

	glClear(GL_DEPTH_BUFFER_BIT);

	MAT4 lightProjection, lightView;
	MAT4 lightSpaceMatrix;
	float near_plane = 0.1f, far_plane = 50.f;
	float lightSapceSize = 10.f;
	jkMath::MakeOrthMatrix(lightProjection, 
		-lightSapceSize, lightSapceSize, -lightSapceSize, lightSapceSize, near_plane, far_plane);
	VEC3 eyePosition = VEC3(0.f, 0.f, 0.f) - mDirLight->Direction;
	jkMath::MakeLookAtMatrix(lightView, eyePosition, mDirLight->Direction, {0.f, 1.f, 0.f});
	lightSpaceMatrix = lightProjection * lightView;
	mLightSpaceMatrix = lightSpaceMatrix;

	for (auto data :  mRenderDatas)
	{
		if (!data) continue;
		// Calc scene depth from light's point of view.
		mDepthShader->use();
		mDepthShader->setMat4("lightSpaceMatrix", mLightSpaceMatrix);

		mCalcDepth(data, mDepthShader);
	}

	for (auto instance : mInstanceRenderDatas)
	{
		//mCalcDepth(instance, depthShader);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void jkBackendRendererGL::mGetDepthCubemap()
{
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, mShadowFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, 
		GL_DEPTH_ATTACHMENT, mDepthCubemap, 0);//TODO : shadowmap for every point light.
	glClear(GL_DEPTH_BUFFER_BIT);

	float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
	float near = 0.1f;
	float far = 15.0f;
	MAT4 lightProjMatrix;
	MakePerspectiveMatrix_GL(lightProjMatrix, PI / 4, aspect, near, far);
	
	auto lightPos = mPointLights.at(0)->Position;
	std::vector<MAT4> shadowTransforms;
	shadowTransforms.push_back(lightProjMatrix *
		GetLookAtMatrix(lightPos, VEC3(1.0, 0.0, 0.0), VEC3(0.0, -1.0, 0.0)));//Flip.
	shadowTransforms.push_back(lightProjMatrix *
		GetLookAtMatrix(lightPos, VEC3(-1.0, 0.0, 0.0), VEC3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(lightProjMatrix *
		GetLookAtMatrix(lightPos, VEC3(0.0, 1.0, 0.0), VEC3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(lightProjMatrix *
		GetLookAtMatrix(lightPos, VEC3(0.0, -1.0, 0.0), VEC3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(lightProjMatrix *
		GetLookAtMatrix(lightPos, VEC3(0.0, 0.0, 1.0), VEC3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(lightProjMatrix *
		GetLookAtMatrix(lightPos, VEC3(0.0, 0.0, -1.0), VEC3(0.0, -1.0, 0.0)));

	mOMDepthShader->use();
	mOMDepthShader->setVec3("lightPos", lightPos);
	mOMDepthShader->setFloat("farPlane", far);

	for (size_t i = 0; i < shadowTransforms.size(); i++)
	{
		mOMDepthShader->setMat4(std::string("shadowMatrices[")+(char)('0'+i)+"]", shadowTransforms.at(i));
	}

	glCullFace(GL_FRONT);

	for (auto data : mRenderDatas)
	{
		mCalcDepth(data, mOMDepthShader);
	}

	glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void jkBackendRendererGL::mCalcDepth(RenderData* pData, glShader* depthShader)
{
	depthShader->use();

	//Set model matrix.
	mModelMatrix = pData->pOriginMesh->GetWorldMatrx();
	depthShader->setMat4("model", mModelMatrix);

	//Set far plane for omnidirection shadowing.
	//depthShader->setFloat("farPlane", 25.0f);

	glBindVertexArray(((GLRenderData*)pData)->VAO);
	glDrawElements(GL_TRIANGLES, pData->pOriginMesh->mIndexBuffer.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void jkBackendRendererGL::mCalcDepth(InstanceRenderData* pData, glShader* depthShader)
{
	depthShader->use();
	glBindVertexArray(((GLInstanceRenderData*) pData)->VAO);

	glDrawElementsInstanced(
		GL_TRIANGLES, ((GLInstanceRenderData*)pData)->pOriginMesh->mIndexBuffer.size(), GL_UNSIGNED_INT, 
		0, ((GLInstanceRenderData*)pData)->pModelMatrices.size()
	);
}

void jkBackendRendererGL::mRenderNormal(GLRenderData* pData)
{
	mNormalShader->use();
	if (pData->normalTex)
	{
		mNormalShader->setInt("bUseNormal", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pData->normalTex);
		mNormalShader->setInt("normalMap", 1);
	}
	else
	{
		mNormalShader->setInt("bUseNormal", 0);
	}
	
	// Set model matrix.
	mModelMatrix = pData->pOriginMesh->GetWorldMatrx();
	mNormalShader->setMat4("model", mModelMatrix);

	glBindVertexArray(pData->VAO);
	glDrawElements(GL_TRIANGLES, pData->pOriginMesh->mIndexBuffer.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
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
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, 
			textureFormat.width, textureFormat.height, 0, format, GL_UNSIGNED_BYTE, faces.at(i));
		stbi_image_free(faces.at(i));
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//glCmData->TexID.push_back(textureID);
	glCmData->difffuseTex = textureID;

	/*glCmData->pShader->use();
	glCmData->pShader->setInt("skybox", 0);*/

	return glCmData;
}

SurroundingRenderData* jkBackendRendererGL::mProcessSurroundingData(
	std::vector<VEC3>& positions, Texture* tex)
{
	GLSurroundingRenderData* glSRData = new GLSurroundingRenderData();

	glSRData->pShader = new glShader("./Shaders/grass.vs", "./Shaders/grass.fs",
		"./Shaders/grass.gs");


	glSRData->TexID.push_back(mCreateTexture(tex));
	

	glSRData->count = positions.size();
	glGenVertexArrays(1, &glSRData->VAO);
	glGenBuffers(1, &glSRData->VBO);

	glBindVertexArray(glSRData->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, glSRData->VBO);

	// Pass grass positions.
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(VEC3), &positions[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VEC3), (void*)0);//Vertex coords.
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	return glSRData;
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

	GLenum taregetFormat;
	if (pTexture->TextureFormat.channels == 1)
		taregetFormat = GL_RED;
	else if (pTexture->TextureFormat.channels == 3)
		taregetFormat = GL_RGB;
	else if (pTexture->TextureFormat.channels == 4)
		taregetFormat = GL_RGBA;

	GLenum sourceFormat;
	if (pTexture->isSRGB)
	{
		if (pTexture->TextureFormat.channels == 1)
			sourceFormat = GL_RED;
		else if (pTexture->TextureFormat.channels == 3)
			sourceFormat = GL_SRGB;
		else if (pTexture->TextureFormat.channels == 4)
			sourceFormat = GL_SRGB_ALPHA;
	}
	else
	{
		sourceFormat = taregetFormat;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, sourceFormat, pTexture->TextureFormat.width, pTexture->TextureFormat.height, 0, taregetFormat,
		GL_UNSIGNED_BYTE, pTexture->pImageData);

	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
}

void jkBackendRendererGL::mRender(GLRenderData* pData)
{
	pData->pShader->use();

	// Set model matrix.
	mModelMatrix = pData->pOriginMesh->GetWorldMatrx();
	pData->pShader->setMat4("model", mModelMatrix);

	//pData->pShader->setVec3("viewPos", mViewPos);

	if (pData->pOriginMesh->m_bLighting)//Enable lighting or not.
	{
		pData->pShader->setBool("lighting", true);
		mEnableLighting(pData);
	}
	else
	{
		pData->pShader->setBool("lighting", false);
	}

	if (pData->pOriginMesh->m_bShinning)//Enable blooming or not.
	{
		pData->pShader->setBool("shinning", true);
	}
	else
	{
		pData->pShader->setBool("shinning", false);
	}
	//for (size_t i = 0; i < pData->TexID.size(); i++)
	//{
	//	glActiveTexture(GL_TEXTURE0 + i);
	//	glBindTexture(GL_TEXTURE_2D, pData->TexID.at(i));
	//	auto tex_name = std::string("texture") + (char)('0'+i);
	//	pData->pShader->setInt(tex_name, i);
	//}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mDepthMap);
	pData->pShader->setInt("shadowMap", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mDepthCubemap);
	pData->pShader->setInt("depthCubemap", 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, pData->difffuseTex);
	pData->pShader->setInt("material.diffuseMap", 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, pData->specularTex);
	pData->pShader->setInt("material.specularMap", 3);
	
	if (pData->normalTex)
	{
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, pData->normalTex);
		pData->pShader->setInt("material.normalMap", 4);
		pData->pShader->setInt("bUseNormalMap", 1);
	}
	else
	{
		pData->pShader->setInt("bUseNormalMap", 0);
	}

	if (pData->heightTex)
	{
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, pData->heightTex);
		pData->pShader->setInt("material.heightMap", 5);
		pData->pShader->setInt("bUseHeightMap", 1);
	}
	else
	{
		pData->pShader->setInt("bUseHeightMap", 0);
	}

	pData->pShader->setMat4("lightSpaceMatrix", mLightSpaceMatrix);

	pData->pShader->setFloat("material.shininess", (float)pData->shininess);

	pData->pShader->setFloat("time", pTimer->GetTime());

	glBindVertexArray(pData->VAO);
	glDrawElements(GL_TRIANGLES, pData->IndexSize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void jkBackendRendererGL::mEnableLighting(GLRenderData* pData)
{
	// Set Light.

	int lightCount = 0;
	if (mDirLight)
	{
		pData->pShader->setVec3(std::string("lightDir"), mDirLight->Direction);

		pData->pShader->setVec3("dirLight.direction", mDirLight->Direction);
		pData->pShader->setVec3("dirLight.color.ambient", mDirLight->AmbientColor);
		pData->pShader->setVec3("dirLight.color.diffuse", mDirLight->DiffuseColor);
		pData->pShader->setVec3("dirLight.color.specular", mDirLight->SpecularColor);
	}
	for (size_t i = 0; i < mPointLights.size(); i++)
	{
		auto pointLight = mPointLights.at(i);

		pData->pShader->setVec3(std::string("lightPos[") + (char)('0' + lightCount++) + "]", pointLight->Position);

		pData->pShader->setVec3(std::string("pointLights[") + (char)('0' + i) + "].position", pointLight->Position);
		pData->pShader->setVec3(std::string("pointLights[") + (char)('0' + i) + "].color.ambient", pointLight->AmbientColor);
		pData->pShader->setVec3(std::string("pointLights[") + (char)('0' + i) + "].color.diffuse", pointLight->DiffuseColor);
		pData->pShader->setVec3(std::string("pointLights[") + (char)('0' + i) + "].color.specular", pointLight->SpecularColor);
		pData->pShader->setFloat(std::string("pointLights[") + (char)('0' + i) + "].constant", pointLight->Constant);
		pData->pShader->setFloat(std::string("pointLights[") + (char)('0' + i) + "].linear", pointLight->Linear);
		pData->pShader->setFloat(std::string("pointLights[") + (char)('0' + i) + "].quadratic", pointLight->Quadratic);
		pData->pShader->setFloat(std::string("pointLights[") + (char)('0' + i) + "].bound", 15.f);
	}
	pData->pShader->setInt("pointLightNum", mPointLights.size());

	if (mSpotLight)
	{
		pData->pShader->setVec3(std::string("lightPos[") + (char)('0' + lightCount++) + "]", mSpotLight->Position);

		pData->pShader->setVec3("spotLight.position", mSpotLight->Position);
		pData->pShader->setVec3("spotLight.direction", mSpotLight->Direction);
		pData->pShader->setVec3("spotLight.ambient", mSpotLight->AmbientColor);
		pData->pShader->setVec3("spotLight.diffuse", mSpotLight->DiffuseColor);
		pData->pShader->setVec3("spotLight.specular", mSpotLight->SpecularColor);
		pData->pShader->setFloat("spotLight.constant", mSpotLight->Constant);
		pData->pShader->setFloat("spotLight.linear", mSpotLight->Linear);
		pData->pShader->setFloat("spotLight.quadratic", mSpotLight->Quadratic);
		pData->pShader->setFloat("spotLight.cutOff", cos(mSpotLight->InnerCutOffRadian));
		pData->pShader->setFloat("spotLight.outerCutOff", cos(mSpotLight->OutterCutOffRadian));
	}
	pData->pShader->setInt("lightNum", lightCount);
}

void jkBackendRendererGL::mRenderInstance(GLInstanceRenderData* instanceData)
{
	instanceData->pShader->use();
	glBindVertexArray(instanceData->VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, instanceData->difffuseTex);
	instanceData->pShader->setInt("texture_diffuse1", 0);
	glDrawElementsInstanced(
		GL_TRIANGLES, instanceData->pOriginMesh->mIndexBuffer.size(), GL_UNSIGNED_INT, 0, instanceData->instanceNum
	);
}

void jkBackendRendererGL::mCopyBufferData(UINT vbo_from, UINT vbo_target, UINT dataSize)
{
	// Copy data : from to target.
	glBindBuffer(GL_COPY_READ_BUFFER, vbo_from);
	glBindBuffer(GL_COPY_WRITE_BUFFER, vbo_target);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, dataSize);
}

void jkBackendRendererGL::mUpdateViewPos()
{
	glBindBuffer(GL_UNIFORM_BUFFER, mViewPosUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 3*sizeof(float), &mViewPos);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void jkBackendRendererGL::mUpdateLightPos()
{
	glBindBuffer(GL_UNIFORM_BUFFER, mLightPosUBO);
	if(mDirLight)
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 4*sizeof(float), &(mDirLight->Direction));
	for (size_t i = 0; i < mPointLights.size(); i++)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float)*(i+1), 4 * sizeof(float) * (i + 2),
			&(mPointLights.at(i)->Position));
	}
	glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float) * (MAXLIGHTSNUM+1), 
		4 * sizeof(float) * (MAXLIGHTSNUM + 2), &(mSpotLight->Position));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void jkBackendRendererGL::mUpdateViewMatrix()
{
	glBindBuffer(GL_UNIFORM_BUFFER, mMatrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(MAT4), sizeof(MAT4), &(mViewMatrix.T()));

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void jkBackendRendererGL::mUpdateProjMatrix()
{
	glBindBuffer(GL_UNIFORM_BUFFER, mMatrixUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(MAT4), &(mProjMatrix.T()));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


#pragma once
#ifndef JKRESOURCEMANAGER_H_
#define JKRESOURCEMANAGER_H_

#include<string>
#include<unordered_map>
#include"Mesh.h"
#include"Model.h"
#include"GLShader.h"

struct aiScene;
struct aiNode;
struct aiMesh;



class jkResourceManager
{
public:

	static unsigned char* ImportImage(const std::string& imageFilePath, ImageFormat& textureFormat, bool flip = false);

	static inline Texture* ImportTexture(const std::string& textureFile,  TextureType type = TextureType::Diffuse, bool flip=false)
	{
		Texture* texture = new Texture();
		texture->Type = type;
		if (type == TextureType::Diffuse)
			texture->isSRGB = true;
		else
			texture->isSRGB = false;
		texture->pImageData = ImportImage(textureFile,
			texture->TextureFormat, flip);
		if (!texture->pImageData) return nullptr;
		mTextures.push_back(texture);

		return texture;
	};

	static inline void LoadTexture(const std::string& textureFile, Texture* pTexture, bool flip = false)
	{
		pTexture->pImageData = ImportImage(textureFile, 
			pTexture->TextureFormat, flip);
	};

	static inline void FreeTexture(Texture* pTexture) { pTexture->~Texture(); }

	/*static inline jkMesh* ImportMesh(const std::string& modelFile)
	{

	}

	static inline void LoadMesh(const std::string& modelFile, jkMesh* mesh)
	{

	}*/

	static void ImportCubeMap(std::vector<unsigned char*>& faces, ImageFormat& textureFormat, 
		const std::string& cubeMapFolder, const std::string& formatName = ".jpg");

	static void LoadModel(const std::string& modelFile, jkModel* model, bool withAnim=false);

	static inline bool ImportMeshFromOBJ(const std::string& objFilePath, jkMesh* mesh) { return ImportFileOBJ(objFilePath, mesh->mVertexBuffer, mesh->mIndexBuffer); };

	static bool ImportFileOBJ(const std::string& objFilePath, std::vector<Vertex>& rVertexBuffer, std::vector<UINT>& rIndexBuffer);
	static bool ImportFileBMP(const std::string& bmpFilePath, UINT& outWidth, UINT& outHeight, std::vector<COLOR3>& outColorBuffer);

	static void inline ImportShader(const std::string& shaderName, const char* vs_f, const char* fs_f,
		const char* gs_f = nullptr)
	{
		if (auto shader = new glShader(vs_f, fs_f, gs_f))
		{
			mShaderMap.insert(std::make_pair(shaderName, shader));
		}
	}

	static glShader* GetShader(const std::string& shader_name) { return mShaderMap.at(shader_name); }

private:

	//Recursively process node in scene(mesh+bone_weight&id).
	static void RecurProcessNode(aiNode* node, const aiScene* scene, jkModel* model);
	//Process mesh in node.
	static jkMesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, jkModel* model);

	//Global entrance for collecting key frame animations. 
	static void ProcessAnim(const aiScene* scene, jkModel* model);
	//Recursively connect node anims(for bone matrices updation).
	static void RecurLinkNodeAnim(const aiNode* node, const aiScene* scene, NodeAnimation* parent_node_anim, Animation& anim);

	static std::vector<Texture*> mTextures;
	static std::unordered_map<std::string, glShader*> mShaderMap;
};

#endif // !JKRESOURCEMANAGER_H_
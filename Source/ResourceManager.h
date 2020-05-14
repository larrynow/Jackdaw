#pragma once
#ifndef JKRESOURCEMANAGER_H_
#define JKRESOURCEMANAGER_H_

#include<string>
#include<unordered_map>
#include"Mesh.h"
#include"SkeletalMesh.h"
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

	static void ImportCubeMap(std::vector<unsigned char*>& faces, ImageFormat& textureFormat, 
		const std::string& cubeMapFolder, const std::string& formatName = ".jpg");

	static inline std::shared_ptr<jkModel> ImportModel(const std::string& modelFile, const std::vector<std::string>& anima_names)
	{
		auto res = new jkModel(modelFile);
		LoadModel(modelFile, res, anima_names);
		//If Load Fail should delete res.
		
		return std::shared_ptr<jkModel>(res);
	}

	static void LoadModel(const std::string& modelFile, jkModel* model, const std::vector<std::string>& anima_names);

	static inline bool ImportMeshFromOBJ(const std::string& objFilePath, jkMesh* mesh) 
	{ return ImportFileOBJ(objFilePath, mesh->mVertexBuffer, mesh->mIndexBuffer); };

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
	//static jkMesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	static std::shared_ptr<jkMesh> ProcessMesh(aiMesh* mesh, const aiScene* scene, jkModel* model);

	//Global entrance for collecting key frame animations. 
	static void ProcessAnim(const aiScene* scene, jkModel* model, const std::vector<std::string>& anima_names);
	//Recursively connect node anims(for bone matrices updation).
	static AnimationNode* RecurLinkNodeAnim(const aiNode* node, const aiScene* scene, AnimationNode* parent_node_anim, Animation& anim,
		std::unordered_map<std::string, NodeAnimationKeys*>& node_key_anim_map);

	static std::vector<Texture*> mTextures;
	static std::unordered_map<std::string, glShader*> mShaderMap;

	static std::unordered_map<std::string, jkModel*> mModelMap;
};

#endif // !JKRESOURCEMANAGER_H_
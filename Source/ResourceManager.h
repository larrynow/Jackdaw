#pragma once
#ifndef JKRESOURCEMANAGER_H_
#define JKRESOURCEMANAGER_H_

#include<string>
#include"Mesh.h"
#include"Model.h"

struct aiScene;
struct aiNode;
struct aiMesh;

class jkResourceManager
{
public:

	static unsigned char* ImportImage(const std::string& imageFilePath, ImageFormat& textureFormat, bool flip = false);

	static inline Texture* ImportTexture(const std::string& textureFile, bool flip=false)
	{
		Texture* texture = new Texture();
		texture->pImageData = ImportImage(textureFile,
			texture->TextureFormat, flip);
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

	static void LoadModel(const std::string& modelFile, jkModel* model);

	static void ProcessNode(aiNode* node, const aiScene* scene, jkModel* model);

	static jkMesh* ProcessMesh(aiMesh* mesh, const aiScene* scene, const jkModel* model);

	static void ImportCubeMap(std::vector<unsigned char*>& faces, ImageFormat& textureFormat, 
		const std::string& cubeMapFolder, const std::string& formatName = ".jpg");

	static inline bool ImportMeshFromOBJ(const std::string& objFilePath, jkMesh* mesh) { return ImportFileOBJ(objFilePath, mesh->mVertexBuffer, mesh->mIndexBuffer); };

	static bool ImportFileOBJ(const std::string& objFilePath, std::vector<Vertex>& rVertexBuffer, std::vector<UINT>& rIndexBuffer);
	static bool ImportFileBMP(const std::string& bmpFilePath, UINT& outWidth, UINT& outHeight, std::vector<COLOR3>& outColorBuffer);

private:

	static std::vector<Texture*> mTextures;
};

#endif // !JKRESOURCEMANAGER_H_
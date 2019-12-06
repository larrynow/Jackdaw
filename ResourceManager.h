#pragma once
#ifndef JKRESOURCEMANAGER_H_
#define JKRESOURCEMANAGER_H_

#include<string>
#include"Mesh.h"

class jkResourceManager
{
public:

	static inline void ImportTexture(const std::string& textureFile, Texture* pTexture)
	{
		pTexture->pImageData = ImportImage(textureFile, 
			pTexture->TextureFormat);
	};

	static inline void FreeTexture(Texture* pTexture) { pTexture->~Texture(); }

	static void ImportCubeMap(std::vector<unsigned char*>& faces, ImageFormat& textureFormat, 
		const std::string& cubeMapFolder, const std::string& formatName = ".jpg");

	static inline bool ImportMeshFromOBJ(const std::string& objFilePath, jkMesh* mesh) { return ImportFileOBJ(objFilePath, mesh->mVertexBuffer, mesh->mIndexBuffer); };

private:

	static unsigned char* ImportImage(const std::string& imageFilePath, ImageFormat& textureFormat, bool flip = true);

	static bool ImportFileOBJ(const std::string& objFilePath, std::vector<Vertex>& rVertexBuffer, std::vector<UINT>& rIndexBuffer);
	static bool ImportFile_BMP(const std::string& bmpFilePath, UINT& outWidth, UINT& outHeight, std::vector<COLOR3>& outColorBuffer);

};

#endif // !JKRESOURCEMANAGER_H_
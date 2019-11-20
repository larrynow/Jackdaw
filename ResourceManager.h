#pragma once
#ifndef JKRESOURCEMANAGER_H_
#define JKRESOURCEMANAGER_H_

#include<string>
#include"Mesh.h"

class jkResourceManager
{
	static inline bool ImportMeshFromOBJ(const std::string& objFilePath, jkMesh* mesh) { return ImportFileOBJ(objFilePath, *mesh->m_pVertexBuffer, *mesh->m_pIndexBuffer); };
	static inline bool ImportTexture_BMP(const std::string& bmpFilePath, Texture* tex) { return ImportFile_BMP(bmpFilePath, tex->Width, tex->Height, *tex->pColorBuffer); };


private:

	static bool ImportFileOBJ(const std::string& objFilePath, std::vector<Vertex>& rVertexBuffer, std::vector<UINT>& rIndexBuffer);
	static bool ImportFile_BMP(const std::string& bmpFilePath, UINT& outWidth, UINT& outHeight, std::vector<COLOR3>& outColorBuffer);

};

#endif // !JKRESOURCEMANAGER_H_
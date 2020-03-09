#include "ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include<fstream>
#include<sstream>

std::vector<Texture*> jkResourceManager::mTextures = std::vector<Texture*>();
std::unordered_map<std::string, glShader*> jkResourceManager::mShaderMap = std::unordered_map<std::string, glShader*>();

void jkResourceManager::LoadModel(const std::string& modelFile, jkModel* model)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(modelFile, 
		aiProcess_Triangulate | aiProcess_FlipUVs);
	// Use aiProcess_OptimizeMeshes to splice triangles.

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	ProcessNode(scene->mRootNode, scene, model);
}

void jkResourceManager::ProcessNode(aiNode* node, const aiScene* scene, jkModel* model)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		model->mMeshes.push_back(ProcessMesh(mesh, scene, model));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, model);
	}
}

jkMesh* jkResourceManager::ProcessMesh(aiMesh* mesh, const aiScene* scene, const jkModel* model)
{
	std::vector<Vertex> vertices;
	std::vector<UINT> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// Positions.
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		// Normals.
		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		// Texture coordinates.
		if (mesh->mTextureCoords[0])// If have texture coordinate.
		{
			// Only use first texture coord.
			vertex.texcoord.x = mesh->mTextureCoords[0][i].x;
			vertex.texcoord.y = mesh->mTextureCoords[0][i].y;
		}// Else texture coord will be (0,0).
		
		// Tangent.
		if (mesh->mTangents)
		{
			vertex.tangent.x = mesh->mTangents[i].x;
			vertex.tangent.y = mesh->mTangents[i].y;
			vertex.tangent.z = mesh->mTangents[i].z;
		}

		// Bitangent.
		if (mesh->mBitangents)
		{
			vertex.bitangent.x = mesh->mBitangents[i].x;
			vertex.bitangent.y = mesh->mBitangents[i].y;
			vertex.bitangent.z = mesh->mBitangents[i].z;
		}

		vertices.push_back(vertex);
	}
	
	// Indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
		
	// Only load mesh currently.

	auto ret = new jkMesh(model->mTransform.position);
	ret->mVertexBuffer = std::move(vertices);
	ret->mIndexBuffer = std::move(indices);

	return ret;
}

void jkResourceManager::ImportCubeMap(std::vector<unsigned char*>& faces, ImageFormat& imgFormat,
	const std::string& cubeMapFolder, const std::string& formatName)
{
	std::vector<std::string> faceNames = { "right", "left", "top", "bottom", "front", "back" };
	for (auto face : faceNames)
	{
		faces.push_back(ImportImage(cubeMapFolder+"/"+face+formatName, imgFormat, false));
	}
}

unsigned char* jkResourceManager::ImportImage(const std::string& imageFilePath, ImageFormat& imgFormat, bool flip)
{
	stbi_set_flip_vertically_on_load(flip);
	return stbi_load(imageFilePath.c_str(), &imgFormat.width, &imgFormat.height, &imgFormat.channels, 0);
}

// For obj loading.
struct OBJVertexIndex
{
	inline bool operator ==(const OBJVertexIndex& _OV)const
	{
		return (vertexID == _OV.vertexID && texCoordID == _OV.texCoordID && vertexNormalID == _OV.vertexNormalID);
	}

	UINT vertexID;
	UINT texCoordID;
	UINT vertexNormalID;
};

bool jkResourceManager::ImportFileOBJ(const std::string& objFilePath, std::vector<Vertex>& rVertexBuffer, std::vector<UINT>& rIndexBuffer)
{
	std::fstream file(objFilePath);
	if (!file.good())
	{
		//MSG("OBJ file import ERROR!");
		return false;
	}

	std::vector<VEC3> pointList;// XYZ coordinates.
	std::vector<VEC3> normalList;// Normals.
	std::vector<VEC2> texCoordList;// Texture coordinates.
	std::vector<OBJVertexIndex> vertexInfoList;

	std::string str;// Current string.
	while (!file.eof())
	{
		file >> str;// Get string from stream.

		// "v 1.0000000 0.52524242 5.12312345"
		if (str == "v")
		{
			VEC3 curPoint;
			file >> curPoint.x >> curPoint.y >> curPoint.z;
			pointList.push_back(curPoint);
		}
		// "vn 1.0000000 0.52524242 5.12312345"
		if (str == "vn")
		{
			VEC3 curNormal;
			file >> curNormal.x >> curNormal.y >> curNormal.z;
			normalList.push_back(curNormal);
		}
		// "vt 1.0000000 0.0000000"
		if (str == "vt")
		{
			VEC2 curTexCoord;
			file >> curTexCoord.x >> curTexCoord.y;
			texCoordList.push_back(curTexCoord);
		}

		if (str == "f")
		{
			// For 3 vertices.
			for (size_t i = 0; i < 3; i++)
			{
				OBJVertexIndex v;
				std::stringstream tmpStream;
				std::string str;

				file >> str;
				// Replace / with ' '. "1/2/3" for "vertex index", "texcoord index", "normal index".
				for (auto& c : str) { if (c == '/')c = ' '; };
				tmpStream << str;
				tmpStream >> v.vertexID >> v.texCoordID >> v.vertexNormalID;

				bool isExist = false;
				UINT existedVertexIndex = 0;
				for (size_t j = 0; j < vertexInfoList.size(); j++)
				{
					if (vertexInfoList[j] == v)// Already exist.
					{
						isExist = true;
						existedVertexIndex = j;
						break;
					}
				}

				if (!isExist)
				{
					vertexInfoList.push_back(v);
					rIndexBuffer.push_back(vertexInfoList.size() - 1);// Save index.
				}
				else
				{
					rIndexBuffer.push_back(existedVertexIndex);
				}
			}
		}
	}// file eof.

	file.close();

	//  Resize.
	rVertexBuffer.resize(vertexInfoList.size());
	for (size_t i = 0; i < vertexInfoList.size(); i++)
	{
		Vertex vertex;
		auto vInfo = vertexInfoList.at(i);// Get infos.
		if (vInfo.vertexID == 0 || vInfo.texCoordID == 0 || vInfo.vertexNormalID == 0)continue;// For safety.
		vertex.pos = pointList.at(vInfo.vertexID - 1);
		vertex.texcoord = texCoordList.at(vInfo.texCoordID - 1);
		vertex.normal = normalList.at(vInfo.vertexNormalID - 1);

		rVertexBuffer.at(i) = vertex;
	}

	return true;
}

// For bmp loading.
struct BITMAPFILEHEADER
{
	unsigned short   bfType;
	unsigned long    bfSize;
	unsigned short   bfReserved1;
	unsigned short   bfReserved2;
	unsigned long    bfOffBits;
};

struct BITMAPINFOHEADER {
	unsigned long    biSize;
	long             biWidth;
	long             biHeight;
	unsigned short   biPlanes;
	unsigned short   biBitCount;
	unsigned long    biCompression;
	unsigned long    biSizeImage;
	long             biXPelsPerMeter;
	long             biYPelsPerMeter;
	unsigned long    biClrUsed;
	unsigned long    biClrImportant;
};

bool jkResourceManager::ImportFileBMP(const std::string& bmpFilePath, UINT& outWidth, UINT& outHeight, std::vector<COLOR3>& outColorBuffer)
{
	BITMAPFILEHEADER fileHeader = { 0 };

	std::ifstream fileIn(bmpFilePath, std::ios::binary | std::ios::in);
	if (!fileIn.good())
	{
		//MSG("Load BMP : File Open Failed!!");
		return false;
	}

	fileIn.read((char*)&fileHeader, sizeof(BITMAPFILEHEADER));
	if (fileHeader.bfType != 0x4D42)
	{
		fileIn.close();
		return false;
	}


	BITMAPINFOHEADER fileInfoHeader;
	fileIn.read((char*)&fileInfoHeader, sizeof(BITMAPINFOHEADER));
	outWidth = fileInfoHeader.biWidth;
	outHeight = fileInfoHeader.biHeight;// Minus?

	int biBitCount = fileInfoHeader.biBitCount;
	int byteCount = biBitCount / 8;// Byte for one color.
	int lineSize = (outWidth * biBitCount / 8 + 3) / 4 * 4;// Size for one line, must be 4N.

	// Move to color data(compared to begin).
	int offset = fileHeader.bfOffBits;// Offset to get real data.
	fileIn.seekg(offset, std::ios::beg);

	unsigned char* byteBuff = new unsigned char[outHeight * lineSize];
	fileIn.read((char*)byteBuff, outHeight * lineSize);

	fileIn.close();
	outColorBuffer.resize(outHeight * outWidth);
	// Save(maybe in row flipped order).
	for (int i = outHeight - 1; i >= 0; --i)
		for (int j = 0; j < int(outWidth); ++j)
		{
			int r, g, b;

			b = *(byteBuff + i * lineSize + (j * byteCount + 0));
			g = *(byteBuff + i * lineSize + (j * byteCount + 1));
			r = *(byteBuff + i * lineSize + (j * byteCount + 2));

			//std::cout << r <<","<<g<<","<<b<<std::endl;
			COLOR3 tempColor = { r / 255.f, g / 255.f, b / 255.f };
			outColorBuffer.at((i)*outWidth + j) = tempColor;
		}
	delete[] byteBuff;


	return true;
}

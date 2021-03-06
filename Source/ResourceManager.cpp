#include "ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include<fstream>
#include<sstream>

std::vector<Texture*> jkResourceManager::mTextures = std::vector<Texture*>();
std::unordered_map<std::string, glShader*> jkResourceManager::mShaderMap = 
	std::unordered_map<std::string, glShader*>();
std::unordered_map<std::string, jkModel*> jkResourceManager::mModelMap =
	std::unordered_map<std::string, jkModel*>();

// Helper from aiMat4 to MAT4.
auto aiMat4_to_Mat4 = [](const aiMatrix4x4& aimat)
{
	MAT4 mat;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mat[i][j] = aimat[i][j];
		}
	}

	return mat;
};

void jkResourceManager::LoadModel(const std::string& modelFile, jkModel* model,
	const std::vector<std::string>& anima_names)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(modelFile, 
		aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace
		| aiProcess_GenSmoothNormals);
	// Use aiProcess_OptimizeMeshes to splice triangles.

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	RecurProcessNode(scene->mRootNode, scene, model);
	if(anima_names.size()!=0)
		ProcessAnim(scene, model, anima_names);
}

void jkResourceManager::RecurProcessNode(aiNode* node, const aiScene* scene, jkModel* model)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		model->mMeshes.push_back(ProcessMesh(mesh, scene, model));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		RecurProcessNode(node->mChildren[i], scene, model);
	}
}

std::shared_ptr<jkMesh> jkResourceManager::ProcessMesh(aiMesh* mesh, const aiScene* scene, jkModel* model)
{
	//Resource new.
	//auto ret = new jkMesh();
	auto ret = std::make_shared<jkMesh>();
		//new jkSkeletalMesh();

	std::vector<Vertex>& vertices = ret->mVertexBuffer;
	std::vector<UINT>& indices = ret->mIndexBuffer;

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

	std::vector<MAT4>& bone_offset = model->mBoneOffsetMatrices;
	bone_offset.reserve(mesh->mNumBones);

	auto& bone_mat = model->mBoneMatrices;
	bone_mat.resize(mesh->mNumBones);

	

	for (int i = 0; i < mesh->mNumBones; i++)
	{
		auto bone = mesh->mBones[i];
		bone_offset.push_back(aiMat4_to_Mat4(bone->mOffsetMatrix));
		model->mBoneIDMap.insert(
			std::make_pair(bone->mName.C_Str(), i));

		for (int j = 0; j < bone->mNumWeights; j++)
		{
			auto weight = bone->mWeights[j];
			//Let bone influence vertex.
			vertices[weight.mVertexId].AddBone(i, weight.mWeight);
		}
	}

	return ret;
}

void jkResourceManager::ProcessAnim(const aiScene* scene, jkModel* model, 
	const std::vector<std::string>& anima_names)
{
	auto VEC3FromAiVector = [](const aiVector3D& aVec3)
	{
		return VEC3(aVec3.x, aVec3.y, aVec3.z);
	};

	auto VEC4FromAiQuaternion = [](const aiQuaternion& aQuat)
	{
		return VEC4(aQuat.w, aQuat.x, aQuat.y, aQuat.z);
	};

	model->mAnimations.resize(scene->mNumAnimations);
	for (int i = 0; i < scene->mNumAnimations; i++)
	{
		if (i > anima_names.size()) break;

		auto ai_animation = scene->mAnimations[i];
		double tick_per_sec = ai_animation->mTicksPerSecond;

		model->mAnimationNameMap.insert(std::make_pair(anima_names[i], i));
		model->mAnimations[i].animationID = i;
		model->mAnimations[i].pRoot = nullptr;
		model->mAnimations[i].tickPerSec = tick_per_sec;

		double max_time = 0.0;

		std::unordered_map<std::string, NodeAnimationKeys*> node_key_anim_map;
		node_key_anim_map.reserve(ai_animation->mNumChannels);

		//Collect channels in an animation(node which changes in this animation).
		for (int j = 0; j < ai_animation->mNumChannels; j++)
		{
			//Each channel affect a node.
			auto channel = ai_animation->mChannels[j];
			NodeAnimationKeys* nodeAnimKeys = new NodeAnimationKeys();
			nodeAnimKeys->positionKeys.reserve(channel->mNumPositionKeys);
			nodeAnimKeys->rotationKeys.reserve(channel->mNumRotationKeys);
			nodeAnimKeys->scalingKeys.reserve(channel->mNumScalingKeys);

			for (int i = 0; i < channel->mNumPositionKeys; i++)
			{
				nodeAnimKeys->positionKeys.push_back(
					KeyInfo<VEC3>(channel->mPositionKeys[i].mTime,
						VEC3FromAiVector(channel->mPositionKeys[i].mValue)));
			}
			for (int i = 0; i < channel->mNumRotationKeys; i++)
			{
				nodeAnimKeys->rotationKeys.push_back(
					KeyInfo<Quaternion>(channel->mRotationKeys[i].mTime,
						VEC4FromAiQuaternion(channel->mRotationKeys[i].mValue)));
			}
			for (int i = 0; i < channel->mNumScalingKeys; i++)
			{
				nodeAnimKeys->scalingKeys.push_back(
					KeyInfo<VEC3>(channel->mScalingKeys[i].mTime,
						VEC3FromAiVector(channel->mScalingKeys[i].mValue)));
			}
			auto pos_max_time = channel->mPositionKeys[channel->mNumPositionKeys - 1].mTime;
			auto rot_max_time = channel->mRotationKeys[channel->mNumRotationKeys - 1].mTime;
			auto sca_max_time = channel->mScalingKeys[channel->mNumScalingKeys - 1].mTime;

			auto max_in_3 = std::max(std::max(pos_max_time, rot_max_time), sca_max_time);
			max_time = std::max(max_in_3, max_time);

			node_key_anim_map.insert(
				std::make_pair(channel->mNodeName.C_Str(), nodeAnimKeys));
		}
		model->mAnimations[i].maxTime = max_time;

		model->mAnimations[i].pRoot = RecurLinkNodeAnim(scene->mRootNode, scene, nullptr, model->mAnimations[i], node_key_anim_map);
	}
}

AnimationNode* jkResourceManager::RecurLinkNodeAnim(const aiNode* node, const aiScene* scene,
	AnimationNode* parent_node_anim, Animation& anim,
	std::unordered_map<std::string, NodeAnimationKeys*>& node_key_anim_map)
{
	AnimationNode* pCurAnimNode = new AnimationNode(node->mName.C_Str());
	auto it = node_key_anim_map.find(node->mName.C_Str());
	if (it != node_key_anim_map.end())//This node is changes in this animation.
	{
		pCurAnimNode->pAnimationKeys = (it->second);
	}
	else//This node is not changes in this animation, only record transform with parent.
	{
		pCurAnimNode->transformWithParent = aiMat4_to_Mat4(node->mTransformation);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		pCurAnimNode->childNodeAnims.push_back(
			RecurLinkNodeAnim(node->mChildren[i], scene, pCurAnimNode, anim, node_key_anim_map));
	}

	return pCurAnimNode;
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

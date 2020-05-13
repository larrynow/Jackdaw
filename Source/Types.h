#pragma once
#ifndef JKTYPES_H_
#define JKTYPES_H_

#include"Math.h"
#include<vector>
#include<unordered_map>

using namespace jkMath;

template<typename T>
struct Rect
{
	T Width;
	T Height;
};

//////////////////////////////////////////
// Types for rendering.

typedef VEC3 COLOR3;
typedef VEC4 COLOR4;

inline float RGB2Gray(const UINT& r, const UINT& g, const UINT& b) {
	return (float)((r * 19595 + g * 38469 + b * 7472) >> 16);
}

inline float RGB2Gray(const VEC3& rgb_color) {
	return rgb_color.DotProduct({ 0.2989f, 0.587f, 0.114f});
}

struct Vertex
{
	Vertex()
	{
		std::fill(boneIDs, boneIDs + MaxBoneNum, 0);
		std::fill(boneWeigths, boneWeigths + MaxBoneNum, 0);
	}

	Vertex(const VEC3& p, const COLOR4& c, const VEC2& tex, const VEC3& n) :
		pos(p), color(c), texcoord(tex), normal(n)
	{
		std::fill(boneIDs, boneIDs + MaxBoneNum, 0);
		std::fill(boneWeigths, boneWeigths + MaxBoneNum, 0);
	}

	Vertex(const VEC3& p, const COLOR4& c, const VEC2& tex,
		const VEC3& n, const VEC3& t, const VEC3& b): 
		pos(p), color(c), texcoord(tex), normal(n), tangent(t), bitangent(b)
	{
		std::fill(boneIDs, boneIDs + MaxBoneNum, 0);
		std::fill(boneWeigths, boneWeigths + MaxBoneNum, 0);
	}

	VEC3 pos;
	COLOR4 color;
	VEC2 texcoord;

	VEC3 normal;
	VEC3 tangent;
	VEC3 bitangent;

	enum { MaxBoneNum = 3 };

	int boneIDs[MaxBoneNum];//Bone id in mesh.
	float boneWeigths[MaxBoneNum];

	void AddBone(int id, float weight)
	{
		int i;
		for (i = 0; i < MaxBoneNum; i++)
		{
			if (boneWeigths[i] == 0)//Find index to add bone info.
			{
				break;
			}
		}
		if (i < MaxBoneNum)
		{
			boneIDs[i] = id;
			boneWeigths[i] = weight;
		}
	}
};

template<typename T>
struct KeyInfo
{
	KeyInfo(double t, T v) : time(t), value(v) {}
	KeyInfo(double t) : KeyInfo(t, static_cast<T>(0.f)) {}

	double time;
	T value;

	// Compare with time.
	bool operator <(const KeyInfo<T>& k) const
	{
		return time < k.time;
	}
	bool operator >(const KeyInfo<T>& k) const
	{
		return k < (*this);
	}

};

//Animation for one node.
struct NodeAnimation
{
	NodeAnimation(std::string name) :nodeName(name) {};

	std::string nodeName;
	std::vector<NodeAnimation*> childNodeAnims;

	//Key Frame animations.
	std::vector<KeyInfo<VEC3>> positionKeys;
	std::vector<KeyInfo<VEC4>> rotationKeys;
	std::vector<KeyInfo<VEC3>> scalingKeys;
};

struct Animation
{
	Animation(int i) :animationID(i), maxTime(0), pRootAnimationNode(nullptr){};
	Animation() :Animation(0) {};

	~Animation(){}//Should manage animation resource correctly.

	int animationID;

	double maxTime;
	NodeAnimation* pRootAnimationNode;

	//std::vector<NodeAnimation> nodeAnimations;
	std::unordered_map<std::string, NodeAnimation> nodeAnimationMap;

	//BoneMatrices at each tick.
	//std::vector<std::vector<MAT4>> TickBoneMatrices;
};

class jkMesh;

struct RenderData
{
	// Backend Renderer load Mesh as RenderData.
	RenderData() : pOriginMesh(nullptr){}
	virtual ~RenderData() {}

	jkMesh* pOriginMesh;
	UINT IndexSize;
};

struct CubeMapData {};// Empty Render data class.

struct InstanceRenderData { UINT instanceNum; };

struct SurroundingRenderData {};

struct ImageFormat
{
	ImageFormat(int w, int h, int c) : width(w), height(h), channels(c) {};
	ImageFormat() : ImageFormat(0, 0, 0) {};

	int width, height, channels;
};

enum class TextureType
{
	Diffuse,
	Specular,
	Normal,
	Height
};

struct Texture
{
	Texture() : TextureFormat(), Type(TextureType::Diffuse), pImageData(nullptr), isSRGB(true) {};

	~Texture() { delete pImageData; };

	unsigned char* pImageData;
	ImageFormat TextureFormat;
	TextureType Type;
	bool isSRGB;
};

struct Material
{
	Material():diffuseMap(nullptr), specularMap(nullptr), 
		normalMap(nullptr), heightMap(nullptr), shininess(32){ }
	Material(Texture* dif, Texture* spec, int shin) :
		diffuseMap(dif), specularMap(spec), 
		normalMap(nullptr), heightMap(nullptr), shininess(shin) {}
	Material(Texture* dif) : Material() {diffuseMap = dif;}
	Material(int shi) : Material() { shininess = shi; }

	Texture* diffuseMap;
	Texture* specularMap;

	Texture* normalMap;
	Texture* heightMap;

	int shininess;// For specular. Bigger for smaller specular boundary.
};

struct Light;

#endif // !JKTYPES_H_

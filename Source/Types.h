#pragma once
#ifndef JKTYPES_H_
#define JKTYPES_H_

#include"Math.h"
#include<vector>

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
	VEC3 pos;
	COLOR4 color;
	VEC2 texcoord;

	VEC3 normal;
	VEC3 tangent;
	VEC3 bitangent;
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

////////////////////////////////////////////////
// Types for entity.

struct Transform
{
	VEC3 position;
	float scaleX;
	float scaleY;
	float scaleZ;

	float mRotationPitch, mRotationYaw, mRotationRoll;// Angle.
	//TODO: Latter for rotation
};

#endif // !JKTYPES_H_

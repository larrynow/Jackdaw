#pragma once
#ifndef JKTYPES_H_
#define JKTYPES_H_

#include"Math.h"
#include<vector>

using namespace jkMath;

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
};

struct CubeMapData {};// Empty Render data class.

struct InstanceRenderData {};// Empty Render data class.

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
	Specular
};

struct Texture
{
	Texture() : TextureFormat(), pImageData(nullptr) {};

	~Texture() { delete pImageData; };

	unsigned char* pImageData;
	ImageFormat TextureFormat;
	TextureType Type;
};

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

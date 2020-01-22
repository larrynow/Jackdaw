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

struct Vertex
{
	VEC3 pos;
	COLOR4 color;
	VEC3 normal;
	VEC2 texcoord;
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

struct ImageFormat
{
	ImageFormat(int w, int h, int c) : width(w), height(h), channels(c) {};
	ImageFormat() : ImageFormat(0, 0, 0) {};

	int width, height, channels;
};

struct Texture
{
	Texture() : TextureFormat(), pImageData(nullptr) {};

	~Texture() { delete pImageData; };

	unsigned char* pImageData;
	ImageFormat TextureFormat;
};

////////////////////////////////////////////////
// Types for entity.

struct Transform
{
	VEC3 position;
	float scaleX;
	float scaleY;
	float scaleZ;

	//TODO: Latter for rotation
};

#endif // !JKTYPES_H_

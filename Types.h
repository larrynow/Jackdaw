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

struct Texture
{
	Texture() : width(0), height(0), channels(0), pImageData(nullptr) {};

	~Texture() { delete pImageData; };

	unsigned char* pImageData;
	UINT width, height;
	UINT channels;
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

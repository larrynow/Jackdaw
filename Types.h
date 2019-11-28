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
	VEC3 normal;
	VEC2 texcoord;
};

struct RenderData
{
	RenderData() :pVertexBuffer(nullptr), pIndexBuffer(nullptr), offset(0) {}

	~RenderData() {}

	std::vector<Vertex>* pVertexBuffer;
	std::vector<UINT>* pIndexBuffer;
	UINT offset;

	MAT4 modelMatrix;

};

struct Texture
{
	Texture() : width(0), height(0) { pColorBuffer = new std::vector<COLOR3>; };
	~Texture() { delete pColorBuffer; };

	COLOR3 GetPixel(UINT x, UINT y) const
	{
		if (x < width && y < height)
		{
			UINT index = y * width + x;
			return pColorBuffer->at(index);

		}
		else
			return COLOR3();

	}

	std::vector<COLOR3>* pColorBuffer;
	UINT width, height;
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

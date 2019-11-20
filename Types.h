#pragma once
#ifndef JKTYPES_H_
#define JKTYPES_H_

#include"Math.h"
#include<vector>

using namespace jkMath;

struct RECT
{
	UINT X;
	UINT Y;
};

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
	std::vector<Vertex>* pVertexBuffer;
	std::vector<UINT>* pIndexBuffer;
	UINT offset;
	UINT vertexCount;
};

struct Texture
{
	Texture() : Width(0), Height(0) { pColorBuffer = new std::vector<COLOR3>; };
	~Texture() { delete pColorBuffer; };

	COLOR3 GetPixel(UINT x, UINT y) const
	{
		if (x < Width && y < Height)
		{
			UINT index = y * Width + x;
			return pColorBuffer->at(index);

		}
		else
			return COLOR3();

	}

	std::vector<COLOR3>* pColorBuffer;
	UINT Width, Height;
};

struct jkMap {

};
#endif // !JKTYPES_H_

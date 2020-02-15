#pragma once
#ifndef JKGEOMETRY_H_
#define JKGEOMETRY_H_
#include"Types.h"
#include"Mesh.h"

class jkGeometry
{
public:
	static void CreateCube(const float width, 
		std::vector<Vertex>& vertices, std::vector<UINT>& indices);// Create a Cube's vertices and indices.
	
	inline static void MakeCubeMesh(jkMesh* mesh, const float width) 
	{ CreateCube(width, mesh->mVertexBuffer, mesh->mIndexBuffer); };

	static float* CreateCubeVertices();

	static void CreateGrid(const float width, const float height, UINT m, UINT n,
		std::vector<Vertex>& vertices, std::vector<UINT>& indices);

	inline static void MakePlaneMesh(const float width, const float height, 
		UINT m, UINT n, jkMesh* gridMesh)
	{
		CreateGrid(width, height, m, n, gridMesh->mVertexBuffer, gridMesh->mIndexBuffer);
	};

	static void MakeHeightMapMesh(const float width, const float height,
		UINT m, UINT n, const std::vector<float>& heightInfo, jkMesh* gridMesh);

};

#endif // !JKGEOMETRY_H_


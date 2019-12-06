#pragma once
#ifndef JKGEOMETRY_H_
#define JKGEOMETRY_H_
#include"Types.h"
#include"Mesh.h"

class jkGeometry
{
public:
	static void CreateCube(const float width, std::vector<Vertex>& vertices, std::vector<UINT>& indices);// Create a Cube's vertices and indices.
	inline static void MakeCubeMesh(jkMesh* mesh, const float width) { CreateCube(width, mesh->mVertexBuffer, mesh->mIndexBuffer); };

	static float* CreateCubeVertices();
};

#endif // !JKGEOMETRY_H_


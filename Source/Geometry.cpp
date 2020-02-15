#include "Geometry.h"

void jkGeometry::CreateCube(const float width, std::vector<Vertex>& vertices, std::vector<UINT>& indices)
{
	////////////////////////////////////////////////////////////
	// Create cube as vertices and indices.
	// Build 6 faces, each face with 2 triangles(6 vertices) all in anticlockwise.

	vertices.clear();
	indices.clear();

	// Front face.
	vertices.push_back({ VEC3(-width / 2, -width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 0.f), VEC3(0.f, 0.f, -1.f) });// Bottom-left.
	vertices.push_back({ VEC3(width / 2,  width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 1.f), VEC3(0.f, 0.f, -1.f) });// Top-right.
	vertices.push_back({ VEC3(width / 2, -width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 0.f), VEC3(0.f, 0.f, -1.f) });// Bottom-right.
	//vertices.push_back({ VEC3( width/2,  width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, -1.f), VEC2(1.f, 1.f) });// Top-right.
	//vertices.push_back({ VEC3(-width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, -1.f), VEC2(0.f, 0.f) });// Bottom-left.
	vertices.push_back({ VEC3(-width / 2,  width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 1.f), VEC3(0.f, 0.f, -1.f) });// Top-left.

	// Clockwise: 0, 1, 2; 1, 0, 3;

	// Back face.
	vertices.push_back({ VEC3(-width / 2, -width / 2, width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 0.f), VEC3(0.f, 0.f, 1.f) });
	vertices.push_back({ VEC3(width / 2, -width / 2, width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 0.f), VEC3(0.f, 0.f, 1.f) });
	vertices.push_back({ VEC3(width / 2,  width / 2, width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 1.f), VEC3(0.f, 0.f, 1.f) });
	//vertices.push_back({ VEC3( width/2,  width/2, width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, 1.f), VEC2(1.f, 1.f) });
	vertices.push_back({ VEC3(-width / 2,  width / 2, width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 1.f), VEC3(0.f, 0.f, 1.f) });
	//vertices.push_back({ VEC3(-width/2, -width/2, width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, 1.f), VEC2(0.f, 0.f) });

	// 4, 5, 6; 6, 7, 4;

	// Left face.
	vertices.push_back({ VEC3(-width / 2,  width / 2,  width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 0.f), VEC3(-1.f, 0.f, 0.f) });
	vertices.push_back({ VEC3(-width / 2,  width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 1.f), VEC3(-1.f, 0.f, 0.f) });
	vertices.push_back({ VEC3(-width / 2, -width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 1.f), VEC3(-1.f, 0.f, 0.f) });
	//vertices.push_back({ VEC3(-width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(-1.f, 0.f, 0.f), VEC2(0.f, 1.f) });
	vertices.push_back({ VEC3(-width / 2, -width / 2,  width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 0.f), VEC3(-1.f, 0.f, 0.f) });
	//vertices.push_back({ VEC3(-width/2,  width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(-1.f, 0.f, 0.f), VEC2(1.f, 0.f) });

	// 8, 9, 10; 10, 11, 8;

	// Right face.
	vertices.push_back({ VEC3(width / 2,  width / 2,  width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 0.f), VEC3(1.f, 0.f, 0.f) });
	vertices.push_back({ VEC3(width / 2, -width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 1.f), VEC3(1.f, 0.f, 0.f) });
	vertices.push_back({ VEC3(width / 2,  width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 1.f), VEC3(1.f, 0.f, 0.f) });
	//vertices.push_back({ VEC3(width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(1.f, 0.f, 0.f), VEC2(0.f, 1.f) });
	//vertices.push_back({ VEC3(width/2,  width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(1.f, 0.f, 0.f), VEC2(1.f, 0.f) });
	vertices.push_back({ VEC3(width / 2, -width / 2,  width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 0.f), VEC3(1.f, 0.f, 0.f) });

	// 12, 13, 14; 13, 12, 15;

	// Bottom face.
	vertices.push_back({ VEC3(-width / 2, -width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 1.f), VEC3(0.f, -1.f, 0.f) });
	vertices.push_back({ VEC3(width / 2, -width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 1.f), VEC3(0.f, -1.f, 0.f) });
	vertices.push_back({ VEC3(width / 2, -width / 2,  width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 0.f), VEC3(0.f, -1.f, 0.f) });
	//vertices.push_back({ VEC3( width/2, -width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, -1.f, 0.f), VEC2(1.f, 0.f) });
	vertices.push_back({ VEC3(-width / 2, -width / 2,  width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 0.f), VEC3(0.f, -1.f, 0.f) });
	//vertices.push_back({ VEC3(-width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, -1.f, 0.f), VEC2(0.f, 1.f) });

	// 16, 17, 18; 18, 19, 16;

	// Up face.
	vertices.push_back({ VEC3(-width / 2,  width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 1.f), VEC3(0.f, 1.f, 0.f) });
	vertices.push_back({ VEC3(width / 2,  width / 2,  width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 0.f), VEC3(0.f, 1.f, 0.f) });
	vertices.push_back({ VEC3(width / 2,  width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 1.f), VEC3(0.f, 1.f, 0.f) });
	//vertices.push_back({ VEC3( width/2,  width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 1.f, 0.f), VEC2(1.f, 0.f) });
	//vertices.push_back({ VEC3(-width/2,  width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 1.f, 0.f), VEC2(0.f, 1.f) });
	vertices.push_back({ VEC3(-width / 2,  width / 2,  width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 0.f), VEC3(0.f, 1.f, 0.f) });

	// 20, 21, 22; 21, 20, 23;

	indices.insert(indices.end(), { 0,1,2,1,0,3,4,5,6,6,7,4,8,9,10,10,11,8,12,13,14,13,12,15,16,17,18,18,19,16,20,21,22,21,20,23 });
}

float* jkGeometry::CreateCubeVertices()
{
	///////////////////////////////////
	// Create cube vertice data.

	return new float[108]
	{
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

}

void jkGeometry::CreateGrid(const float width, const float height, UINT m, UINT n, 
	std::vector<Vertex>& vertices, std::vector<UINT>& indices)
{
	float stepW = width / (m - 1);
	float stepH = height / (n - 1);

	vertices.resize(m * n);

	float TEX_SIZE = 10.f;

	for (size_t i = 0; i < n; i++)
	{
		float h = (i * stepH) - (height / 2);
		for (size_t j = 0; j < m; j++)
		{
			float w = (j * stepW) - (width / 2);
			vertices[i * m + j].pos = { w, 0.f, h };
			vertices[i * m + j].texcoord = {stepH*i/TEX_SIZE, stepW*j/TEX_SIZE};
		}
	}

	for (size_t i = 0; i < n-1; i++)
	{
		for (size_t j = 0; j < m-1; j++)
		{
			// upper triangle.
			indices.push_back(i * m + j);
			indices.push_back(i * m + j + 1);
			indices.push_back((i + 1) * m + j + 1);

			// down triangle.
			indices.push_back(i * m + j);
			indices.push_back((i + 1) * m + j + 1);
			indices.push_back((i + 1) * m + j);

		}
	}
}

void jkGeometry::MakeHeightMapMesh(const float width, const float height, UINT m, UINT n, 
	const std::vector<float>& heightInfo, jkMesh* gridMesh)
{
	/*
	* width, height : mesh size to create.
	* m, n : mesh grid size.
	* heightInfo : mesh height values.
	* gridMesh : return created mesh.
	*/

	if (heightInfo.size() != m * n)// Height map size don't match grid size, do nothing.
		return;
	CreateGrid(width, height, m, n, gridMesh->mVertexBuffer, gridMesh->mIndexBuffer);
	for (size_t i = 0; i < heightInfo.size(); i++)
	{
		gridMesh->mVertexBuffer.at(i).pos.y = heightInfo.at(i);
	}
}

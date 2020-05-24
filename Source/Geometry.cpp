#include "Geometry.h"

void jkGeometry::mCreateCube(const float width, std::vector<Vertex>& vertices, std::vector<UINT>& indices)
{
	////////////////////////////////////////////////////////////
	// Create cube as vertices and indices.
	// Build 6 faces, each face with 2 triangles(6 vertices) all in anticlockwise.

	vertices.clear();
	indices.clear();

	// Front face.
	vertices.push_back({ VEC3(-width / 2.f, -width / 2.f, -width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 0.f), VEC3(0.f, 0.f, -1.f) });// Bottom-left.
	vertices.push_back({ VEC3(width / 2.f,  width / 2.f, -width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 1.f), VEC3(0.f, 0.f, -1.f) });// Top-right.
	vertices.push_back({ VEC3(width / 2.f, -width / 2.f, -width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 0.f), VEC3(0.f, 0.f, -1.f) });// Bottom-right.
	//vertices.push_back({ VEC3( width/2,  width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, -1.f), VEC2(1.f, 1.f) });// Top-right.
	//vertices.push_back({ VEC3(-width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, -1.f), VEC2(0.f, 0.f) });// Bottom-left.
	vertices.push_back({ VEC3(-width / 2.f,  width / 2.f, -width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 1.f), VEC3(0.f, 0.f, -1.f) });// Top-left.

	// Clockwise: 0, 1, 2; 1, 0, 3;

	// Back face.
	vertices.push_back({ VEC3(-width / 2.f, -width / 2.f, width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 0.f), VEC3(0.f, 0.f, 1.f) });
	vertices.push_back({ VEC3(width / 2.f, -width / 2.f, width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 0.f), VEC3(0.f, 0.f, 1.f) });
	vertices.push_back({ VEC3(width / 2.f,  width / 2.f, width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 1.f), VEC3(0.f, 0.f, 1.f) });
	//vertices.push_back({ VEC3( width/2,  width/2, width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, 1.f), VEC2(1.f, 1.f) });
	vertices.push_back({ VEC3(-width / 2.f,  width / 2.f, width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 1.f), VEC3(0.f, 0.f, 1.f) });
	//vertices.push_back({ VEC3(-width/2, -width/2, width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, 1.f), VEC2(0.f, 0.f) });

	// 4, 5, 6; 6, 7, 4;

	// Left face.
	vertices.push_back({ VEC3(-width / 2.f,  width / 2.f,  width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 0.f), VEC3(-1.f, 0.f, 0.f) });
	vertices.push_back({ VEC3(-width / 2.f,  width / 2.f, -width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 1.f), VEC3(-1.f, 0.f, 0.f) });
	vertices.push_back({ VEC3(-width / 2.f, -width / 2.f, -width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 1.f), VEC3(-1.f, 0.f, 0.f) });
	//vertices.push_back({ VEC3(-width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(-1.f, 0.f, 0.f), VEC2(0.f, 1.f) });
	vertices.push_back({ VEC3(-width / 2.f, -width / 2.f,  width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 0.f), VEC3(-1.f, 0.f, 0.f) });
	//vertices.push_back({ VEC3(-width/2,  width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(-1.f, 0.f, 0.f), VEC2(1.f, 0.f) });

	// 8, 9, 10; 10, 11, 8;

	// Right face.
	vertices.push_back({ VEC3(width / 2.f,  width / 2.f,  width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 0.f), VEC3(1.f, 0.f, 0.f) });
	vertices.push_back({ VEC3(width / 2.f, -width / 2.f, -width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 1.f), VEC3(1.f, 0.f, 0.f) });
	vertices.push_back({ VEC3(width / 2.f,  width / 2.f, -width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 1.f), VEC3(1.f, 0.f, 0.f) });
	//vertices.push_back({ VEC3(width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(1.f, 0.f, 0.f), VEC2(0.f, 1.f) });
	//vertices.push_back({ VEC3(width/2,  width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(1.f, 0.f, 0.f), VEC2(1.f, 0.f) });
	vertices.push_back({ VEC3(width / 2.f, -width / 2.f,  width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 0.f), VEC3(1.f, 0.f, 0.f) });

	// 12, 13, 14; 13, 12, 15;

	// Bottom face.
	vertices.push_back({ VEC3(-width / 2.f, -width / 2.f, -width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 1.f), VEC3(0.f, -1.f, 0.f) });
	vertices.push_back({ VEC3(width / 2.f, -width / 2.f, -width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 1.f), VEC3(0.f, -1.f, 0.f) });
	vertices.push_back({ VEC3(width / 2.f, -width / 2.f,  width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 0.f), VEC3(0.f, -1.f, 0.f) });
	//vertices.push_back({ VEC3( width/2, -width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, -1.f, 0.f), VEC2(1.f, 0.f) });
	vertices.push_back({ VEC3(-width / 2.f, -width / 2.f,  width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 0.f), VEC3(0.f, -1.f, 0.f) });
	//vertices.push_back({ VEC3(-width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, -1.f, 0.f), VEC2(0.f, 1.f) });

	// 16, 17, 18; 18, 19, 16;

	// Up face.
	vertices.push_back({ VEC3(-width / 2.f,  width / 2.f, -width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 1.f), VEC3(0.f, 1.f, 0.f) });
	vertices.push_back({ VEC3(width / 2.f,  width / 2.f,  width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 0.f), VEC3(0.f, 1.f, 0.f) });
	vertices.push_back({ VEC3(width / 2.f,  width / 2.f, -width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(1.f, 1.f), VEC3(0.f, 1.f, 0.f) });
	//vertices.push_back({ VEC3( width/2,  width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 1.f, 0.f), VEC2(1.f, 0.f) });
	//vertices.push_back({ VEC3(-width/2,  width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 1.f, 0.f), VEC2(0.f, 1.f) });
	vertices.push_back({ VEC3(-width / 2.f,  width / 2.f,  width / 2.f), COLOR4(1.f, 1.f, 1.f, 1.f), VEC2(0.f, 0.f), VEC3(0.f, 1.f, 0.f) });

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

void jkGeometry::mCreateGrid(const float width, const float height, UINT m, UINT n, 
	std::vector<Vertex>& vertices, std::vector<UINT>& indices, const float textureDensity)
{
	/*
	* width, height : mesh size.
	* m, n : grid size.
	* textureDensity : density to cover mesh to grid.
	*/
	float stepW = width / (m - 1);
	float stepH = height / (n - 1);

	vertices.resize(m * n);

	for (size_t i = 0; i < n; i++)
	{
		float h = (i * stepH) - (height / 2);
		for (size_t j = 0; j < m; j++)
		{
			float w = (j * stepW) - (width / 2);
			vertices[i * m + j].pos = { w, 0.f, h };
			vertices[i * m + j].texcoord = {stepH*i * textureDensity, stepW*j * textureDensity };
			vertices[i * m + j].normal = { 0.f, 1.f, 0.f };
		}
	}

	for (size_t i = 0; i < n-1; i++)
	{
		for (size_t j = 0; j < m-1; j++)
		{
			// upper triangle.
			indices.push_back(i * m + j);
			indices.push_back((i + 1) * m + j + 1);
			indices.push_back(i * m + j + 1);

			// down triangle.
			indices.push_back(i * m + j);
			indices.push_back((i + 1) * m + j);
			indices.push_back((i + 1) * m + j + 1);

		}
	}
}

void jkGeometry::mCreateSphere(const float radius, const VEC3& color, 
	std::vector<Vertex>& vertices, std::vector<UINT>& indices)
{
	int slice = 100;//Slices in longitude.
	int stack = 100;//Slices in latitude.

	//int vertsPerRow = slice + 1;
	int vertsPerRow = slice;
	int nRows = stack - 1;

	Vertex topV, bottomV;
	topV.pos = VEC3(0.f, radius, 0.f);
	topV.color = COLOR4(color, 1.f);
	topV.normal = VEC3(0.f, 1.f, 0.f);
	vertices.push_back(topV);//Top vertex.
	for (int i = 1; i <= nRows; ++i)
	{
		float phy = PI * i / stack;
		float tmpRadius = radius * sin(phy);
		for (int j = 0; j < vertsPerRow; ++j)
		{
			float theta = 2 * PI * j / slice;

			float x = tmpRadius * cos(theta);
			float y = radius * cos(phy);
			float z = tmpRadius * sin(theta);

			Vertex v;
			v.pos = VEC3(x, y, z);
			v.normal = v.pos.Normalize();
			v.color = COLOR4(color, 1.f);

			vertices.push_back(v);

		}
	}
	bottomV.pos = VEC3(0.f, -radius, 0.f);
	bottomV.color = COLOR4(color, 1.f);
	bottomV.normal = VEC3(0.f, -1.f, 0.f);
	vertices.push_back(bottomV);//Top vertex.

	//Top.
	for (int i = 1; i <= vertsPerRow; ++i)
	{
		indices.push_back(i);
		indices.push_back(0);
		indices.push_back((i + 1) > vertsPerRow ? 1 : i + 1);
	}

	for (int i = 0; i < nRows-1; ++i)
	{
		for (int j = 0; j < vertsPerRow; ++j)
		{
			indices.push_back(i * vertsPerRow + j);
			indices.push_back(i * vertsPerRow+1 + j);
			indices.push_back((i + 1) * vertsPerRow + j);
			indices.push_back((i + 1) * vertsPerRow + j);
			indices.push_back(i * vertsPerRow+1 + j);
			indices.push_back((i + 1) * vertsPerRow + 1 + j);
		}
	}

	//Bottom.
	for (int i = (stack - 2) * vertsPerRow; i <= (stack - 1) * vertsPerRow; ++i)
	{
		indices.push_back(i);
		indices.push_back((i == (stack - 1) * vertsPerRow) ? (stack - 2) * vertsPerRow : i + 1);
		indices.push_back(vertices.size() - 1);
	}
}	


void jkGeometry::MakeHeightMapMesh(const float width, const float height, UINT m, UINT n, 
	const std::vector<float>& heightInfo, jkMesh* gridMesh, const float textureDensity)
{
	/*
	* width, height : mesh size to create.
	* m, n : mesh grid size.
	* heightInfo : mesh height values.
	* textureDensity : density to cover mesh.
	* gridMesh : return created mesh.
	*/

	if (heightInfo.size() != m * n)// Height map size don't match grid size, do nothing.
		return;

	mCreateGrid(width, height, m, n, gridMesh->mVertexBuffer, gridMesh->mIndexBuffer, textureDensity);

	for (size_t i = 0; i < heightInfo.size(); i++)
	{
		gridMesh->mVertexBuffer.at(i).pos.y = heightInfo.at(i);
	}

	for (size_t i = 0; i+2 < gridMesh->mIndexBuffer.size(); i+=3)
	{
		auto i1 = gridMesh->mIndexBuffer.at(i);
		auto i2 = gridMesh->mIndexBuffer.at(i+1);
		auto i3 = gridMesh->mIndexBuffer.at(i+2);

		auto v1 = gridMesh->mVertexBuffer.at(i1);
		auto v2 = gridMesh->mVertexBuffer.at(i2);
		auto v3 = gridMesh->mVertexBuffer.at(i3);

		v1.normal = (v3.pos - v1.pos).CrossProduct(v2.pos - v1.pos);
		v3.normal = v2.normal = v1.normal;
		/*v2.normal = (v1.pos - v2.pos).CrossProduct(v3.pos - v2.pos);
		v3.normal = (v2.pos - v3.pos).CrossProduct(v1.pos - v3.pos);*/
	}
}

void jkGeometry::MakeHeightMapMesh(const float width, const float height, UINT m, UINT n,
	const Array2D_f& heightInfo, jkMesh* gridMesh, const float textureDensity)
{
	/*
	* width, height : mesh size to create.
	* m, n : mesh grid size.
	* heightInfo : mesh height values.
	* textureDensity : density to cover mesh.
	* gridMesh : return created mesh.
	*/
	/*assert(m == heightInfo[0].size());
	assert(n == heightInfo.size());*/

	// Create grid mesh.
	mCreateGrid(width, height, m, n, gridMesh->mVertexBuffer, gridMesh->mIndexBuffer, textureDensity);
	int id = 0;
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			int hi = float(i) / n * heightInfo.size();
			int hj = float(j) / m * heightInfo[i].size();
			gridMesh->mVertexBuffer.at(id).pos.y = heightInfo.at(hi).at(hj);
		}
	}

	// Calc normal.
	for (size_t i = 0; i + 2 < gridMesh->mIndexBuffer.size(); i += 3)
	{
		auto i1 = gridMesh->mIndexBuffer.at(i);
		auto i2 = gridMesh->mIndexBuffer.at(i + 1);
		auto i3 = gridMesh->mIndexBuffer.at(i + 2);

		auto v1 = gridMesh->mVertexBuffer.at(i1);
		auto v2 = gridMesh->mVertexBuffer.at(i2);
		auto v3 = gridMesh->mVertexBuffer.at(i3);

		v1.normal = (v3.pos - v1.pos).CrossProduct(v2.pos - v1.pos);
		v3.normal = v2.normal = v1.normal;
	}
}

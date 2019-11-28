#include "Geometry.h"

void jkGeometry::CreateCube(const float width, std::vector<Vertex>& vertices, std::vector<UINT>& indices)
{
	////////////////////////////////////////////////////////////
	// Build 6 faces, each face with 2 triangles(6 vertices) all in anticlockwise.

	vertices.clear();
	indices.clear();

	// Front face.
	vertices.push_back({ VEC3(-width / 2, -width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, -1.f), VEC2(0.f, 0.f) });// Bottom-left.
	vertices.push_back({ VEC3(width / 2,  width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, -1.f), VEC2(1.f, 1.f) });// Top-right.
	vertices.push_back({ VEC3(width / 2, -width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, -1.f), VEC2(1.f, 0.f) });// Bottom-right.
	//vertices.push_back({ VEC3( width/2,  width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, -1.f), VEC2(1.f, 1.f) });// Top-right.
	//vertices.push_back({ VEC3(-width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, -1.f), VEC2(0.f, 0.f) });// Bottom-left.
	vertices.push_back({ VEC3(-width / 2,  width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, -1.f), VEC2(0.f, 1.f) });// Top-left.

	// Clockwise: 0, 1, 2; 1, 0, 3;

	// Back face.
	vertices.push_back({ VEC3(-width / 2, -width / 2, width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, 1.f), VEC2(0.f, 0.f) });
	vertices.push_back({ VEC3(width / 2, -width / 2, width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, 1.f), VEC2(1.f, 0.f) });
	vertices.push_back({ VEC3(width / 2,  width / 2, width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, 1.f), VEC2(1.f, 1.f) });
	//vertices.push_back({ VEC3( width/2,  width/2, width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, 1.f), VEC2(1.f, 1.f) });
	vertices.push_back({ VEC3(-width / 2,  width / 2, width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, 1.f), VEC2(0.f, 1.f) });
	//vertices.push_back({ VEC3(-width/2, -width/2, width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 0.f, 1.f), VEC2(0.f, 0.f) });

	// 4, 5, 6; 6, 7, 4;

	// Left face.
	vertices.push_back({ VEC3(-width / 2,  width / 2,  width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(-1.f, 0.f, 0.f), VEC2(1.f, 0.f) });
	vertices.push_back({ VEC3(-width / 2,  width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(-1.f, 0.f, 0.f), VEC2(1.f, 1.f) });
	vertices.push_back({ VEC3(-width / 2, -width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(-1.f, 0.f, 0.f), VEC2(0.f, 1.f) });
	//vertices.push_back({ VEC3(-width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(-1.f, 0.f, 0.f), VEC2(0.f, 1.f) });
	vertices.push_back({ VEC3(-width / 2, -width / 2,  width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(-1.f, 0.f, 0.f), VEC2(0.f, 0.f) });
	//vertices.push_back({ VEC3(-width/2,  width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(-1.f, 0.f, 0.f), VEC2(1.f, 0.f) });

	// 8, 9, 10; 10, 11, 8;

	// Right face.
	vertices.push_back({ VEC3(width / 2,  width / 2,  width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(1.f, 0.f, 0.f), VEC2(1.f, 0.f) });
	vertices.push_back({ VEC3(width / 2, -width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(1.f, 0.f, 0.f), VEC2(0.f, 1.f) });
	vertices.push_back({ VEC3(width / 2,  width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(1.f, 0.f, 0.f), VEC2(1.f, 1.f) });
	//vertices.push_back({ VEC3(width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(1.f, 0.f, 0.f), VEC2(0.f, 1.f) });
	//vertices.push_back({ VEC3(width/2,  width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(1.f, 0.f, 0.f), VEC2(1.f, 0.f) });
	vertices.push_back({ VEC3(width / 2, -width / 2,  width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(1.f, 0.f, 0.f), VEC2(0.f, 0.f) });

	// 12, 13, 14; 13, 12, 15;

	// Bottom face.
	vertices.push_back({ VEC3(-width / 2, -width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, -1.f, 0.f), VEC2(0.f, 1.f) });
	vertices.push_back({ VEC3(width / 2, -width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, -1.f, 0.f), VEC2(1.f, 1.f) });
	vertices.push_back({ VEC3(width / 2, -width / 2,  width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, -1.f, 0.f), VEC2(1.f, 0.f) });
	//vertices.push_back({ VEC3( width/2, -width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, -1.f, 0.f), VEC2(1.f, 0.f) });
	vertices.push_back({ VEC3(-width / 2, -width / 2,  width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, -1.f, 0.f), VEC2(0.f, 0.f) });
	//vertices.push_back({ VEC3(-width/2, -width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, -1.f, 0.f), VEC2(0.f, 1.f) });

	// 16, 17, 18; 18, 19, 16;

	// Up face.
	vertices.push_back({ VEC3(-width / 2,  width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 1.f, 0.f), VEC2(0.f, 1.f) });
	vertices.push_back({ VEC3(width / 2,  width / 2,  width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 1.f, 0.f), VEC2(1.f, 0.f) });
	vertices.push_back({ VEC3(width / 2,  width / 2, -width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 1.f, 0.f), VEC2(1.f, 1.f) });
	//vertices.push_back({ VEC3( width/2,  width/2,  width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 1.f, 0.f), VEC2(1.f, 0.f) });
	//vertices.push_back({ VEC3(-width/2,  width/2, -width/2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 1.f, 0.f), VEC2(0.f, 1.f) });
	vertices.push_back({ VEC3(-width / 2,  width / 2,  width / 2), COLOR4(1.f, 1.f, 1.f, 1.f), VEC3(0.f, 1.f, 0.f), VEC2(0.f, 0.f) });

	// 20, 21, 22; 21, 20, 23;

	indices.insert(indices.end(), { 0,1,2,1,0,3,4,5,6,6,7,4,8,9,10,10,11,8,12,13,14,13,12,15,16,17,18,18,19,16,20,21,22,21,20,23 });
}

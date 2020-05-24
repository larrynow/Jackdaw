#pragma once
#ifndef JKGEOMETRY_H_
#define JKGEOMETRY_H_
#include"Types.h"
#include"Mesh.h"
#include"Model.h"
#include"Entity.h"

class jkGeometry
{
public:
	
	// Return a cube's vertex positions.
	static float* CreateCubeVertices();

	static void MakeHeightMapMesh(const float width, const float height,
		UINT m, UINT n, const std::vector<float>& heightInfo, jkMesh* gridMesh, const float textureDensity=0.1f);

	static void MakeHeightMapMesh(const float width, const float height,
		UINT m, UINT n, const Array2D_f& heightInfo, jkMesh* gridMesh, const float textureDensity = 0.1f);
	
	///////////////////////////////////////////////
	// Functions to create entity with mesh.

	inline static jkEntity* GetCubeEntity(const VEC3& pos, const float width = 1.f)
	{
		auto mesh = std::make_shared<jkMesh>();
		mCreateCube(width, mesh->mVertexBuffer, mesh->mIndexBuffer);
		std::shared_ptr<jkModel> model(new jkModel(mesh));

		return new jkEntity(pos, model);
	}
	
	inline static jkEntity* GetSphereEntity(const VEC3& pos, const VEC3& color = VEC3(1.f), const float radius = 0.5f)
	{
		auto mesh = std::make_shared<jkMesh>();
		mCreateSphere(radius, color, mesh->mVertexBuffer, mesh->mIndexBuffer);
		std::shared_ptr<jkModel> model(new jkModel(mesh));

		return new jkEntity(pos, model);
	}

	inline static jkEntity* GetPlaneEnity(const VEC3& pos, 
		const float width, const float height,
		UINT m, UINT n, const float textureDensity = 1.0f)
	{
		auto mesh = std::make_shared<jkMesh>();
		mCreateGrid(width, height, m, n, mesh->mVertexBuffer, mesh->mIndexBuffer, textureDensity);
		std::shared_ptr<jkModel> model(new jkModel(mesh));

		return new jkEntity(pos, model);
	}

private:

	//////////////////////////////////////
	// Functions to create meshe vertices.

	static void mCreateCube(const float width,
		std::vector<Vertex>& vertices, std::vector<UINT>& indices);// Create a Cube's vertices and indices.

	static void mCreateGrid(const float width, const float height, UINT m, UINT n,
		std::vector<Vertex>& vertices, std::vector<UINT>& indices, const float textureDensity);

	static void mCreateSphere(const float radius, const VEC3& color,
		std::vector<Vertex>& vertices, std::vector<UINT>& indices);
};

#endif // !JKGEOMETRY_H_


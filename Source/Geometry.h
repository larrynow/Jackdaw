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
	
	// Geometry model which has only one mesh.

	/*inline jkEntity* GetStaticMesh() {
		return new jkEntity();
	}*/

	///////////////////////////////////
	// Functions to create meshes.

	inline static void MakeCubeMesh(jkMesh* mesh, const float width = 1.f) { 
		mCreateCube(width, mesh->mVertexBuffer, mesh->mIndexBuffer); 
	}
	
	// Return a cube's vertex positions.
	static float* CreateCubeVertices();

	inline static void MakePlaneMesh(jkMesh* gridMesh, const float width, const float height,
		UINT m, UINT n, const float meshDensity = 1.0f)
	{
		mCreateGrid(width, height, m, n, gridMesh->mVertexBuffer, gridMesh->mIndexBuffer, meshDensity);
	};

	static void MakeHeightMapMesh(const float width, const float height,
		UINT m, UINT n, const std::vector<float>& heightInfo, jkMesh* gridMesh, const float meshScale=0.1f);
	
	inline static void MakeCuboidMesh(jkMesh* mesh,
		const float length, const float width, const float height)
	{}

	inline static void MakeSphereMesh(jkMesh* mesh, const VEC3& color, const float radius = 0.5f)
	{
		mCreateSphere(radius, color, mesh->mVertexBuffer, mesh->mIndexBuffer);
	}

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
		UINT m, UINT n, const float meshDensity = 1.0f)
	{
		auto mesh = std::make_shared<jkMesh>();
		mCreateGrid(width, height, m, n, mesh->mVertexBuffer, mesh->mIndexBuffer, meshDensity);
		std::shared_ptr<jkModel> model(new jkModel(mesh));

		return new jkEntity(pos, model);
	}

private:

	static void mCreateCube(const float width,
		std::vector<Vertex>& vertices, std::vector<UINT>& indices);// Create a Cube's vertices and indices.

	static void mCreateGrid(const float width, const float height, UINT m, UINT n,
		std::vector<Vertex>& vertices, std::vector<UINT>& indices, const float meshDensity);

	static void mCreateSphere(const float radius, const VEC3& color,
		std::vector<Vertex>& vertices, std::vector<UINT>& indices);
};

#endif // !JKGEOMETRY_H_


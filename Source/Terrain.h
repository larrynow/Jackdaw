#pragma once
#ifndef JKTERRAIN
#define JKTERRAIN
#include"Types.h"
#include"Mesh.h"

namespace jkTerrain
{
	struct Block
	{
		enum{MAX_LOD_LEVEL = 5};
		int blockIdX;
		int blockIdY;
		std::vector<UINT> blockLODIndices[MAX_LOD_LEVEL];
		std::vector<Block*> adjacentBlocks;
		VEC3 Position;
		VEC2 XBoundary;
		VEC2 YBoundary;
		UINT LOD;

		//Quad-tree.
		Block* pChildLeftTop;
		Block* pChildLeftBottom;
		Block* pChildRightTop;
		Block* pChildRightBottom;

		std::vector<MAT4> InstanceMatrices;
		std::vector<VEC3> GrassPositions;
	};
	struct Tile
	{
		Tile() { pFullTerrainMesh = new jkMesh(); }

		jkMesh* pFullTerrainMesh;//delete latter.
		std::vector<Block*> mBlocks;
		VEC3 center;
		VEC2 size;
		VEC2 xBoundary;
		VEC2 yBoundary;
		UINT vertexNumX;
		UINT vertexNumY;
	};

	// Load terrain height info from a image.
	void LoadHeightInfo(const std::string& heightMapFile, ImageFormat& format,
		std::vector<float>& heightInfo, const float heightScale);

	// Create a tile-terrain from a heightmap image.
	void LoadTerrain(const std::string& heightMapFile, const VEC3& terrainSize,
		Tile* terrainTile, const VEC3& centerPosition = { 0.f, 0.f, 0.f });

	// Create grass positions in terrain.
	void CreateTurf(Tile* terrainTile);

	// Create vegetation.
	void CreateVegetation(Tile* terrainTile);

	class jkTerrainManager
	{
	public:

		jkTerrainManager() : mTile(nullptr){};

		inline auto GetMesh() { return mTile->pFullTerrainMesh; }

		std::vector<UINT>& GetTerrainData() { return mCurrentIndices; };

		void InitializeBlocks(UINT blockNumX, UINT blockNumY);

		inline void CreateTile() 
		{ 
			mTile = new Tile(); 
		}

		Tile* GetTile() { return mTile; }

		void TileUpdate(const VEC3& viewPos, const MAT4& frustum);

		inline void CreateTerrain(const std::string& heightMapFile, const VEC3& terrainSize,
			const VEC3& centerPosition = { 0.f, 0.f, 0.f })
		{
			LoadTerrain(heightMapFile, terrainSize, mTile, centerPosition);
		}
		
		void CreateInstances();//Create instances positions in every block(matrice format).

		void CreatePositions();//Create positions to grow plants.

		std::vector<MAT4>& GetInstanceMatrices();//Return current rendering instances.

		std::vector<VEC3>& GetPositions();//Return positions for surrounding rendering.
		
	private:

		Tile* mTile;
		Block* m_pCurrentBlock;

		std::vector<UINT> mCurrentIndices;

		std::vector<Texture*> mTerrainTexs;

		Texture* mTerrainBlendTex;

		void mUpdateBlockLOD(const VEC3& viewPos);//Update all the blocks LOD;

	};

	
};

#endif // !JKTERRAIN
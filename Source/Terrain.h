#pragma once
#ifndef JKTERRAIN
#define JKTERRAIN
#include"Types.h"

namespace jkTerrain
{
	struct Block
	{
		int blockIdX;
		int blockIdY;
	};
	struct Tile
	{
		//std::vector<Vertex>* pVertices;
		jkMesh* pMesh;
		std::vector<Block>* pBlocks;
		VEC3 center;
		float scale;
	};

	// Load terrain height info from a image.
	void LoadHeightInfo(const std::string& heightMapFile, ImageFormat& format,
		std::vector<float>& heightInfo, const float& heightScale);

	// Create a tile-terrain from a heightmap image.
	void CreateTerrain(const std::string& heightMapFile, const VEC3& terrainSize,
		Tile* terrainTile, const VEC3& centerPosition = { 0.f, 0.f, 0.f });

	// Create grass positions in terrain.
	void CreateTurf(Tile* terrainTile);

	// Create vegetation.
	void CreateVegetation(Tile* terrainTile);
};

#endif // !JKTERRAIN
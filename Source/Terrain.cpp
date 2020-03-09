#include "Terrain.h"
#include "ResourceManager.h"
#include "Geometry.h"
#include <unordered_set>
#include"Algorithm.h"

using namespace jkTerrain;

void jkTerrain::LoadHeightInfo(const std::string& heightMapFile, ImageFormat& format,
	std::vector<float>& heightInfo, const float& heightScale)
{
	/*
	* LoadHeightInfo
	* Load height info from a height map file.
	* Height value calculated with height map gray value v as v * height_scale.
	*/

	auto imgData = jkResourceManager::ImportImage(heightMapFile, format);
	int imgDataSize = std::strlen((const char*)imgData);

	int heightInfoCount = format.height * format.width;
	heightInfo.resize(heightInfoCount);

	for (int i = 0; i < heightInfoCount; i++)
	{
		float gray;
		if (format.channels == 3 || format.channels == 4)
		{
			UINT b, g, r;
			r = imgData[i * format.channels];
			g = imgData[i * format.channels + 1];
			b = imgData[i * format.channels + 2];

			gray = RGB2Gray(r, g, b);
		}

		heightInfo[i] = gray / 255.f * heightScale;
	}

	delete imgData;
}

void jkTerrain::LoadTerrain(const std::string& heightMapFile, 
	const VEC3& terrainSize, Tile* terrainTile, const VEC3& centerPosition)
{
	// Load terrain height info(y-value).
	std::vector<float> heightInfo;
	ImageFormat format;

	LoadHeightInfo(heightMapFile, format, heightInfo, terrainSize.y);

	jkGeometry::MakeHeightMapMesh(terrainSize.x, terrainSize.z, 
		format.width, format.height, heightInfo, terrainTile->pFullTerrainMesh);
	
	terrainTile->size.x = terrainSize.x;
	terrainTile->size.y = terrainSize.z;
	terrainTile->center = { 0.f, 0.f, 0.f };
	terrainTile->xBoundary = { centerPosition.x-terrainSize.x / 2.f, 
		centerPosition.x + terrainSize.x / 2.f };
	terrainTile->yBoundary = { centerPosition.z -terrainSize.z / 2.f,
		centerPosition.z + terrainSize.z / 2.f };
	terrainTile->vertexNumX = format.width;
	terrainTile->vertexNumY = format.height;
}

void jkTerrainManager::InitializeBlocks(UINT blockNumX, UINT blockNumY)
{
	if (blockNumX > mTile->vertexNumX)
		blockNumX = mTile->vertexNumX;
	if (blockNumY > mTile->vertexNumY)
		blockNumY = mTile->vertexNumY;

	int blockIndicesNum = mTile->pFullTerrainMesh->mVertexBuffer.size() 
		/ blockNumX / blockNumY * 3;
	int blockIndicesNumX = mTile->vertexNumX / blockNumX;
	int blockIndicesNumY = mTile->vertexNumY / blockNumY;

	mTile->mBlocks.reserve(blockNumX * blockNumY);
	for (size_t i = 0; i < blockNumX * blockNumY; i++)
	{
		mTile->mBlocks.push_back(new Block());
	}

	for (UINT block_id = 0; block_id < mTile->mBlocks.size(); block_id++)
	{
		auto block = mTile->mBlocks.at(block_id);
		block->blockIdX = block_id % blockNumX;
		block->blockIdY = block_id / blockNumX;

		//Find adjecent blocks.
		if (block_id / blockNumX != 0)//Not in first row.
		{
			block->adjacentBlocks.push_back(mTile->mBlocks.at(block_id - blockNumX));//Up.
			if (block_id % blockNumX != 0)//Not in first col.
			{
				block->adjacentBlocks.push_back(mTile->mBlocks.at(block_id - blockNumX - 1));//Up-left.
				block->adjacentBlocks.push_back(mTile->mBlocks.at(block_id - 1));//Left.
			}
			if (block_id % blockNumX != blockNumX - 1)//Not in last col.
			{
				block->adjacentBlocks.push_back(mTile->mBlocks.at(block_id - blockNumX + 1));//Up-right.
				block->adjacentBlocks.push_back(mTile->mBlocks.at(block_id + 1));//right.
			}
		}
		if (block_id / blockNumX != blockNumY-1)//Not in last row.
		{
			block->adjacentBlocks.push_back(mTile->mBlocks.at(block_id + blockNumX));
			if (block_id % blockNumX != 0)//Not in first col.
			{
				block->adjacentBlocks.push_back(mTile->mBlocks.at(block_id + blockNumX - 1));//Down-left.
			}
			if (block_id % blockNumX != blockNumX - 1)//Not in last col.
			{
				block->adjacentBlocks.push_back(mTile->mBlocks.at(block_id + blockNumX + 1));//Down-right.
			}
		}

		//Calculate block boundary.
		block->XBoundary.x = (mTile->xBoundary.y - mTile->xBoundary.x) / blockNumX * block->blockIdX
			+ mTile->xBoundary.x;
		block->XBoundary.y = (mTile->xBoundary.y - mTile->xBoundary.x) / blockNumX * (block->blockIdX+1)
			+ mTile->xBoundary.x;
		block->YBoundary.x = (mTile->yBoundary.y - mTile->yBoundary.x) / blockNumY * block->blockIdY
			+ mTile->yBoundary.x;
		block->YBoundary.y = (mTile->yBoundary.y - mTile->yBoundary.x) / blockNumY * (block->blockIdY+1)
			+ mTile->yBoundary.x;

		//Select indices for each LOD.
		int step = 1;
		for (size_t lod = 0; lod < 3; lod++)
		{
			block->blockLODIndices[lod].reserve(blockIndicesNumX * blockIndicesNumY * 6);

			for (int bi = 0; bi < blockIndicesNumY ; bi += (step))
			{
				for (int bj = 0; bj < blockIndicesNumX ; bj += (step))
				{
					//From block index to global index.

					int i = block->blockIdY * blockIndicesNumY + bi;
					int j = block->blockIdX * blockIndicesNumX + bj;

					if (i + step > mTile->vertexNumX - 1 || j + step > mTile->vertexNumY - 1) continue;
					// upper triangle.
					block->blockLODIndices[lod].push_back(i * mTile->vertexNumX + j);
					block->blockLODIndices[lod].push_back((i+step) * mTile->vertexNumX + j);
					block->blockLODIndices[lod].push_back(i * mTile->vertexNumX + j + step);

					// down triangle.
					block->blockLODIndices[lod].push_back(i * mTile->vertexNumX + j + step);
					block->blockLODIndices[lod].push_back((i + step) * mTile->vertexNumX + j);
					block->blockLODIndices[lod].push_back((i + step) * mTile->vertexNumX + j + step);

				}
			}
			step *= 2;
		}
	}
}

void jkTerrain::jkTerrainManager::TileUpdate(const VEC3& viewPos, const MAT4& frustum)
{
	/////////////////////////
	// Update curent block.
	if (!m_pCurrentBlock)
	{
		for (auto aBlock : mTile->mBlocks)
		{
			if ((viewPos.x < aBlock->XBoundary.y) && (viewPos.x >= aBlock->XBoundary.x)
				&& (viewPos.z < aBlock->YBoundary.y) && (viewPos.z >= aBlock->YBoundary.x))
			{
				m_pCurrentBlock = aBlock;
				break;
			}
		}
	}
	else if ((viewPos.x < m_pCurrentBlock->XBoundary.y) && (viewPos.x >= m_pCurrentBlock->XBoundary.x)
		&& (viewPos.z < m_pCurrentBlock->YBoundary.y) && (viewPos.z >= m_pCurrentBlock->YBoundary.x))
	{
		// Unchange, return.
		return;
	}
	else
	{
		for (auto adBlock : m_pCurrentBlock->adjacentBlocks)
		{
			if ((viewPos.x < adBlock->XBoundary.y) && (viewPos.x >= adBlock->XBoundary.x)
				&& (viewPos.z < adBlock->YBoundary.y) && (viewPos.z >= adBlock->YBoundary.x))
			{
				m_pCurrentBlock = adBlock;
				break;
			}
		}
	}

	/////////////////////////
	// Update block LOD.

	if (m_pCurrentBlock)
	{
		mUpdateBlockLOD();
	}
	mCurrentIndices.clear();
	for (auto aBlock : mTile->mBlocks)
	{
		auto& lodIndices = aBlock->blockLODIndices[aBlock->LOD];
		mCurrentIndices.insert(mCurrentIndices.end(),
			lodIndices.begin(), lodIndices.end());
	}
}

void jkTerrain::jkTerrainManager::CreateTerrain(const std::string& heightMapFile, const VEC3& terrainSize, const VEC3& centerPosition)
{
	LoadTerrain(heightMapFile, terrainSize, mTile, centerPosition);
}

void jkTerrain::jkTerrainManager::CreateInstances()
{
	//Create instance matrices for dynamic terrain surroundings.

	for (auto block : mTile->mBlocks)
	{
		std::vector<UINT> ids;
		//Use a low LOD meshes to select all triangles.
		for (int i = 0; i < block->blockLODIndices[2].size();i+=3)
		{
			ids.push_back(i);
		}

		std::vector<UINT> selected_id;//Select a triangle.
		jkAlgorithm::RandomSelect(ids, selected_id, (int)ids.size() * 0.05);

		for (auto id : selected_id)
		{
			//Sample from triangle.
			auto p1 = mTile->pFullTerrainMesh->mVertexBuffer.at(block->blockLODIndices[2].at(id)).pos;
			auto p2 = mTile->pFullTerrainMesh->mVertexBuffer.at(block->blockLODIndices[2].at(id+1)).pos;
			auto p3 = mTile->pFullTerrainMesh->mVertexBuffer.at(block->blockLODIndices[2].at(id+2)).pos;
			auto sample_pos = jkAlgorithm::RandomSampleFromTriangle(p1, p2, p3);
			//sample_pos = p1;
			MAT4 model;

			//Scale.
			float scale = (rand() % 20) / 100.0f + 0.50f;//[0.50 to 0.70].
			MakeScaleMatrix(model, scale);

			MakeTranslateMatrix(model, 
				sample_pos.x, sample_pos.y, sample_pos.z);

			//Rotate.
			float rotAngleRadian = GetRadian(float(rand() % 360));
			model = model * Matrix_RotationXYZ(rotAngleRadian, rotAngleRadian, rotAngleRadian);

			block->InstanceMatrices.push_back(model.T());
		}
		
	}
}

void jkTerrain::jkTerrainManager::CreatePositions()
{
	for (auto block : mTile->mBlocks)
	{
		std::vector<UINT> ids;
		//Use a low LOD meshes to select all triangles.
		for (int i = 0; i < block->blockLODIndices[2].size(); i += 3)
		{
			ids.push_back(i);
		}

		std::vector<UINT> selected_id;//Select a triangle.
		jkAlgorithm::RandomSelect(ids, selected_id, (int)ids.size() * 0.3);

		for (auto id : selected_id)
		{
			//Sample from triangle.
			auto p1 = mTile->pFullTerrainMesh->mVertexBuffer.at(block->blockLODIndices[2].at(id)).pos;
			auto p2 = mTile->pFullTerrainMesh->mVertexBuffer.at(block->blockLODIndices[2].at(id + 1)).pos;
			auto p3 = mTile->pFullTerrainMesh->mVertexBuffer.at(block->blockLODIndices[2].at(id + 2)).pos;
			for (size_t i = 0; i < 10; i++)
			{
				auto sample_pos = jkAlgorithm::RandomSampleFromTriangle(p1, p2, p3);

				block->GrassPositions.push_back(sample_pos);
			}
			
		}

	}
}

std::vector<MAT4>& jkTerrain::jkTerrainManager::GetInstanceMatrices()
{
	//m_pCurrentBlock->InstanceMatrices.clear();

	return m_pCurrentBlock->InstanceMatrices;
}

std::vector<VEC3>& jkTerrain::jkTerrainManager::GetPositions()
{
	return m_pCurrentBlock->GrassPositions;
}



void jkTerrain::jkTerrainManager::mUpdateBlockLOD()
{
	m_pCurrentBlock->LOD = 0;//Highest level.
	for (auto block : mTile->mBlocks)
	{
		block->LOD = 2;
	}
	for (auto block : m_pCurrentBlock->adjacentBlocks)
	{
		block->LOD = 1;
	}
}

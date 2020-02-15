#include "Terrain.h"
#include "ResourceManager.h"
#include "Geometry.h"

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

void jkTerrain::CreateTerrain(const std::string& heightMapFile, 
	const VEC3& terrainSize, Tile* terrainTile, const VEC3& centerPosition)
{
	// Load terrain height info(y-value).
	std::vector<float> heightInfo;
	ImageFormat format;

	LoadHeightInfo(heightMapFile, format, heightInfo, terrainSize.y);

	jkMesh* terrainMesh = new jkMesh(centerPosition);
	jkGeometry::MakeHeightMapMesh(terrainSize.x, terrainSize.z, 
		format.width, format.height, heightInfo, terrainMesh);

	terrainTile->pMesh = terrainMesh;
}


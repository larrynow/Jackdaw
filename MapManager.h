#pragma once
#ifndef MAPMANAGER_H_
#define MAPMANAGER_H_

#include<vector>
#include"Map.h"

/*
Load maps for game.
*/

class jkMapManager
{
public:
	static bool LoadMaps(const char* path);
	static jkMap* NextMap();

private:
	std::vector<jkMap*> maps;
};


#endif // !MAPMANAGER_H_
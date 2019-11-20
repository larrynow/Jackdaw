#pragma once
#ifndef MAPMANAGER_H_
#define MAPMANAGER_H_

#include<vector>
#include<string>
#include"Map.h"

/*
Load maps for game.
*/

class jkMapManager
{
public:
	static bool LoadMaps(std::string path);
	static jkMap* NextMap();

private:
	std::vector<jkMap*> maps;
};


#endif // !MAPMANAGER_H_
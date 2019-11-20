#pragma once
#ifndef JKCONTENT_H_
#define JKCONTENT_H_

#include"Devices.h"
#include"Map.h"

class jkFrontendRenderer;
class jkBackendRenderer;

/*
jkContent : a content for engine modules.

Setting up a content for game.
Select both frontend and backend devices.
*/

class jkContent
{
public:
	jkContent();
	~jkContent();

	inline void SelectFrontendDevice(jkFrontendDevice ftDevice = jkFrontendDevice::WIN32) { contentFrontendDevice = ftDevice; };
	inline void SelectBackendDevice(jkBackendDevice bkDevice = jkBackendDevice::OPENGL) { contentBackendDevice = bkDevice; };

	inline void SelectMap(jkMap* map) { currentMap = map; };

	void StartUp();

	void Display();

private:

	jkFrontendDevice contentFrontendDevice;
	jkBackendDevice contentBackendDevice;

	jkFrontendRenderer* frontendRenderer;
	jkBackendRenderer* backendRenderer;

	jkMap* currentMap;
	
};

jkContent::jkContent()
{
}

jkContent::~jkContent()
{
}

#endif	
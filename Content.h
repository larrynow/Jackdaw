#pragma once
#ifndef JKCONTENT_H_
#define JKCONTENT_H_

#include"Types.h"
#include"Devices.h"
#include"Map.h"

class jkFrontendRenderer;
class jkBackendRenderer;
class jkInputManager;

/*
jkContent : a content for engine modules.

Setting up a content for game.
Select both frontend and backend devices.
*/

class jkContent
{

public:
	jkContent() {};
	~jkContent() {};

	inline void SelectFrontendDevice(jkFrontendDevice ftDevice = jkFrontendDevice::WIN_32) { mContentFrontendDevice = ftDevice; };
	inline void SelectBackendDevice(jkBackendDevice bkDevice = jkBackendDevice::OPENGL) { mContentBackendDevice = bkDevice; };

	inline void SelectMap(const jkMap* map) { m_pCurrentMap = map; };

	void Init(UINT width, UINT height);

	void Display();

	void StartUp();

private:

	//////////////////////////////////
	// Devices

	jkFrontendDevice mContentFrontendDevice;
	jkBackendDevice mContentBackendDevice;

	jkFrontendRenderer* m_pFrontendRenderer;
	jkBackendRenderer* m_pBackendRenderer;

	jkMap* m_pCurrentMap;

	jkCharacter* m_pControlledCharacter;
	
	jkInputManager* m_pInputManager;
	//m_pControllActor.

	bool ShouldFinish();
};
#endif	
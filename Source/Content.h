#pragma once
#ifndef JKCONTENT
#define JKCONTENT

#include"Singleton.h"
#include"Types.h"
#include"Devices.h"
#include"InputManager.h"
#include"Clock.h"
#include"Map.h"

class jkFrontendRenderer;
class jkBackendRenderer;
class jkInputManager;
class jkResourceManager;
namespace jkTerrain {
	class jkTerrainManager;
}
//class jkTerrain::jkTerrainTileManager;

/*
jkContent : a content for engine modules.

Setting up a content for game.
Select both frontend and backend devices.
*/

class jkContent : public jkSingleton<jkContent>
{

public:
	jkContent() {};
	~jkContent() {};

	inline void SelectFrontendDevice(jkFrontendDevice ftDevice = jkFrontendDevice::WIN_32) { mContentFrontendDevice = ftDevice; };
	inline void SelectBackendDevice(jkBackendDevice bkDevice = jkBackendDevice::OPENGL) { mContentBackendDevice = bkDevice; };

	void SelectMapWild(jkMap* map);

	void SelectMapIndoor(jkMap* map);

	void Init(UINT width, UINT height);

	void Display();

	void StartUp();

	inline void RegisterInput(jkInput input, std::string input_name) { m_pInputManager->RegisterInput(input, input_name); };

	inline void SetMouseSensi(float value) { m_pInputManager->SetMouseSensitivity(value); }

	void ChangeView();

//private:

	//////////////////////////////////
	// Components.

	jkFrontendDevice mContentFrontendDevice;
	jkBackendDevice mContentBackendDevice;

	jkFrontendRenderer* m_pFrontendRenderer;
	jkBackendRenderer* m_pBackendRenderer;

	jkMap* m_pCurrentMap;

	jkCharacter* m_pControlledCharacter;
	
	jkInputManager* m_pInputManager;

	jkClock* m_pTimer;

	jkTerrain::jkTerrainManager* m_pTerrainTileManager;

	bool mShouldFinish();

	void mPrepareBackendRenderer();

	/////////////////////////////////////
	// Properties.

	union{
		Rect<UINT> mWindowSize;
		struct {
			UINT mWidth;
			UINT mHeight;
		};
	};

	const float mFixedUpdateTime = 0.02f;
	float mSimulatedTime;
};


#endif	
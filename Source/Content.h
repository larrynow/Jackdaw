#pragma once
#ifndef JKCONTENT
#define JKCONTENT

#include"Singleton.h"
#include"Types.h"
#include"Devices.h"
#include"InputManager.h"
#include"Clock.h"
#include"Map.h"

class jkDeviceWindow;
class jkBackendRenderer;
class jkSysInputManager;
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

#define pTimer jkContent::GetInstancePtr()->m_pTimer// Content's mian Timer.
#define iContent jkContent::GetInstance()// Content.

class jkContent : public jkSingleton<jkContent>
{

public:
	jkContent() {};
	~jkContent() {};

	inline void SelectFrontendDevice(jkFrontendDevice ftDevice = jkFrontendDevice::WIN_32) { mContentFrontendDevice = ftDevice; };
	inline void SelectBackendDevice(jkBackendDevice bkDevice = jkBackendDevice::OPENGL) { mContentBackendDevice = bkDevice; };

	void SelectMapWild(jkMap* map);

	void SelectMapIndoor(jkMap* map);

	void SelectMapNew(jkMap* map);

	void Init(UINT width, UINT height);

	void Display();

	void StartUp();

	inline void RegisterInput(jkSysInput input, std::string input_name) { m_pInputManager->RegisterInput(input, input_name); };

	inline void SetMouseSensi(float value) { m_pInputManager->SetMouseSensitivity(value); }

	void ChangeView();

//private:

	//////////////////////////////////
	// Components.

	jkFrontendDevice mContentFrontendDevice;
	jkBackendDevice mContentBackendDevice;

	jkDeviceWindow* m_pDeviceWindow;
	jkBackendRenderer* m_pBackendRenderer;

	jkMap* m_pCurrentMap;

	jkCharacter* m_pControlledCharacter;
	
	jkCamera* m_pCurrentCamera;

	jkSysInputManager* m_pInputManager;

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
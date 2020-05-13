#include "InputManager.h"

unsigned int jkSysInputManager::KeyStatus[512] = {0};
unsigned int jkSysInputManager::ExitStatus = 0;

bool jkSysInputManager::MouseMove = false;

float jkSysInputManager::LastMousePosX=0.f;
float jkSysInputManager::LastMousePosY=0.f;

float jkSysInputManager::MousePosX=0.f;
float jkSysInputManager::MousePosY=0.f;

unsigned int jkSysInputManager::MouseLeftButton = 0;
unsigned int jkSysInputManager::MouseRightButton = 0;

int jkSysInputManager::MouseWheel = 0;

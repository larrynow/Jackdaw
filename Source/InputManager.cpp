#include "InputManager.h"

unsigned int jkInputManager::KeyStatus[512] = {0};
unsigned int jkInputManager::ExitStatus = 0;

bool jkInputManager::MouseMove = false;

float jkInputManager::LastMousePosX=0.f;
float jkInputManager::LastMousePosY=0.f;

float jkInputManager::MousePosX=0.f;
float jkInputManager::MousePosY=0.f;

unsigned int jkInputManager::MouseLeftButton = 0;
unsigned int jkInputManager::MouseRightButton = 0;

int jkInputManager::MouseWheel = 0;

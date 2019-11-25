#pragma once
#ifndef JKINPUTMANAGER_H_
#define JKINPUTMANAGER_H_

#include<unordered_map>
#include<functional>

enum class jkInput
{
	// Keyboard inputs.
	KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M, KEY_N,
	KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
	KEY_TAB, KEY_SHIFT, KEY_CTRL, KEY_SPACE, KEY_ENTER,
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

class jkInputManager
{
	friend class jkContent;

public:

	jkInputManager() {};

	virtual void Listen() = 0;

	void LoadInputTable(const std::string inputTablePath) {};

	virtual int MapKey(jkInput input) = 0;// Map a jkInput(key) to a device key id.

	static unsigned int KeyStatus[512];
	static unsigned int ExitStatus;

private:

	std::unordered_map<jkInput, std::string> input_name_map;// From a user input to a input name.
	//std::unordered_map<std::string, std::function<void()>> input_op_map;// From a input to a operation.

	// Input setting before start up.
	inline void RegisterInput(jkInput input, std::string input_name) { input_name_map.insert(std::make_pair(input, input_name)); };
	//inline void BindInput(std::string input_name, std::function<void()> operation) { input_op_map.insert(std::make_pair(input_name, operation)); };
	// Note, the BindInput should be excuted in actor(or controller) class.




};
#endif // !JKINPUTMANAGER_H_
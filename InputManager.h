#pragma once
#ifndef JKINPUTMANAGER_H_
#define JKINPUTMANAGER_H_

#include<unordered_map>
#include<functional>

enum class jkInput
{
	KEY_W,
};

class jkInputManager
{
	friend class jkContent;

public:

	jkInputManager() {};

	virtual void Listen() = 0;

	virtual int GetInputId(jkInput) = 0;

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
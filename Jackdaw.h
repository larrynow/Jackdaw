#pragma once
#ifndef JACKDAW
#include"Content.h"
#include<string>

////////////////////////////////////////////
// Easy coding.

template<typename T>
inline const char* ToCStr(T value)// WRONG£¡ return array error.                  
{
	std::string s = std::to_string(value);
	//std::cout << std::to_string(value).c_str() << std::endl;
	return s.c_str();
}

#define PRINT(msg) std::cout<<msg<<std::endl

#define pTimer jkContent::GetInstancePtr()->m_pTimer// Content's mian Timer.
#define iContent jkContent::GetInstance()// Content.

#endif // !JACKDAW

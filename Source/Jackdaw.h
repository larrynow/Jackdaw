#pragma once
#ifndef JACKDAW
//#include"Content.h"
#include<string>
#include <assert.h>

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



// A macro to disallow the copy constructor and operator= functions.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;               \
  void operator=(const TypeName&) = delete

#endif // !JACKDAW

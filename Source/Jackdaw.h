#pragma once
#ifndef JACKDAW
//#include"Content.h"
#include<iostream>
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

inline void PRINT() { std::cout << std::endl; }

template< typename First, typename ... Rest>
inline void PRINT(First&& first, Rest&& ... rest)
{
	std::cout << std::forward<First>(first);
	PRINT(std::forward<Rest>(rest)...);
}

// A macro to disallow the copy constructor and operator= functions.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;               \
  void operator=(const TypeName&) = delete

// From a c-style array, get its size.
template<typename T, std::size_t N>
constexpr std::size_t ArraySize(T(&)[N]) noexcept
{
	return N;
}

#endif // !JACKDAW

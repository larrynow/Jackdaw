#pragma once
#ifndef JKBACKENDRENDERER_H_
#define JKBACKENDRENDERER_H_

#include"Types.h"

class jkBackendRenderer
{
public:

	virtual void Clear() = 0;

	virtual inline void SetClearColor(COLOR3 clearColor = { 0.7f, 0.7f, 0.7f }) { mClearColor = clearColor; };

	virtual void StartUp() = 0;
	//virtual void Display() = 0;

private:

	COLOR3 mClearColor;
};

#endif // !JKBACKENDRENDERER_H_




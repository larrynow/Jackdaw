#pragma once
#ifndef JKBACKENDRENDERER_H_
#define JKBACKENDRENDERER_H_

#include"Types.h"

class jkBackendRenderer
{
public:

	void Clear();

	inline void SetClearColor(COLOR3 clearColor) { mClearColor = clearColor; };

	virtual void Display() = 0;

private:

	COLOR3 mClearColor;
};

#endif // !JKBACKENDRENDERER_H_




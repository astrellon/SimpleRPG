#include "Rect.h"


Rect::Rect()
{
	mX = 0;
	mY = 0;
	mWidth = 0;
	mHeight = 0;
}

Rect::Rect(int x, int y, int width, int height)
{
	mX = x;
	mY = y;
	mWidth = width;
	mHeight = height;
}


Rect::~Rect(void)
{
}

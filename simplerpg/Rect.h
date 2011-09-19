#pragma once

#include "Vector2.h"

class Rect
{
public:
	Rect();
	Rect(int x, int y, int width, int height);
	~Rect(void);

	int inline getX(){ return mX; }
	int inline getY(){ return mY; }
	int inline getWidth() { return mWidth; }
	int inline getHeight() { return mHeight; }

	void inline setX(const int &x){ mX = x; }
	void inline setY(const int &y){ mY = y; }
	void setWidth(const int &width);
	void setHeight(const int &height);

	void inline setPosition(const int &x, const int &y) { setX(x); setY(y); }
	void inline setPosition(const Vector2i &position) { setX(position.x); setY(position.y); }

	void inline setSize(const int &width, const int &height) { setWidth(width); setHeight(height); }
	void inline setSize(const Vector2i &size) { setWidth(size.x); setHeight(size.y); }

	void setRect(const int &x, const int &y, const int &width, const int &height);
	void setRect(const Vector2i &position, const Vector2i &size);

protected:
	int mX;
	int mY;
	int mWidth;
	int mHeight;
};


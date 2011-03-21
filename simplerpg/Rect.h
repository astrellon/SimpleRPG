#pragma once
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

	void inline setX(int x){ mX = x; }
	void inline setY(int y){ mY = y; }
	void inline setWidth(int width){ mWidth = width; }
	void inline setHeight(int height){ mHeight = height; }

protected:
	int mX;
	int mY;
	int mWidth;
	int mHeight;
};


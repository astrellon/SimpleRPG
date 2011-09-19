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

void Rect::setWidth(const int &width)
{
	if(width < 0)
	{
		setX(getX() + width);
		mWidth = -width;
	}
	else
	{
		mWidth = width;
	}
}

void Rect::setHeight(const int &height)
{
	if(height < 0)
	{
		setY(getY() + height);
		mHeight = -height;
	}
	else
	{
		mHeight = height;
	}
}

void Rect::setRect(const int &x, const int &y, const int &width, const int &height)
{
	setX(x);
	setY(y);
	setWidth(width);
	setHeight(height);
}

void Rect::setRect(const Vector2i &position, const Vector2i &size)
{
	setX(position.x);
	setY(position.y);
	setWidth(size.x);
	setHeight(size.y);
}
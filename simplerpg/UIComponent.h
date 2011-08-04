#pragma once

#include "curses.h"
#include <vector>

using namespace std;

typedef pair<int, int> int_pair;

class UIComponent
{
public:
	UIComponent();
	~UIComponent();

	virtual void render(bool overrideFormat = false) {}

	inline void setWindow(WINDOW *wnd) { mWnd = wnd; }
	inline WINDOW *getWindow();

	virtual void setX(int x) { mXPosition = x; }
	virtual int getX() { return mXPosition; }

	virtual void setY(int y) { mYPosition = y; }
	virtual int getY() { return mYPosition; }

	virtual void setMaxWidth(int width) { mMaxWidth = width; }
	virtual int getMaxWidth() { return mMaxWidth; }

	virtual void setMaxHeight(int height) { mMaxHeight = height; }
	virtual int getMaxHeight() { return mMaxHeight; }

	virtual void setParent(UIComponent *parent)
	{
		if(parent == this)
		{
			throw "Cannot set parent to itself";
		}
		mParent = parent;
	}
	virtual UIComponent *getParent() { return mParent; }

	virtual void setVisible(bool visible) { mVisible = visible; }
	virtual bool getVisible() { return mVisible; }

	virtual int getMeasuredWidth() { return mMeasuredWidth; }
	virtual int getMeasuredHeight() { return mMeasuredHeight; }

protected:

	int mXPosition;
	int mYPosition;

	int mMaxWidth;
	int mMaxHeight;

	bool mVisible;
	bool mRedisplay;

	int mMeasuredHeight;
	int mMeasuredWidth;

	WINDOW *mWnd;
	UIComponent *mParent;

	virtual bool getNeedsRedisplay() { return mRedisplay; }
	virtual void setNeedsRedisplay()
	{
		if (mParent != NULL)
			mParent->setNeedsRedisplay();
		mRedisplay = true;
	}

	virtual void renderRaw(bool nullRender = false, bool overrideFormat = false)	{}
	virtual void renderChar(int x, int y, char c);
	
};

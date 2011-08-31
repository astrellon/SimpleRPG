#pragma once

#include "UIContainer.h"

class UIScroller : public UIContainer
{
public:
	UIScroller();
	~UIScroller();

	virtual void setScrollX(int scroll) { mScrollXOffset = scroll; }
	virtual int getScrollX() { return mScrollXOffset; }

	virtual void setScrollY(int scroll) { mScrollYOffset = scroll; }
	virtual int getScrollY() { return mScrollYOffset; }

	virtual void scrollX(int scroll) { mScrollXOffset += scroll; }
	virtual void scrollY(int scroll) { mScrollYOffset += scroll; }

	virtual void renderChar(int x, int y, char c);

protected:

	int mScrollXOffset;
	int mScrollYOffset;
};

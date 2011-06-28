#include "UIComponent.h"

UIComponent::UIComponent()
{
	mXPosition = 0;
	mYPosition = 0;

	mMaxWidth = 80;
	mMaxHeight = 25;

	mParent = NULL;
	mWnd = NULL;

	mVisible = true;

	mMeasuredWidth = 1;
	mMeasuredHeight = 1;
}

UIComponent::~UIComponent()
{

}

WINDOW *UIComponent::getWindow()
{
	if(mWnd == NULL && mParent != NULL)
	{
		return mParent->getWindow();
	}
	return mWnd;
}


void UIComponent::renderChar(int x, int y, char c)
{
	if(mParent == NULL)
	{
		WINDOW *wnd = getWindow();
		if(wnd != NULL)
		{
			mvwaddch(wnd, y + getY(), x + getX(), c);
		}
	}
	else
	{
		if(x > getMaxWidth() || y > getMaxHeight())
		{
			return;
		}
		mParent->renderChar(x + getX(), y + getY(), c);
	}
}


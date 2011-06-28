#include "UIScoller.h"

UIScroller::UIScroller() : UIContainer()
{
	mScrollXOffset = 0;
	mScrollYOffset = 0;
}

UIScroller::~UIScroller()
{

}

void UIScroller::renderChar(int x, int y, char c)
{
	int nx = x + getScrollX();
	int ny = y + getScrollY();

	if(nx < 0 || nx >= getMaxWidth() || ny < 0 || ny >= getMaxHeight())
	{
		return;
	}
	
	if(mParent == NULL)
	{
		mvwaddch(getWindow(), ny + getY(), nx + getX(), c);
	}
	else
	{
		UIComponent::renderChar(nx, ny, c);
	}
}
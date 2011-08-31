#include "UISelector.h"

UISelector::UISelector() : UIList()
{
	mSelectedIndex = -1;
	setWrapSelection(true);
}

UISelector::~UISelector()
{

}

void UISelector::renderChild(UIComponent *child, int childIndex, bool overrideFormat)
{
	if(childIndex == mSelectedIndex)
	{
		WINDOW *wnd = getWindow();
		wattron(wnd, COLOR_PAIR(9));
		
		UIList::renderChild(child, childIndex, true);
	}
	else
	{
		UIList::renderChild(child, childIndex, overrideFormat);
	}
}

void UISelector::setSelectionIndex(const int &index)
{
	if(getWrapSelection())
	{
		if(index < 0)
		{
			mSelectedIndex = mChildren.size() - 1;
		}
		else if(index >= (int)mChildren.size())
		{
			mSelectedIndex = 0;
		}
		else
		{
			mSelectedIndex = index;
		}
	}
	else
	{
		if(index < -1)
		{
			mSelectedIndex = -1;
		}
		else if(index >= (int)mChildren.size())
		{
			mSelectedIndex = mChildren.size() - 1;
		}
		else
		{
			mSelectedIndex = index;
		}
	}

	updateScroll();
}

void UISelector::updateScroll()
{
	if(mSelectedIndex < 0)
		return;

	int scroll = mSelectedIndex - getMaxHeight() + 2;
	if (scroll < 0)
		scroll = 0;

	setScrollY(-scroll);
}

UIComponent *UISelector::getSelectedItem()
{
	if(mSelectedIndex < 0)
	{
		return NULL;
	}
	else
	{
		return mChildren[mSelectedIndex];
	}
}
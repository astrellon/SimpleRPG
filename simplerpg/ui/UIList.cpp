#include "UIList.h"

UIList::UIList() : UIScroller()
{

}

UIList::~UIList()
{

}

void UIList::renderChild(UIComponent *child, int childIndex, bool overrideFormat)
{
	child->setY(mMeasuredHeight);
	child->render(overrideFormat);
}
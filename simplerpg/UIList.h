#pragma once

#include "UIScoller.h"

class UIList : public UIScroller
{
public:
	UIList();
	~UIList();

protected:

	virtual void renderChild(UIComponent *child, int childIndex, bool overrideFormat);
};

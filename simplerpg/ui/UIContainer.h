#pragma once

#include <vector>
#include <iostream>

#include "UIComponent.h"

using namespace std;

class UIContainer : public UIComponent
{
public:
	UIContainer();
	~UIContainer();

	virtual void addChild(UIComponent &child);
	virtual void addChild(UIComponent *child);
	virtual bool containsChild(UIComponent &child);
	virtual bool containsChild(UIComponent *child);
	virtual void removeChild(UIComponent &child);
	virtual void removeChild(int index);
	virtual void removeChild(UIComponent *child);
	virtual void removeAllChildren(bool clearChildren = false);

	virtual int getNumChildren() { return mChildren.size(); }

	virtual void render(bool overrideFormat = false);

	virtual int getMeasuredHeight();
	virtual int getMeasuredWidth();

protected:

	virtual vector<UIComponent *>::iterator findChild(UIComponent *child);
	virtual void renderChild(UIComponent *child, int childIndex, bool overrideFormat);
	virtual void renderRaw(bool nullRender = false, bool overrideFormat = false);

	vector<UIComponent *> mChildren;
};

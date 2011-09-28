#include "UIContainer.h"

UIContainer::UIContainer() : UIComponent()
{

}

UIContainer::~UIContainer()
{

}

vector<UIComponent *>::iterator UIContainer::findChild(UIComponent *child)
{
	vector<UIComponent *>::iterator found = mChildren.end();
	for(vector<UIComponent *>::iterator iter = mChildren.begin(); iter != mChildren.end(); ++iter)
	{
		if(*iter == child)
		{
			found = iter;
			break;
		}
	}

	return found;
}

bool UIContainer::containsChild(UIComponent &child)
{
	vector<UIComponent *>::iterator found = findChild(&child);
	return found != mChildren.end();
}

bool UIContainer::containsChild(UIComponent *child)
{
	vector<UIComponent *>::iterator found = findChild(child);
	return found != mChildren.end();
}

void UIContainer::addChild(UIComponent &child)
{
	addChild(&child);
}

void UIContainer::addChild(UIComponent *child)
{
	if(!containsChild(child))
	{
		mChildren.push_back(child);
		child->setParent(this);
	}
}

void UIContainer::removeChild(UIComponent &child)
{
	removeChild(&child);
}

void UIContainer::removeChild(UIComponent *child)
{
	vector<UIComponent *>::iterator found = findChild(child);
	if(found != mChildren.end())
	{
		mChildren.erase(found);
		child->setParent(NULL);
	}
}

void UIContainer::removeChild(int index)
{
	if(index >= 0 && index < (int)mChildren.size())
	{
		removeChild(*mChildren[index]);
	}
}

void UIContainer::render(bool overrideFormat)
{
	if(!getVisible())
	{
		return;
	}

	renderRaw(false, overrideFormat);
}

void UIContainer::renderChild(UIComponent *child, int childIndex, bool overrideFormat)
{
	child->render(overrideFormat);
}

int UIContainer::getMeasuredWidth()
{
	if(mRedisplay)
	{
		renderRaw(true);
	}
	return mMeasuredWidth;
}

int UIContainer::getMeasuredHeight()
{
	if(mRedisplay)
	{
		renderRaw(true);
	}
	return mMeasuredHeight;
}

void UIContainer::renderRaw(bool nullRender, bool overrideFormat)
{
	int i = 0;

	mMeasuredWidth = 0;
	mMeasuredHeight = 0;

	for(vector<UIComponent *>::iterator iter = mChildren.begin(); iter != mChildren.end(); ++iter)
	{
		UIComponent *comp = *iter;
		
		if(!comp->getVisible())
		{
			continue;
		}

		renderChild(comp, i++, overrideFormat);

		mMeasuredWidth = max(mMeasuredWidth, comp->getX() + comp->getMeasuredWidth());
		mMeasuredHeight = max(mMeasuredHeight, comp->getY() + comp->getMeasuredHeight());
	}

	mRedisplay = false;
}

void UIContainer::removeAllChildren(bool clearChildren)
{
	while(mChildren.size() > 0)
	{
		UIComponent *child = mChildren[0];
		removeChild(0);
		if(clearChildren)
		{
			delete child;
		}
	}
}
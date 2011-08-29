#include "UIMenu.h"

UIMenu::UIMenu() : UIList()
{
	mMenuLevel = 0;
}

UIMenu::~UIMenu()
{

}

void UIMenu::setMenuItem(int level, UIComponent *comp)
{
	mMenuItems[level] = comp;
	addChild(comp);
	if (level != getMenuLevel())
	{
		comp->setVisible(false);
	}
}

void UIMenu::setMenuItem(int level, UIComponent &comp)
{
	setMenuItem(level, &comp);
}

void UIMenu::clearMenuItem(int level)
{
	mMenuItems.erase(level);
}

UIComponent *UIMenu::getItemAtLevel(int level)
{
	map<int, UIComponent *>::iterator iter = mMenuItems.find(level);
	if (iter == mMenuItems.end())
	{
		return NULL;
	}
	return iter->second;
}

void UIMenu::setMenuLevel(int level)
{
	if (level == mMenuLevel)
	{
		return;
	}
	mHistory.push_back(mMenuLevel);
	changeMenuLevel(level);
}

int UIMenu::getMenuLevel()
{
	return mMenuLevel;
}

void UIMenu::goBack()
{
	if (mHistory.size() > 0)
	{
		int level = mHistory.back();
		mHistory.pop_back();
		changeMenuLevel(level);
	}
}

void UIMenu::changeMenuLevel(int level)
{
	UIComponent *comp = getItemAtLevel(mMenuLevel);
	if (comp != NULL)
	{
		comp->setVisible(false);
	}
	
	mMenuLevel = level;

	comp = getItemAtLevel(level);
	if (comp != NULL)
	{
		comp->setVisible(true);
	}
}
#pragma once

#include "UIList.h"
#include "UIComponent.h"

#include <map>
#include <vector>

using std::map;
using std::vector;

class UIMenu : public UIList
{
public:
	UIMenu();
	~UIMenu();

	void setMenuItem(int level, UIComponent *comp);
	void setMenuItem(int level, UIComponent &comp);

	UIComponent *getItemAtLevel(int level);

	void clearMenuItem(int level);

	void setMenuLevel(int level);
	int getMenuLevel();

	void goBack();

protected:
	int mMenuLevel;

	map<int, UIComponent *> mMenuItems;
	vector<int> mHistory;

	void changeMenuLevel(int level);
};

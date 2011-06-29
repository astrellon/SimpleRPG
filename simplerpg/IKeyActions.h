#pragma once

#include "UIContainer.h"

class IKeyActions
{
public:
	virtual ~IKeyActions() {};
	virtual void keyActions(const int key) = 0;
	virtual void setupDisplay(UIContainer &hud) = 0;
	virtual void displayActions(UIContainer &hud) = 0;
	virtual void clearDisplay(UIContainer &hud) = 0;
};

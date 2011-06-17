#pragma once

#include "HUD.h"

class IKeyActions
{
public:
	virtual ~IKeyActions() {};
	virtual void keyActions(const int key) = 0;
	virtual void displayActions(HUD &hud) = 0;
	virtual void clearDisplay() = 0;
};

#pragma once

#include "HUD.h"

class IKeyActions
{
public:
	virtual ~IKeyActions() {};
	virtual void keyActions() = 0;
	virtual void displayActions(HUD &hud) = 0;
	virtual void clearDisplay() = 0;
};

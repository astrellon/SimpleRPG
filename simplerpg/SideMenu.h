#pragma once

#include "IKeyActions.h"
#include "HUD.h"

class SideMenu :
	public IKeyActions
{
public:
	SideMenu(void);
	~SideMenu(void);

	virtual void keyActions()
	{
	}

	virtual void displayActions(HUD *hud)
	{
		if(!redisplay)
			return;

		hud->clear();
		hud->writeLine("Hello thar");
		hud->writeLine("<1>c<2>o<3>l<4>o<5>u<6>r<7>s</></></></></></></>");
		hud->writeLine("<4>hell<3>o th</></>ar</> asdf<5>sdfs");
		hud->writeLine("SDJFLK");

		redisplay = false;
	}

	virtual void clearDisplay()
	{
		redisplay = true;
	}

protected:
	bool redisplay;
};


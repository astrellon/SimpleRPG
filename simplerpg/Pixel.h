#pragma once

#include "curses.h"

class Pixel
{
public:
	Pixel(void);
	Pixel(char graphic, int colour, bool bold = false);
	~Pixel(void);

	char graphic;

	bool bold;

	inline int getColour() { return mColour - 1; }

	inline void setColour(int colour)
	{
		colour++;
		if(colour <= 8 && colour > 0)
			mColour = colour;
	}

	inline void render(WINDOW *wnd, int x, int y)
	{
		if(bold)
			wattron(wnd, A_BOLD);
		else
			wattroff(wnd, A_BOLD);

		wattron(wnd, COLOR_PAIR(mColour));
		mvwaddch(wnd, y, x, graphic);
	}

protected:
	int mColour;
};


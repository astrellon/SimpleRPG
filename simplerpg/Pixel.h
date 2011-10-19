#pragma once

#include "ui/ui.hpp"

#include "FormattedFile.h"
#include "FormattedFileIterator.h"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

using boost::algorithm::iequals;
using boost::lexical_cast;

const char *PixelColourNames[];

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

	inline int getFullColour() 
	{
		int colour = getColour();
		if (bold)
		{
			colour |= 0x8;
		}
	
		return colour;
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

	void savePixel(FormattedFile &file);
	void loadPixel(FormattedFileIterator &iter);

	static int getColourFromString(const string &colourName);

protected:
	int mColour;
};


#include "Pixel.h"

extern const char *PixelColourNames[] = { "black", 
#ifdef PDC_RGB
	"red", "green", "blue",
#else
	"blue", "green", "red",
#endif
	"cyan", "magenta", "yellow", "white" };


Pixel::Pixel(void)
{
	bold = false;
	mColour = 7;
	graphic = ' ';
}

Pixel::Pixel(char graphic, int colour, bool bold)
{
	this->graphic = graphic;
	this->bold = bold;
	setColour(colour);
}

Pixel::~Pixel(void)
{
}

void Pixel::savePixel(FormattedFile &file)
{
	const char *colourName = PixelColourNames[getColour()];
	file << colourName << ' ' << (bold ? '1' : '0') << ' ' << graphic << '\n';
}

void Pixel::loadPixel(FormattedFileIterator &iter)
{
	string line = *iter;	++iter;
	char c = line[0];
	if(c >= '0' && c <= '8')
	{
		setColour(c - '0');
	}
	else
	{
		for(int i = 0; i < 8; i++)
		{
			if(iequals(line, PixelColourNames[i]))
			{
				setColour(i);
				break;
			}
		}
	}
	bold = (*iter)[0] == '1';	++iter;
	graphic = (*iter)[0];		++iter;
}
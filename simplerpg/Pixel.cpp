#include "Pixel.h"

extern const char *PixelColourNames[] = { "black", "blue", "green", "cyan", "red", "magenta", "yellow", "white" };

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
	int colour = getColourFromString(*iter);	++iter;
	if (colour >= 0)
	{
		setColour(colour);
	}
	bold = (*iter)[0] == '1';					++iter;
	graphic = (*iter)[0];						++iter;
}

int Pixel::getColourFromString(const string &colourName)
{
	char c = colourName[0];
	if(c >= '0' && c <= '8')
	{
		return c - '0';
	}
	else
	{
		for(int i = 0; i < 8; i++)
		{
			if(iequals(colourName, PixelColourNames[i]))
			{
				return i;
			}
		}
		return -1;
	}
}
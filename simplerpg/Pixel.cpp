#include "Pixel.h"


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
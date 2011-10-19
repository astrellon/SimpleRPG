#include "UIText.h"

UIText::UIText() : UIComponent()
{
	mRedisplay = true;
	mWordWrap = true;
}

UIText::UIText(const char *t) : UIComponent()
{
	mRedisplay = true;
	mWordWrap = true;
	*this << t;
}

UIText::UIText(const string t) : UIComponent()
{
	mRedisplay = true;
	mWordWrap = true;
	*this << t;
}

UIText::~UIText()
{

}

void UIText::pushColour(int colour, bool bold)
{
	colour_t2 c = {bold, colour + 1};
	mColours.push_back(c);
	updateColour();
}
void UIText::popColour()
{
	if(mColours.size() > 0)
	{
		mColours.pop_back();
		updateColour();
	}
}

void UIText::updateColour()
{
	if(mColours.empty())
	{
		wattroff(getWindow(), A_BOLD);
		wattron(getWindow(), COLOR_PAIR(8));
	}
	else
	{
		colour_t2 c = mColours.back();
		if(c.bold)
		{
			wattron(getWindow(), A_BOLD);
		}
		else
		{
			wattroff(getWindow(), A_BOLD);
		}
		wattron(getWindow(), COLOR_PAIR(c.colour));
	}
}

void UIText::render(bool overrideFormat)
{
	if(!getVisible())
		return;

	mRedisplay = false;

	renderRaw(false, overrideFormat);
}

int UIText::getMeasuredHeight()
{
	if(mRedisplay)
		renderRaw(true);

	return mMeasuredHeight;
}

int UIText::getMeasuredWidth()
{
	if(mRedisplay)
		renderRaw(true);

	return mMeasuredWidth;
}

void UIText::renderRaw(bool nullRender, bool overrideFormat)
{
	int y = 0;
	bool inColour = false;
	bool escaped = false;
	int colour = 0;
	char prevChar = '\0';

	string sss = mText.str();
	const char *line = sss.c_str();
	int pos = 0;
	int x = 0;

	mColours.clear();
	if(!overrideFormat)
	{
		updateColour();
	}

	while(line[pos] != '\0')
	{
		char c = line[pos++];

		if(c == '\n')
		{
			y++;
			x = 0;
			continue;
		}

		if(c == '\t')
		{
			// Make the next x value a multiple of 4.
			// By rounding down the next lowest value of 4
			// and then adding 4.
			x = (x & (~3)) + 4;
			if(x >= getMaxWidth() && getWordWrap())
			{
				x = 0;
				if(line[pos] != '\0')
					y++;
				continue;
			}
		}

		if(c == '\\' && line[pos] == '<')
		{
			escaped = true;
			continue;
		}

		if(c == '<' && !escaped)
		{
			inColour = true;
			continue;
		}
		if(inColour)
		{
			if(c != '>')
			{
				if(c == '/')
				{
					if(!overrideFormat)
					{
						popColour();
					}
				}
				else
				{
					if(line[pos] != '>')
					{
						colour = 10 * (c - '0') + (line[pos++] - '0');
					}
					else
					{
						colour = (c - '0');
					}
					if(!overrideFormat)
					{
						// If the colour value is greater than 8 then
						// the colour is brighter bold version.
						bool bold = (colour & 0x8) > 0;
						colour = colour & 0x7;
						pushColour(colour, bold);
					}
				}
			}
			else
			{
				inColour = false;
			}
			continue;
		}

		if(y >= 0)
		{
			if(!nullRender)
				renderChar(x, y, c);
			x++;
		}

		escaped = false;

		int maxWidth = getMaxWidth();
		if(x >= maxWidth && getWordWrap())
		{
			x = 0;
			if(line[pos] != '\0')
				y++;
		}
		prevChar = c;
		mMeasuredWidth = max(mMeasuredWidth, x);
	}

	mMeasuredHeight = y + 1;
}
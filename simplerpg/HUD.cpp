#include "HUD.h"


HUD::HUD(void)
{
	wnd = newwin(25, 20, 0, 60);
	scrollOffset = 0;
}

HUD::~HUD(void)
{
	delwin(wnd);
}

void HUD::render()
{
	wclear(wnd);

	wattroff(wnd, A_BOLD);
	wattron(wnd, COLOR_PAIR(7));

	for(int y = 0; y < wnd->_maxy; y++)
	{
		mvwaddch(wnd, y, 0, '|');
	}

	int y = -scrollOffset;
	bool inColour = false;
	bool escaped = false;
	int colour = 0;

	for(int i = 0; i < (int)log.size(); i++)
	{
		y++;
		if(y > 25)
			break;

		const char *line = log[i].c_str();
		int pos = 0;
		int x = 2;

		colours.clear();
		updateColour();

		while(line[pos] != '\0')
		{
			char c = line[pos++];
			if(!escaped && c == '\\')
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
						popColour();
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
						bool bold = (colour & 0x8) > 0;
						colour = colour & 0x7;
						pushColour(colour, bold);
					}
				}
				else
				{
					inColour = false;
				}
				continue;
			}
			mvwaddch(wnd, y, x++, c);

			escaped = false;

			if(x >= 20)
			{
				x = 2;
				if(line[pos] != '\0')
					y++;
			}
		}
	}

	wrefresh(wnd);
}
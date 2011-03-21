#pragma once

#include <curses.h>
#include <vector>
#include <string>

using namespace std;

struct colour_t
{
	bool bold;
	int colour;
};

class HUD
{
public:
	HUD(void);
	~HUD(void);

	inline WINDOW *getWindow() { return wnd; }

	inline void clear()
	{
		log.clear();
	}

	inline void writeLine(char *line)
	{
		log.push_back(string(line));
	}
	inline void writeLine(string line)
	{
		log.push_back(line);
	}

	void render();

	inline void scrollConsole(int diff)
	{
		scrollOffset += diff;
	}

	inline void setScroll(int s)
	{
		scrollOffset = s;
	}

	inline int getScroll()
	{
		return scrollOffset;
	}

protected:
	WINDOW *wnd;

	vector<string> log;

	int scrollOffset;

	vector<colour_t> colours;

	void pushColour(int colour, bool bold)
	{
		colour_t c = {bold, colour + 1};
		colours.push_back(c);
		updateColour();
	}
	void popColour()
	{
		colours.pop_back();
		updateColour();
	}

	void updateColour()
	{
		if(colours.empty())
		{
			wattroff(wnd, A_BOLD);
			wattron(wnd, COLOR_PAIR(8));
		}
		else
		{
			colour_t c = colours.back();
			if(c.bold)
			{
				wattron(wnd, A_BOLD);
			}
			else
			{
				wattroff(wnd, A_BOLD);
			}
			wattron(wnd, COLOR_PAIR(c.colour));
		}
	}
};


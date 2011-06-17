#pragma once

#include <curses.h>
#include <vector>
#include <string>
#include <sstream>

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
		ss.str("");
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

	inline HUD &operator<<(const char *c)
	{
		ss << c;
		mRedisplay = true;
		return *this;
	}

	inline HUD &operator<<(const string &s)
	{
		ss << s;
		mRedisplay = true;
		return *this;
	}

	inline HUD &operator<<(const int &i)
	{
		ss << i;
		mRedisplay = true;
		return *this;
	}

	inline HUD &operator<<(const unsigned int &i)
	{
		ss << i;
		mRedisplay = true;
		return *this;
	}

	inline HUD &operator<<(const bool &b)
	{
		ss << (b ? "true" : "false");
		mRedisplay = true;
		return *this;
	}

	inline HUD &operator<<(const char &c)
	{
		ss << c;
		mRedisplay = true;
		return *this;
	}

	inline HUD &operator<<(const unsigned char &c)
	{
		ss << c;
		mRedisplay = true;
		return *this;
	}

	inline HUD &operator<<(const float &f)
	{
		ss << f;
		mRedisplay = true;
		return *this;
	}

	inline HUD &operator<<(const double &f)
	{
		ss << f;
		mRedisplay = true;
		return *this;
	}

protected:
	WINDOW *wnd;

	stringstream ss;

	int scrollOffset;
	bool mRedisplay;

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


#pragma once

#include "UIComponent.h"

#include <sstream>

#include <boost\regex.hpp>

struct colour_t2
{
	bool bold;
	int colour;
};

using namespace std;

class UIText : public UIComponent
{
public:
	UIText();
	UIText(const char *t);
	UIText(const string t);
	~UIText();

	virtual void render(bool overrideFormat = false);

	virtual bool getWordWrap() { return mWordWrap; }
	virtual void setWordWrap(bool wrap) { mWordWrap = wrap; }

	virtual void clearText() { mText.str(""); }

	inline UIText &operator<<(const bool &b)
	{
		mText << (b ? "true" : "false");
		newText();
		return *this;
	}

	inline UIText &operator<<(const UIText &t)
	{
		mText << t.mText;
		newText();
		return *this;
	}

	template <class T>
	inline UIText &operator<<(const T &t)
	{
		mText << t;
		newText();
		return *this;
	}
	
	virtual int getMeasuredHeight();
	virtual int getMeasuredWidth();

	string getString()
	{
		return mText.str();
	}

	char getFirstUnformatted()
	{
		string s = getString();
		char c = s[0];
		if(c != '<')
		{
			return c;
		}
		int i = 1;
		while(true)
		{
			if(i >= s.size())
			{
				return '\0';
			}
			c = s[i];
			if(c == '>' && s[i + 1] != '<')
			{
				return s[i + 1];
			}
			i++;
		}
	}

protected:

	bool mWordWrap;

	stringstream mText;
	vector<colour_t2> mColours;

	void pushColour(int colour, bool bold);
	void popColour();
	void updateColour();

	virtual void renderRaw(bool nullRender = false, bool overrideFormat = false);

	void inline newText()
	{
		mRedisplay = true;
		if(mParent != NULL)
		{
			setNeedsRedisplay();
		}
	}
};

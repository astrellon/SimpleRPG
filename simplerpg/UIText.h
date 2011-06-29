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
	
	inline UIText &operator<<(const UIText &t)
	{
		mText << t.mText;
		newText();
		return *this;
	}

	inline UIText &operator<<(const char *c)
	{
		mText << c;
		newText();
		return *this;
	}

	inline UIText &operator<<(const string &s)
	{
		mText << s;
		newText();
		return *this;
	}

	inline UIText &operator<<(const int &i)
	{
		mText << i;
		newText();
		return *this;
	}

	inline UIText &operator<<(const unsigned int &i)
	{
		mText << i;
		newText();
		return *this;
	}

	inline UIText &operator<<(const bool &b)
	{
		mText << (b ? "true" : "false");
		newText();
		return *this;
	}

	inline UIText &operator<<(const char &c)
	{
		mText << c;
		newText();
		return *this;
	}

	inline UIText &operator<<(const unsigned char &c)
	{
		mText << c;
		newText();
		return *this;
	}

	inline UIText &operator<<(const float &f)
	{
		mText << f;
		newText();
		return *this;
	}

	inline UIText &operator<<(const double &f)
	{
		mText << f;
		newText();
		return *this;
	}

	virtual int getMeasuredHeight();
	virtual int getMeasuredWidth();

	string getString()
	{
		return mText.str();
	}

protected:

	bool mWordWrap;

	stringstream mText;
	vector<colour_t2> mColours;

	void pushColour(int colour, bool bold);
	void popColour();
	void updateColour();

	virtual void renderRaw(bool nullRender = false, bool overrideFormat = false);

	//static boost::regex lineBreakup;

	void inline newText()
	{
		mRedisplay = true;
		if(mParent != NULL)
		{
			setNeedsRedisplay();
		}
	}
};

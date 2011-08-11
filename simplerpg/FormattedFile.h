#pragma once

#include <ostream>
#include <fstream>
#include <string>
#include <sstream>

#include <boost/regex.hpp>

using std::ofstream;
using std::string;
using std::stringstream;

class FormattedFile
{
public:
	FormattedFile();
	FormattedFile(string filename);
	~FormattedFile();

	void openFile(string filename);
	void closeFile();
	bool isOpen();
	
	FormattedFile &operator <<(const string &t)
	{
		checkTabs();
		parseString(t);
		return *this;
	}
	FormattedFile &operator <<(const char *t)
	{
		checkTabs();
		string s = string(t);
		parseString(s);
		return *this;
	}
	FormattedFile &operator <<(const char &t)
	{
		checkTabs();
		mStream << t;
		if(t == '\n')
		{
			mNewline = true;
		}
		return *this;
	}
	
	template <class T>
	FormattedFile &operator <<(const T &t)
	{
		checkTabs();
		mStream << t;
		return *this;
	}

	void changeTabLevel(int diff);
	void setTabLevel(int level);
	int getTabLevel();

	string getTabString();

protected:
	ofstream mStream;
	bool mNewline;

	int mTabLevel;
	int mOldTabLevel;
	string mTabString;

	void checkTabs();
	void parseString(string s);
};
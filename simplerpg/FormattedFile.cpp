#include "FormattedFile.h"

FormattedFile::FormattedFile()
{
	mNewline = false;
	mTabLevel = 0;
	mOldTabLevel = -1;
}

FormattedFile::FormattedFile(string filename)
{
	openFile(filename);
	mNewline = false;
	mTabLevel = 0;
	mOldTabLevel = -1;
}

FormattedFile::~FormattedFile()
{
	closeFile();
}

void FormattedFile::openFile(string filename)
{
	closeFile();
	mStream.open(filename);
}

void FormattedFile::closeFile()
{
	if(mStream.is_open())
	{
		mStream.close();
	}
}

void FormattedFile::checkTabs()
{
	if(mNewline)
	{
		mStream << getTabString();
		mNewline = false;
	}
}

void FormattedFile::parseString( string s )
{
	if(mTabLevel > 0)
	{
		string tabs = getTabString();
		size_t pos = 0;
		while((pos = s.find('\n', pos)) != s.npos)
		{
			if(pos + 1 >= s.size())
				break;
			pos++;
			s.insert(pos, tabs);
		}
	}
	if(s[s.size() - 1] == '\n')
	{
		mNewline = true;
	}
	mStream << s;
}

string FormattedFile::getTabString()
{
	if(mOldTabLevel != mTabLevel)
	{
		mTabString = "";
		mTabString.reserve(mTabLevel);
		for(int i = 0; i < mTabLevel; i++)
		{
			mTabString += '\t';
		}
		mOldTabLevel = mTabLevel;
	}
	return mTabString;
}

void FormattedFile::setTabLevel( int level )
{
	mTabLevel = level;
	if (mTabLevel < 0)
		mTabLevel = 0;
}

void FormattedFile::changeTabLevel( int diff )
{
	setTabLevel(getTabLevel() + diff);
}

int FormattedFile::getTabLevel()
{
	return mTabLevel;
}

bool FormattedFile::isOpen()
{
	return mStream.is_open();
}

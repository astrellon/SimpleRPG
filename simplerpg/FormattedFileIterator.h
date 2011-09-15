#pragma once

#include <boost/regex.hpp>
#include <iostream>
#include <fstream>
#include <string>

using std::string;
using std::ifstream;
using std::clog;
using std::endl;

class FormattedFileIterator
{
public:
	FormattedFileIterator(string filename, bool ignoreComments);
	~FormattedFileIterator() {}

	bool isOpen() { return mOpen; }

	FormattedFileIterator &operator ++()
	{
		if(mIgnoreComments)
		{
			nextNonComment();
		}
		else
		{
			++mIter;
		}
		return *this;
	}

	void nextNonComment();

	bool atEnd()
	{
		return mIter == mEnd;
	}

	string operator*()
	{
		return *mIter;
	}

	FormattedFileIterator &operator =(const FormattedFileIterator &iter)
	{
		mIter = iter.mIter;
		fileStr = iter.fileStr;
		mOpen = iter.mOpen;
		mIgnoreComments = iter.mIgnoreComments;
		return *this;
	}

protected:
	boost::sregex_token_iterator mIter;
	boost::sregex_token_iterator mEnd;
	string fileStr;
	bool mIgnoreComments;
	bool mOpen;

	static boost::regex READ_FILE_REGEX;

	bool isComment(const string &line);
};

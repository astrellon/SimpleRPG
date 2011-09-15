#include "FormattedFileIterator.h"

boost::regex FormattedFileIterator::READ_FILE_REGEX(
	// Multi-line comments.
	"(/\\*[^/\\*]*\\*/)|"
	// Single-line comments.
	"(//[^\n]*\n)|"
	// Double quoted strings.
	"(\"[^\"]*\")|"
	// Single quoted strings.
	"('[^']*')|"
	"(\\|?)"
	//"([\\x21-\x7B\x7D\x7E]+)");
	//"([[^\\|]\\S]+)");
	// Everything that's not whitespace.
	"(\\S+)");

FormattedFileIterator::FormattedFileIterator(string filename, bool ignoreComments)
{
	mIgnoreComments = ignoreComments;
	ifstream file(filename);
	if(!file.is_open())
	{
		clog << "Failed to open file " << filename << endl;
		mOpen = false;
	}
	else
	{
		mOpen = true;

		fileStr = string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		mIter = boost::sregex_token_iterator(fileStr.begin(), fileStr.end(), READ_FILE_REGEX, 0);

		if(mIgnoreComments && isComment(*mIter))
		{
			nextNonComment();
		}
	}
}

void FormattedFileIterator::nextNonComment()
{
	if(atEnd())
	{
		return;
	}

	do
	{
		++mIter;
		if(atEnd())
		{
			break;
		}
		string check = *mIter;
		if(!isComment(check))
		{
			break;
		}
			
	} while(true);
}

bool FormattedFileIterator::isComment(const string &line)
{
	if(line.size() >= 2)
	{
		char c0 = line[0];
		char c1 = line[1];
		// All comments start with a / character
		if(c0 != '/')
		{
			return false;
		}
		// All comments have either a / or * as their second character.
		if(c1 != '/' && c1 != '*')
		{
			return false;
		}
		// If we haven't returned by now, it isn't a comment.
		return true;
	}
	// If the line is less than 2 characters long then it cannot be
	// a comment.
	return false;
}
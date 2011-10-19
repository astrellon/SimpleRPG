#pragma once

#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
#include <algorithm>
#ifdef _WIN32
#include <conio.h>
#endif

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "Map.h"
#include "Tile.h"
#include "Game.h"
#include "Animal.h"
#include "Vector2.h"
#include "Matrix3x3.h"
#include "ui/ui.hpp"
#include "Destination.h"
#include "FormattedFile.h"
#include "FormattedFileIterator.h"
#include "Debug.h"

using namespace boost::filesystem;
using boost::lexical_cast;
using boost::algorithm::iequals;

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#ifdef _WIN32
#include <Windows.h>
inline void msleep(int ms)
{
	Sleep(ms);
}
#else
#include <unistd.h>
inline void msleep(int ms)
{
	usleep(ms * 1000);
}
#endif

using namespace std;

typedef struct _folder_entry
{
	int index;
	bool isDir;
	bool isSrm;
	bool isLikelySupported;
	string filename;
	string ext;
	string formattedName;

	_folder_entry(int i, bool d, string f, string ff, string e, bool srm, bool likely)
	{
		index = i;
		isDir = d;
		filename = f;
		formattedName = ff;
		ext = e;
		isSrm = srm;
		isLikelySupported = likely;
	}
} folder_entry;

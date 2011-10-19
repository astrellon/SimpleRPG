#pragma once

#include <map>
#include <vector>
#include <string>

#include "Vector2.h"
#include "FormattedFileIterator.h"
#include "FormattedFile.h"

using namespace std;

#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/math/special_functions/round.hpp>

using boost::math::round;
using boost::algorithm::iequals;
using boost::lexical_cast;

typedef struct
{
	int id;
	Vector2i position;
} Location;

class DayEvents
{
public:
	DayEvents();
	DayEvents(int day);
	~DayEvents();

	int day;

	vector<Location *> locations;
	map<string, int> populations;
	map<string, map<char, int> > deaths;
	map<string, int> births;

	void loadFromFile(FormattedFileIterator &iter);
	void saveToFile(FormattedFile &file);
};

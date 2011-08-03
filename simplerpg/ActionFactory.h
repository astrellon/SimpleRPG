#pragma once

#include <string>
#include <boost/algorithm/string/predicate.hpp>

#include "Action.h"

using std::string;
using boost::algorithm::iequals;

class ActionFactory
{
public:
	static Action *create(string name);
};
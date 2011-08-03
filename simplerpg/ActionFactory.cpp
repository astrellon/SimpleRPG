#include "ActionFactory.h"

Action *ActionFactory::create(string name)
{
	if(iequals(name, "Action"))
	{
		return new Action();
	}
	return NULL;
}
#include "ActionFactory.h"

Action *ActionFactory::create(string name)
{
	if(iequals(name, "Action"))
	{
		return new Action();
	}
	else if(iequals(name, "TargetAction"))
	{
		return new TargetAction();
	}
	return NULL;
}
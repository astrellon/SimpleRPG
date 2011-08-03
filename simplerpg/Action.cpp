#include "Action.h"

extern const char *EntityActionNames[] = { "Null_action", "Idle", "Eat", "Flee", "Attack" };

Action::Action(EntityAction	action)
{
	mAction = action;
}

Action::Action()
{
	mAction = NULL_ACTION;
}

Action::~Action()
{}

#include "TargetAction.h"

TargetAction::TargetAction() : Action()
{
}

TargetAction::TargetAction(EntityAction action) : Action(action)
{
}

Destination *TargetAction::getTarget()
{
	return &mTarget;
}

void TargetAction::loadProperties(FormattedFileIterator &iter)
{
	string propertyName = *iter;
	if(iequals(propertyName, ActionPropertyNames[TARGET]))
	{
		++iter;
		mTarget.loadDestination(iter);
	}
	else
	{
		Action::loadProperties(iter);
	}
}

void TargetAction::saveProperty(const ActionProperty &propertyId, FormattedFile &file)
{
	switch(propertyId)
	{
	case TARGET:
		getTarget()->saveDestination(ActionPropertyNames[TARGET], file);
		break;
	default:
		Action::saveProperty(propertyId, file);
		break;
	}
}

void TargetAction::saveProperties(FormattedFile &file)
{
	Action::saveProperties(file);
	saveProperty(TARGET, file);
}
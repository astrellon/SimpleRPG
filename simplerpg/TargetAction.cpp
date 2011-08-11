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
		string typeCheck = *iter; ++iter;
		if(typeCheck[0] == '@')
		{
			unsigned int following = lexical_cast<unsigned int>(*iter); ++iter;
			mTarget.setEntityId(following);
		}
		else
		{
			float x = lexical_cast<float>(typeCheck);
			float y = lexical_cast<float>(*iter); ++iter;
			mTarget.setLocation(x, y);
		}
	}
	else
	{
		Action::loadProperties(iter);
	}
}

void TargetAction::saveProperty(const ActionProperty &propertyId, FormattedFile &file)
{
	Destination *dest = NULL;
	Vector2f v;
	switch(propertyId)
	{
	case TARGET:
		dest = getTarget();
		if(dest->getEntity() != NULL)
		{
			file << ActionPropertyNames[TARGET] << " @ " << dest->getEntity()->getId() << '\n';
		}
		else
		{
			v = dest->getLocation();
			file << ActionPropertyNames[TARGET] << ' ' << v.x << ' ' << v.y << '\n';
		}
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
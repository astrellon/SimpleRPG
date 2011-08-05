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

void TargetAction::loadProperties(boost::sregex_token_iterator &iter)
{
	string propertyName = *iter;
	if(iequals(propertyName, ActionPropertyNames[TARGET]))
	{
		iter++;
		string typeCheck = *iter++;
		if(typeCheck[0] == '@')
		{
			unsigned int following = lexical_cast<unsigned int>(*iter++);
			mTarget.setEntityId(following);
		}
		else
		{
			float x = lexical_cast<float>(typeCheck);
			float y = lexical_cast<float>(*iter++);
			mTarget.setLocation(x, y);
		}
	}
	else
	{
		Action::loadProperties(iter);
	}
}

void TargetAction::saveProperty(const ActionProperty &propertyId, ofstream &file)
{
	Destination *dest = NULL;
	Vector2f v;
	switch(propertyId)
	{
	case TARGET:
		dest = getTarget();
		if(dest->getEntity() != NULL)
		{
			file << Game::getOutputTabs() << ActionPropertyNames[TARGET] << " @ " << dest->getEntity()->getId() << endl;
		}
		else
		{
			v = dest->getLocation();
			file << Game::getOutputTabs() << ActionPropertyNames[TARGET] << ' ' << v.x << ' ' << v.y << endl;
		}
		break;
	default:
		Action::saveProperty(propertyId, file);
		break;
	}
}

void TargetAction::saveProperties(ofstream &file)
{
	Action::saveProperties(file);
	saveProperty(TARGET, file);
}
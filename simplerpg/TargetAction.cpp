#include "TargetAction.h"

TargetAction::TargetAction() : Action()
{
	mTargetId = -1;
	mTarget = NULL;
}

TargetAction::TargetAction(EntityAction action) : Action(action)
{
	mTargetId = -1;
	mTarget = NULL;
}

GameEntity *TargetAction::getTarget()
{
	if((mTarget == NULL && mTargetId != -1) || (mTarget != NULL && mTarget->getId() != mTargetId))
	{
		setTarget(GameEntity::getEntityById(mTargetId));
	}

	return mTarget;
}

void TargetAction::setTarget(GameEntity *target)
{
	mTarget = target;
	if(target != NULL)
	{
		mTargetId = target->getId();
	}
	else
	{
		mTargetId = -1;
	}
}

unsigned int &TargetAction::getTargetId()
{
	return mTargetId;
}

void TargetAction::setTargetId(const unsigned int &targetId)
{
	if(mTarget != NULL && mTarget->getId() == targetId)
		return;
	else if(mTargetId == targetId)
		return;

	mTarget = NULL;
	mTargetId = targetId;
}

void TargetAction::loadProperties(boost::sregex_token_iterator &iter)
{
	string propertyName = *iter;
	if(iequals(propertyName, ActionPropertyNames[TARGET]))
	{
		iter++;
		setTargetId(lexical_cast<unsigned int>(*iter++));
	}
	else
	{
		Action::loadProperties(iter);
	}
}

void TargetAction::saveProperty(const ActionProperty &propertyId, ofstream &file)
{
	switch(propertyId)
	{
	case TARGET:
		file << Game::getOutputTabs() << EntityActionNames[TARGET] << ' ' << getTargetId() << endl;
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
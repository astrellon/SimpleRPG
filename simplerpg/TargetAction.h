#pragma once

#include "Action.h"
#include "GameEntity.h";

class TargetAction : public Action
{
public:
	TargetAction();
	TargetAction(EntityAction action);
	~TargetAction();

	virtual GameEntity *getTarget();
	virtual void setTarget(GameEntity *target);

	virtual unsigned int &getTargetId();
	virtual void setTargetId(const unsigned int &targetId);

	virtual string getActionType() { return "TargetAction"; }

protected:

	GameEntity *mTarget;
	unsigned int mTargetId;

	// The function which loads each property from the file tokens.
	// Should increment the iterator at least once.
	virtual void loadProperties(boost::sregex_token_iterator &iter);
	// Saves an individual property based on the property ID to the file stream.
	virtual void saveProperty(const ActionProperty &propertyId, ofstream &file);
	// Calls the appropriate functions to save all the properties for this entity.
	virtual void saveProperties(ofstream &file);
};
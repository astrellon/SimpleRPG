#pragma once

#include "Action.h"
#include "Destination.h"
#include "Vector2.h"
#include "GameEntity.h"

class TargetAction : public Action
{
public:
	TargetAction();
	TargetAction(EntityAction action);
	~TargetAction();

	Destination *getTarget();

	virtual string getActionType() { return "TargetAction"; }

protected:
	Destination mTarget;

	// The function which loads each property from the file tokens.
	// Should increment the iterator at least once.
	virtual void loadProperties(boost::sregex_token_iterator &iter);
	// Saves an individual property based on the property ID to the file stream.
	virtual void saveProperty(const ActionProperty &propertyId, ofstream &file);
	// Calls the appropriate functions to save all the properties for this entity.
	virtual void saveProperties(ofstream &file);
};

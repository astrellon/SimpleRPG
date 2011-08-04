#pragma once

#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <map>

#include <boost/regex.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "Game.h"

using namespace std;

using boost::algorithm::iequals;
using boost::lexical_cast;

enum EntityAction { IDLE, EAT, FLEE, ATTACK, SLEEP, BREED, MAX_ID };
enum ActionProperty { ACTION, COMPLETE, STEP, TARGET };

class Action
{
public:
	Action();
	Action(EntityAction action);
	~Action();

	virtual void setAction(EntityAction action) { mAction = action; }
	virtual EntityAction getAction() { return mAction; }

	virtual bool getCompleted() { return mCompleted; }
	virtual void setCompleted(bool completed) { mCompleted = completed; }

	virtual int getStep() { return mStep; }
	virtual void setStep(int step) { mStep = step; }
	virtual void nextStep() { mStep++; }

	// Loads an entity from tokens split up from a file. Entity must at least increment the iterator
	// if it does not load anything.
	virtual void loadFromFile(boost::sregex_token_iterator &iter);
	// Saves the entity to a file stream. Must be in the same format that can be tokenized and
	// loaded using the loadFromFile function.
	virtual void saveToFile(ofstream &file);

	virtual string getActionType() { return "Action"; }

	static map<string, EntityAction> EntityActionLookup;
	static const char *EntityActionNames[];
	static const char *ActionPropertyNames[];
	static const float EntityActionPriority[];

protected:

	EntityAction mAction;
	bool mCompleted;
	int mStep;

	virtual void init();

	// The function which loads each property from the file tokens.
	// Should increment the iterator at least once.
	virtual void loadProperties(boost::sregex_token_iterator &iter);
	// Saves an individual property based on the property ID to the file stream.
	virtual void saveProperty(const ActionProperty &propertyId, ofstream &file);
	// Calls the appropriate functions to save all the properties for this entity.
	virtual void saveProperties(ofstream &file);

private:

	static bool initLookupMap;
};
#pragma once

#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <map>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "Game.h"
#include "FormattedFile.h"
#include "FormattedFileIterator.h"

using namespace std;

using boost::algorithm::iequals;
using boost::lexical_cast;

enum EntityAction { IDLE, EAT, FLEE, ATTACK, SLEEP, BREED, MOVE, MAX_ID };
enum ActionProperty { ACTION, COMPLETED, STEP, TARGET, COMPLETED_TIME };

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
	
	virtual string getCompletedTime() { return mCompletedTime; }
	virtual void setCompletedTime(string time) { mCompletedTime = time; }

	virtual void setCompleted(bool completed, string time) 
	{
		setCompleted(completed); 
		setCompletedTime(time); 
	}

	virtual int getStep() { return mStep; }
	virtual void setStep(int step) { mStep = (step > 0 ? step : 0); }
	virtual void nextStep() { mStep++; }
	virtual void prevStep() { mStep = ((mStep <= 1) ? 0 : mStep - 1); }

	// Loads an entity from tokens split up from a file. Entity must at least increment the iterator
	// if it does not load anything.
	virtual void loadFromFile(FormattedFileIterator &iter);
	// Saves the entity to a file stream. Must be in the same format that can be tokenized and
	// loaded using the loadFromFile function.
	virtual void saveToFile(FormattedFile &file);

	virtual string getActionType() { return "Action"; }

	static map<string, EntityAction> EntityActionLookup;
	static const char *EntityActionNames[];
	static const char *ActionPropertyNames[];
	static const float EntityActionPriority[];

protected:

	EntityAction mAction;
	bool mCompleted;
	int mStep;
	string mCompletedTime;

	virtual void init();

	// The function which loads each property from the file tokens.
	// Should increment the iterator at least once.
	virtual void loadProperties(FormattedFileIterator &iter);
	// Saves an individual property based on the property ID to the file stream.
	virtual void saveProperty(const ActionProperty &propertyId, FormattedFile &file);
	// Calls the appropriate functions to save all the properties for this entity.
	virtual void saveProperties(FormattedFile &file);

private:

	static bool initLookupMap;
};
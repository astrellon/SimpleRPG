#include "Action.h"

map<string, EntityAction> Action::EntityActionLookup;
const char *Action::EntityActionNames[] = { "Idle", "Eat", "Flee", "Attack", "Sleep", "Breed", "Move", "Invalid_action" };
const char *Action::ActionPropertyNames[] = { "action", "completed", "step", "target", "completed_time" };
const float Action::EntityActionPriority[] = { 0.0f, 0.8f, 1.0f, 0.5f, 0.7f, 0.4f, 0.0f, 0.0f };

bool Action::initLookupMap = false;

Action::Action(EntityAction	action)
{
	mAction = action;
	init();
}

Action::Action()
{
	mAction = MAX_ID;
	init();
}

Action::~Action()
{}

void Action::init()
{
	mStep = 0;
	mCompleted = false;
	mCompletedTime = "0 0";

	if(!initLookupMap)
	{
		EntityActionLookup[EntityActionNames[IDLE]] = IDLE;
		EntityActionLookup[EntityActionNames[EAT]] = EAT;
		EntityActionLookup[EntityActionNames[FLEE]] = FLEE;
		EntityActionLookup[EntityActionNames[ATTACK]] = ATTACK;
		EntityActionLookup[EntityActionNames[SLEEP]] = SLEEP;
		EntityActionLookup[EntityActionNames[BREED]] = BREED;
		EntityActionLookup[EntityActionNames[MOVE]] = MOVE;
		EntityActionLookup[EntityActionNames[MAX_ID]] = MAX_ID;
		initLookupMap = true;
	}
}

void Action::loadFromFile(FormattedFileIterator &iter)
{
	while(!iter.atEnd())
	{
		string line = *iter;
		if(iequals(line, "end"))
		{
			++iter;
			break;
		}
		loadProperties(iter);
	}

	clog << "Finished loading " << getActionType() << endl;
}

void Action::saveToFile(FormattedFile &file)
{
	file << getActionType() << '\n';
	file.changeTabLevel(1);
	saveProperties(file);
	file.changeTabLevel(-1);
	file << "end\n";
}

void Action::loadProperties(FormattedFileIterator &iter)
{
	string propertyName = *iter;	++iter;
	if(iequals(propertyName, ActionPropertyNames[ACTION]))
	{
		setAction(EntityActionLookup[*iter]); ++iter;
	}
	else if(iequals(propertyName, ActionPropertyNames[COMPLETED]))
	{
		setCompleted(lexical_cast<bool>(*iter)); ++iter;
	}
	else if(iequals(propertyName, ActionPropertyNames[STEP]))
	{
		setStep(lexical_cast<int>(*iter));	++iter;
	}
	else if(iequals(propertyName, ActionPropertyNames[COMPLETED_TIME]))
	{
		setCompletedTime(*iter);	++iter;
	}
	else
	{
		clog << "Unable to load unknown property '" << propertyName << '\'' << endl;
	}
}

void Action::saveProperty(const ActionProperty &propertyId, FormattedFile &file)
{
	switch(propertyId)
	{
	case ACTION:
		file << ActionPropertyNames[ACTION] << ' ' << EntityActionNames[getAction()] << '\n';
		break;
	case COMPLETED:
		file << ActionPropertyNames[COMPLETED] << ' ' << (getCompleted() ? '1' : '0') << '\n';
		break;
	case STEP:
		file << ActionPropertyNames[STEP] << ' ' << getStep() << '\n';
		break;
	case COMPLETED_TIME:
		file << ActionPropertyNames[COMPLETED_TIME] << ' ' << getCompletedTime() << '\n';
		break;
	default:
		clog << "Unable to save unknown action property " << propertyId << endl;
		break;
	}
}

void Action::saveProperties(FormattedFile &file)
{
	saveProperty(ACTION, file);
	saveProperty(COMPLETED, file);
	saveProperty(COMPLETED_TIME, file);
	saveProperty(STEP, file);
}
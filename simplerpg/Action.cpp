#include "Action.h"

map<string, EntityAction> Action::EntityActionLookup;
const char *Action::EntityActionNames[] = { "Idle", "Eat", "Flee", "Attack", "Sleep", "Breed", "Invalid_action" };
const char *Action::ActionPropertyNames[] = { "action", "complete", "step", "target" };
const float Action::EntityActionPriority[] = { 0.0f, 0.8f, 1.0f, 0.5f, 0.7f, 0.4f, 0.0f };

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

	if(!initLookupMap)
	{
		EntityActionLookup[EntityActionNames[IDLE]] = IDLE;
		EntityActionLookup[EntityActionNames[EAT]] = EAT;
		EntityActionLookup[EntityActionNames[FLEE]] = FLEE;
		EntityActionLookup[EntityActionNames[ATTACK]] = ATTACK;
		EntityActionLookup[EntityActionNames[SLEEP]] = SLEEP;
		EntityActionLookup[EntityActionNames[BREED]] = BREED;
		EntityActionLookup[EntityActionNames[MAX_ID]] = MAX_ID;
		initLookupMap = true;
	}
}

void Action::loadFromFile(boost::sregex_token_iterator &iter)
{
	boost::sregex_token_iterator end;

	while(iter != end)
	{
		string line = *iter;
		if(iequals(line, "end"))
		{
			iter++;
			break;
		}
		loadProperties(iter);
	}

	cout << "Finished loading " << getActionType() << endl;
}

void Action::saveToFile(FormattedFile &file)
{
	file << getActionType() << '\n';
	file.changeTabLevel(1);
	saveProperties(file);
	file.changeTabLevel(-1);
	file << "end\n";
}

void Action::loadProperties(boost::sregex_token_iterator &iter)
{
	string propertyName = *iter++;
	if(iequals(propertyName, ActionPropertyNames[ACTION]))
	{
		setAction(EntityActionLookup[*iter++]);
	}
	else if(iequals(propertyName, ActionPropertyNames[COMPLETE]))
	{
		setCompleted(lexical_cast<bool>(*iter++));
	}
	else if(iequals(propertyName, ActionPropertyNames[STEP]))
	{
		setStep(lexical_cast<int>(*iter++));
	}
	else
	{
		cout << "Unable to load unknown property '" << propertyName << '\'' << endl;
	}
}

void Action::saveProperty(const ActionProperty &propertyId, FormattedFile &file)
{
	switch(propertyId)
	{
	case ACTION:
		file << ActionPropertyNames[ACTION] << ' ' << EntityActionNames[getAction()] << '\n';
		break;
	case COMPLETE:
		file << ActionPropertyNames[COMPLETE] << ' ' << (getCompleted() ? '1' : '0') << '\n';
		break;
	case STEP:
		file << ActionPropertyNames[STEP] << ' ' << getStep() << '\n';
		break;
	default:
		cout << "Unable to save unknown action property " << propertyId << endl;
		break;
	}
}

void Action::saveProperties(FormattedFile &file)
{
	saveProperty(ACTION, file);
	saveProperty(COMPLETE, file);
	saveProperty(STEP, file);
}
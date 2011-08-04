#include "Action.h"

map<string, EntityAction> Action::EntityActionLookup;
const char *Action::EntityActionNames[] = { "Idle", "Eat", "Flee", "Attack", "Invalid_action" };
const char *Action::ActionPropertyNames[] = { "action", "complete", "step" };

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
}

void Action::saveToFile(ofstream &file)
{
	file << Game::getOutputTabs() << getActionType() << endl;
	Game::changeOutputFileLevel(1);
	saveProperties(file);
	Game::changeOutputFileLevel(-1);
	file << Game::getOutputTabs() << "end" << endl;
}

void Action::loadProperties(boost::sregex_token_iterator &iter)
{
	string propertyName = *iter++;
	if(iequals(propertyName, ActionPropertyNames[ACTION]))
	{
		string action = *iter++;
		EntityAction a = EntityActionLookup[action];
		setAction(a);

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

void Action::saveProperty(const ActionProperty &propertyId, ofstream &file)
{
	switch(propertyId)
	{
	case ACTION:
		file << Game::getOutputTabs() << ActionPropertyNames[ACTION] << ' ' << EntityActionNames[getAction()] << endl;
		break;
	case COMPLETE:
		file << Game::getOutputTabs() << ActionPropertyNames[COMPLETE] << ' ' << (getCompleted() ? '1' : '0') << endl;
		break;
	case STEP:
		file << Game::getOutputTabs() << ActionPropertyNames[STEP] << ' ' << getStep() << endl;
		break;
	default:
		cout << "Unable to save unknown action property " << propertyId << endl;
		break;
	}
}

void Action::saveProperties(ofstream &file)
{
	saveProperty(ACTION, file);
	saveProperty(COMPLETE, file);
	saveProperty(STEP, file);
}
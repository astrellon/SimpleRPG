#pragma once

enum EntityAction { NULL_ACTION, IDLE, EAT, FLEE, ATTACK };

const char *EntityActionNames[];

class Action
{
public:
	Action(EntityAction action);
	Action();
	~Action();

	EntityAction getAction() { return mAction; }

	bool getCompleted() { return mCompleted; }
	void setCompleted(bool completed) { mCompleted = completed; }

	int getStep() { return mStep; }
	void setStep(int step) { mStep = step; }
	void nextStep() { mStep++; }

protected:

	EntityAction mAction;
	bool mCompleted;
	int mStep;
};
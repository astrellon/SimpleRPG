#pragma once

#include <iostream>
#include "curses.h"
#include <math.h>

#include <boost/regex.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "Map.h"
#include "Pixel.h"
#include "Vector2.h"
#include "Matrix3x3.h"
#include "GameMath.h"
#include "IKeyActions.h"
#include "UIText.h"
#include "UIContainer.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

using namespace std;
using namespace boost::algorithm;

#define STATE_IDLE		0
#define STATE_MOVING	1

#define PROPERTY_FACING			0
#define PROPERTY_POSITION		1
#define PROPERTY_DESTINATION	2
#define PROPERTY_NAME			3
#define PROPERTY_GRAPHIC		4

class Game;

class GameEntity : public IKeyActions
{
public:
	GameEntity(Game *game);
	~GameEntity(void);

	friend class Game;

	virtual Pixel getGraphic() { return mGraphic; }
	virtual void setGraphic(Pixel graphic) { mGraphic = graphic; }

	virtual MathType getX() { return mTransform.zx; }
	virtual MathType getY() { return mTransform.zy; }

	virtual Vector2 getPosition() { return Vector2(mTransform.zx, mTransform.zy); }

	virtual void setFacing(MathType facing) { mTransform.setRotation(facing); }
	virtual MathType getFacing() { return mTransform.getAngle(); }

	virtual void move(MathType dx, MathType dy, bool inObjectSpace = true);
	virtual void move(Vector2 v, bool inObjectSpace = true);
	virtual void turn(MathType angle);

	virtual Matrix3x3 *getTransform() { return &mTransform; }

	virtual void update(float dt) {}
	virtual void render(Rect screenSize, WINDOW *wnd);

	virtual void loadFromFile(boost::sregex_token_iterator &iter);
	virtual void saveToFile(ofstream &file);
	
	virtual void keyActions(const int key) {}
	virtual void setupDisplay(UIContainer &hud)
	{
		hud.removeAllChildren(false);

		mHudText = new UIText();

		mHudText->setY(1);
		hud.addChild(*mHudText);

	}
	virtual void displayActions(UIContainer &hud)
	{
		if(!mRedisplay)
			return;

		mHudText->clearText();
		*mHudText << "Entity: " << getEntityName() << '\n';
		*mHudText << "Facing: " << getFacing() << '\n';

		mRedisplay = false;
	}
	virtual void clearDisplay(UIContainer &hud)
	{
		hud.removeChild(*mHudText);
		mRedisplay = true;

		delete mHudText;
	}

	virtual void setEntityName(string name) { mName = name; }
	virtual string getEntityName() { return mName; }

	virtual string getEntityType() { return "GameEntity"; }

protected:
	Pixel mGraphic;
	Game* mGame;

	bool mRedisplay;

	UIText *mHudText;

	Matrix3x3 mTransform;

	string mName;
	
	virtual void loadProperties(boost::sregex_token_iterator &iter);
	virtual void saveProperty(const int &propertyId, ofstream &file);
	virtual void saveProperties(ofstream &file);

	virtual void onAddedToGame() {}
	
};

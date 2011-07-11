#pragma once

#include <iostream>
#include "curses.h"
#include <math.h>

#include <boost/regex.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "Pixel.h"
#include "GameMath.h"
#include "IKeyActions.h"
#include "UIText.h"
#include "UIComponent.h"
#include "Rect.h"

using namespace std;
using namespace boost::algorithm;
using boost::lexical_cast;

// Define different states that the entities can be in.
#define STATE_IDLE		0
#define STATE_MOVING	1
#define STATE_HUNGRY	2

// Different properties that can be used during loading/saving.
// Not all entities will have these.
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

	// Returns the current graphic at the time.
	virtual Pixel getGraphic();
	// Sets the current graphic to use.
	virtual void setGraphic(Pixel graphic);

	// Returns the x position of the entity.
	virtual float getX();
	// Returns the y position of the entity.
	virtual float getY();

	// Returns the Vector2 representing the entity.
	virtual Vector2f getPosition();

	// Returns the facing of the entity in radians.
	// Value of 0 indicates pointing along the x axis.
	virtual double getFacing();
	// Sets the facing of the entity in radians.
	// Value of 0 indicates pointing along the x axis.
	virtual void setFacing(double facing);
	
	// Moves the entity by the vector {dx, dy}.
	// If inObjectSpace is true then the vector is transformed into object space.
	virtual void move(float dx, float dy, bool inObjectSpace = true);
	// Moves the entity by the vector v.
	// If inObjectSpace is true then the vector is transformed into object space.
	virtual void move(Vector2f v, bool inObjectSpace = true);
	// Turns the facing of the entity by the given angle in radians.
	virtual void turn(double angle);

	// Returns the Matrix3x3 used to represent the transform of this entity.
	virtual Matrix3x3f *getTransform();

	// Called once a frame with the time elapsed since the last time the update function was called.
	virtual void update(float dt) {}
	// Renders the entity into the given screen rectangle (which acts as a camera) and the correct curses WINDOW.
	virtual void render(Rect screenSize, WINDOW *wnd);

	// Loads an entity from tokens split up from a file. Entity must at least increment the iterator
	// if it does not load anything.
	virtual void loadFromFile(boost::sregex_token_iterator &iter);
	// Saves the entity to a file stream. Must be in the same format that can be tokenized and
	// loaded using the loadFromFile function.
	virtual void saveToFile(ofstream &file);
	
	// If this entity currently has focus, this function is called with the last key that was pressed.
	// This can be called multiple times a frame if multiple keys have been pressed.
	virtual void keyActions(const int key) {}
	// Called when this entity gets focus.
	virtual void setupDisplay(UIContainer &hud);
	// Called when this entity has focus and needs to display something to the HUD.
	virtual void displayActions(UIContainer &hud);
	// Called when this entity looses focus.
	virtual void clearDisplay(UIContainer &hud);

	// Sets the name of the entity. This is a trivial name.
	virtual void setEntityName(string name) { mName = name; }
	// Returns the name of the entity.
	virtual string getEntityName() { return mName; }

	// Returns the type of the entity. This is used by the load and saving functions to determine
	// the type entity that needs to be saved/loaded.
	virtual string getEntityType() { return "GameEntity"; }

protected:
	Pixel mGraphic;
	Game* mGame;

	bool mRedisplay;

	UIText *mHudText;

	Matrix3x3f mTransform;

	string mName;
	
	// The function which loads each property from the file tokens.
	// Should increment the iterator at least once.
	virtual void loadProperties(boost::sregex_token_iterator &iter);
	// Saves an individual property based on the property ID to the file stream.
	virtual void saveProperty(const int &propertyId, ofstream &file);
	// Calls the appropriate functions to save all the properties for this entity.
	virtual void saveProperties(ofstream &file);

	// Call when the entity is added into a Game.
	virtual void onAddedToGame() {}
	
};

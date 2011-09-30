#pragma once

#include <iostream>
#include <math.h>
#include <map>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/math/special_functions/round.hpp>

#include "Pixel.h"
#include "GameMath.h"
#include "IKeyActions.h"
#include "ui/ui.hpp"
#include "Rect.h"
#include "Action.h"
#include "ActionFactory.h"
#include "FormattedFile.h"
#include "FormattedFileIterator.h"

using std::map;

using boost::math::round;
using boost::algorithm::iequals;
using boost::lexical_cast;

// Different properties that can be used during loading/saving.
// Not all entities will have these.
enum EntityProperty { ID, FACING, POSITION, DESTINATION, NAME, GRAPHIC, HEALTH,
	STRENGTH, DEXTERITY, INTELLIGENCE, RUNNING_SPEED, WALKING_SPEED, TURNING_SPEED,
	ENTITY_SIZE, ENTITY_MASS, DIET, DAMAGE_BASE, AMOUNT_EATEN, CURRENT_ACTION, 
	ACTION_HISTORY, ATTACK_RATE, ATTACK_COOLDOWN, ENERGY, REST_ENERGY_PER_DAY, 
	SPECIES, SPECIES_ALIGNMENT, SIGHT_RADIUS, ATTACKED_BY, ATTACKED_BY_COOLDOWN,
	HUNGER_LIMITS, HUNGER_DAMAGE_COOLDOWN, PARENTS, MUTATION_RATE, MUTATION_AMOUNT,
	ACCUMULATED_ENERGY, AGE, LIFE_EXPECTANCY, BREEDING_AGE, BREEDING_RATE, BIRTHDATE,
	MATE_FIND_COOLDOWN, FERTILITY, BREEDING_COUNT, DEATHDATE, DEATHTIME };

const char *EntityPropertyNames[];

class GameEntity;

typedef map<unsigned int, GameEntity *> EntityMap;

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
	virtual void loadFromFile(FormattedFileIterator &iter);
	// Saves the entity to a file stream. Must be in the same format that can be tokenized and
	// loaded using the loadFromFile function.
	virtual void saveToFile(FormattedFile &file);
	
	// If this entity currently has focus, this function is called with the last key that was pressed.
	// This can be called multiple times a frame if multiple keys have been pressed.
	virtual void keyActions(const int key);
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

	virtual float getAmountEaten() { return mAmountEaten; }
	virtual float beEaten(GameEntity *eater);

	virtual string getSpecies() { return mSpecies; }
	virtual void setSpecies(string species) { mSpecies = species; }

	virtual string getSpeciesName() { return getSpecies(); }

	virtual float getSize() { return mSize; }
	virtual void  setSize(float size) { mSize = size; }

	virtual float getMass() { return mMass; }
	virtual void  setMass(float mass) { mMass = mass; }

	virtual float getLineOfSightRadius();
	virtual void setLineOfSightRadius(float radius);

	static int nextId() { return sId++; }
	
	void setId(int id);
	unsigned int getId() { return mId; }

	static GameEntity *getEntityById(int id);
	static void removeEntity(int id);
	static void removeEntity(GameEntity *entity);

	virtual Action *getCurrentAction() { return mCurrentAction; }
	virtual vector<Action *> *getPastActions() { return &mPastActions; }

	virtual double distanceToEntity(GameEntity *entity);

	virtual bool canSeeEntity(GameEntity *entity);

	virtual void getNearbyEntities(const float &radius, vector<GameEntity *> &result);
	virtual void getNearbyEntities(const float &radius, vector<GameEntity *> &result, string &restrictToSpecies);

protected:
	unsigned int mId;
	Pixel mGraphic;
	float mGraphicFlashCooldown;
	Game* mGame;
	Matrix3x3f mTransform;
	string mName;
	Action *mCurrentAction;
	vector<Action *> mPastActions;

	float mHealth;
	float mMaxHealth;
	float mAmountEaten;
	float mLineOfSightRadius;

	float mSize;
	float mMass;
	
	bool mRedisplay;
	UIText *mHudText;

	string mSpecies;

	static EntityMap sEntities;

	virtual void setCurrentAction(Action *action);

	int mMenuLevel;
	int mMaxMenuLevel;
	
	// The function which loads each property from the file tokens.
	// Should increment the iterator at least once.
	virtual void loadProperties(FormattedFileIterator &iter);
	// Saves an individual property based on the property ID to the file stream.
	virtual void saveProperty(const EntityProperty &propertyId, FormattedFile &file);
	// Calls the appropriate functions to save all the properties for this entity.
	virtual void saveProperties(FormattedFile &file);
	
	// Call when the entity is added into a Game.
	virtual void onAddedToGame() {}

	virtual void getNearbyEntities(const float &radius, vector<GameEntity *> &result, string *restrictToSpecies);

	static const float GRAPHIC_FLASH_COOLDOWN;

private:
	static unsigned int sId;

};

#pragma once

#include <iostream>
#include <curses.h>
#include <math.h>

#include "Map.h"
#include "Pixel.h"
#include "Vector2.h"
#include "Matrix3x3.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

using namespace std;

#define STATE_IDLE		0
#define STATE_MOVING	1

class Game;

class GameEntity
{
public:
	GameEntity(Game *game);
	~GameEntity(void);

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

	virtual void update(float dt);
	virtual void render(Rect screenSize, WINDOW *wnd);

	inline int round(float x)
	{
		x = floor(x + 0.5f);
		if(x < 0)
			return (int)x - 1;
		return (int)x;
	}

	inline int round(double x)
	{
		x = floor(x + 0.5);
		if(x < 0)
			return (int)x - 1;
		return (int)x;
	}

	void setDestination(Vector2 dest);
	void setDestination(MathType xPos, MathType yPos);
	Vector2 getDestination() { return mDestination; }

	vector<Vector2> *getPath() { return mPath; }

protected:
	Pixel mGraphic;
	Game* mGame;

	Matrix3x3 mTransform;
	vector<Vector2> *mPath;
	Vector2 mDestination;

	int mState;

	MathType mSpeed;
	MathType mTurningSpeed;

	virtual void doStateIdle(float dt) {}
	virtual void doStateMoving(float dt);
	virtual void updateMovePath();

	float getTurnAmount(float facing, float dest)
	{
		float diff = dest - facing;
		if(diff < 0.0f)
		{
			if(diff < -M_PI)
			{
				diff += M_PI * 2.0f;
			}
		}
		else
		{
			if(diff > M_PI)
			{
				diff -= M_PI * 2.0f;
			}
		}
		return diff;
	}
};

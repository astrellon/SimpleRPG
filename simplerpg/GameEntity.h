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

class Game;

class GameEntity
{
public:
	GameEntity(Game *game);
	~GameEntity(void);

	virtual Pixel getGraphic() { return mGraphic; }
	virtual void setGraphic(Pixel graphic) { mGraphic = graphic; }

	virtual double getX() { return mTransform.zx; }
	virtual double getY() { return mTransform.zy; }

	virtual void setFacing(double facing) { mTransform.setRotation(facing); }
	virtual double getFacing() { return mTransform.getAngle(); }

	virtual void move(double dx, double dy, bool inObjectSpace = true);
	virtual void move(Vector2 v, bool inObjectSpace = true);
	virtual void turn(double angle);

	virtual Matrix3x3 *getTransform() { return &mTransform; }

	virtual void update();
	virtual void render(Rect screenSize, WINDOW *wnd);

	inline int round(double x)
	{
		x = ceil(x);
		if(x < 0)
			return (int)x - 1;
		return (int)x;
	}

protected:
	Pixel mGraphic;
	Game* mGame;

	Matrix3x3 mTransform;
};

#pragma once

#include <math.h>
#include <time.h>

#include "Vector2.h"
#include "Matrix3x3.h"

#include <iostream>

using namespace std;

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#define CONST_A 10.0f / 2.5f

namespace math
{
	inline float nextFloat()
	{
		return (float)rand() / (float)RAND_MAX;
	}

	inline double nextDouble()
	{
		return (double)rand() / (double)RAND_MAX;
	}

	inline double distF(const double &x, const double &min, const double &max, const double &mid)
	{
		if(x < mid)
		{
			return (x - min) / (mid - min);
		}
		return (x - max) / (mid - max);
	}

	inline double distG(const double &x)
	{
		return 0.5;
	}

	float nextDist(const float &min, const float &max, const float &mid);
	Vector2f getClosetToRay(Vector2f A, Vector2f B, Vector2f P, bool segmentClamp = true);
	Vector2f getClosetToRay(Vector2f origin, Vector2f direction, float length, Vector2f p);
	float getDistanceToRay(Vector2f A, Vector2f B, Vector2f p, bool segmentClamp = true);
	float getDistanceToRay(Vector2f origin, Vector2f direction, float length, Vector2f p);
}
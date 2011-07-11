#pragma once

#include <math.h>
#include <time.h>

#include "Vector2.h"
#include "Matrix3x3.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

namespace math
{
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

	inline float nextFloat()
	{
		return (float)rand() / (float)RAND_MAX;
	}
}
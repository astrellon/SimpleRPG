#pragma once

#include <math.h>
#include <time.h>

#include "Vector2.h"
#include "Matrix3x3.h"

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

	inline float distF(const float &x, const float &min, const float &max, const float &mid)
	{
		if(x < mid)
		{
			return (x - min) / (mid - min);
		}
		return (x - max) / (mid - max);
	}

	inline float distG(const float &x)
	{
		return 0.5f;
	}

	inline float nextDist(const float &min, const float &max, const float &mid)
	{
		float total = max - min;
		while(true)
		{
			float x = total * nextFloat() + min;
			float u = nextFloat();
        
			if(u < distF(x, min, max, mid) / (CONST_A * distG(x)))
			{
				return x;
			}
		}
	}
}
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
	inline float nextFloat()
	{
		return (float)rand() / (float)RAND_MAX;
	}
}
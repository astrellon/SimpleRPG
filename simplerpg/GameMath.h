#pragma once

#include <math.h>

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
}
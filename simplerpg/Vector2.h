#pragma once

#include <math.h>
typedef float MathType;

#ifndef _WIN32
inline MathType abs(MathType x)
{
	return fabs((float)x);
}
#endif

class Vector2
{
public:
	MathType x;
	MathType y;

	Vector2(void) { x = 0.0f; y = 0.0f; }
	Vector2(MathType ix, MathType iy) { x = ix; y = iy; }
	~Vector2(void) {}

	inline Vector2 add(Vector2 rhs)
	{
		return Vector2(x + rhs.x, y + rhs.y);
	}
	inline Vector2 add(MathType dx, MathType dy)
	{
		return Vector2(x + dx, y + dy);
	}

	inline Vector2 sub(Vector2 rhs)
	{
		return Vector2(x - rhs.x, y - rhs.y);
	}
	inline Vector2 sub(MathType dx, MathType dy)
	{
		return Vector2(x - dx, y - dy);
	}

	inline Vector2 scale(MathType factor)
	{
		return Vector2(x * factor, y * factor);
	}

	inline Vector2 &normalise()
	{
		MathType len = length();
		if(len != 0.0f)
		{
			x /= len;
			y /= len;
		}

		return *this;
	}

	inline bool equals(Vector2 rhs, MathType tolerance = 0)
	{
		if(tolerance > 0)
		{
			return abs(rhs.x - x) <= tolerance && abs(rhs.y - y) <= tolerance;
		}
		else
		{
			return rhs.x == x && rhs.y == y;
		}
	}

	inline MathType length() { return sqrt(x * x + y * y); }
	inline MathType dot(Vector2 rhs) { return x * rhs.x + y * rhs.y; }
};


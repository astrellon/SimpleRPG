#pragma once

#include <math.h>

#ifndef _WIN32
inline double abs(double x)
{
	return fabs(x);
}
#endif

class Vector2
{
public:
	double x;
	double y;

	Vector2(void) { x = 0.0f; y = 0.0f; }
	Vector2(double ix, double iy) { x = ix; y = iy; }
	~Vector2(void) {}

	inline Vector2 add(Vector2 rhs)
	{
		return Vector2(x + rhs.x, y + rhs.y);
	}
	inline Vector2 add(double dx, double dy)
	{
		return Vector2(x + dx, y + dy);
	}

	inline Vector2 sub(Vector2 rhs)
	{
		return Vector2(x - rhs.x, y - rhs.y);
	}
	inline Vector2 sub(double dx, double dy)
	{
		return Vector2(x - dx, y - dy);
	}

	inline Vector2 scale(double factor)
	{
		return Vector2(x * factor, y * factor);
	}

	inline void normalise()
	{
		double len = length();
		if(len != 0.0f)
		{
			x /= len;
			y /= len;
		}
	}

	inline bool equals(Vector2 rhs, double tolerance = 0)
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

	inline double length() { return sqrt(x * x + y * y); }
	inline double dot(Vector2 rhs) { return x * rhs.x + y * rhs.y; }
};


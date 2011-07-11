#pragma once

#include <math.h>

#ifndef _WIN32
inline int abs(int x)
{
	return abs(x);
}
#endif

template <class T>
class Vector2
{
public:
	T x;
	T y;

	Vector2(void) { x = 0; y = 0; }
	Vector2(T ix, T iy) { x = ix; y = iy; }
	~Vector2(void) {}

	inline Vector2<T> add(Vector2<T> rhs)
	{
		return Vector2<T>(x + rhs.x, y + rhs.y);
	}
	inline Vector2<T> add(T dx, T dy)
	{
		return Vector2<T>(x + dx, y + dy);
	}

	inline Vector2<T> sub(Vector2<T> rhs)
	{
		return Vector2<T>(x - rhs.x, y - rhs.y);
	}
	inline Vector2<T> sub(T dx, T dy)
	{
		return Vector2<T>(x - dx, y - dy);
	}

	inline Vector2<T> scale(double factor)
	{
		return Vector2<T>((T)(x * factor), (T)(y * factor));
	}

	inline Vector2<T> &normalise()
	{
		double len = length();
		if(len != 0.0f)
		{
			x = (T)(x / len);
			y = (T)(y / len);
		}

		return *this;
	}

	inline bool equals(Vector2<T> rhs, double tolerance = 0)
	{
		if(tolerance > 0)
		{
			return (double)abs(rhs.x - x) <= tolerance && (double)abs(rhs.y - y) <= tolerance;
		}
		else
		{
			return rhs.x == x && rhs.y == y;
		}
	}

	inline double length() { return sqrt(x * x + y * y); }
	inline double dot(Vector2<T> rhs) { return x * rhs.x + y * rhs.y; }
};

typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
typedef Vector2<int> Vector2i;
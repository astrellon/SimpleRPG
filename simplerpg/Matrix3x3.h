#pragma once

#include <stdio.h>
#include <math.h>
#include "Vector2.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#define M_PIF 3.1415926535897932384626433832795f
#define M_PI2	M_PI * 2.0
#define M_PIF2	M_PIF * 2.0f
#endif

template <class T>
class Matrix3x3
{
public:
	Matrix3x3(void) { identity(); mFacing = 0; }
	~Matrix3x3(void) {}

	void setRotation(double angle)
	{
		xx = (T)cos(angle);
		yy = xx;
		yx = (T)sin(angle);
		xy = -yx;
		zz = 1;
		xz = yz = 0;
		while(angle > M_PI)
			angle -= M_PI2;
		
		while(angle <= -M_PI)
			angle += M_PI2;

		mFacing = angle;
	}

	void rotate(double angle)
	{
		setRotation(angle + mFacing);
	}

	void transformVectorConst(Vector2<T> *vec)
	{
		T x = vec->x;
		T y = vec->y;
		vec->x = x * xx + y * xy;
		vec->y = x * yx + y * yy;
	}

	Vector2<T> transformVector(Vector2<T> vec)
	{
		return Vector2<T>(vec.x * xx + vec.y * xy, vec.x * yx + vec.y * yy);
	}

	void multiply(Matrix3x3<T> rhs)
	{
		Matrix3x3<T> clone = *this;
		xx = clone.xx * rhs.xx +
			 clone.xy * rhs.yx +
			 clone.xz * rhs.zx;

		xy = clone.xx * rhs.xy +
			 clone.xy * rhs.yy +
			 clone.xz * rhs.zy;

		xz = clone.xx * rhs.xz +
			 clone.xy * rhs.yz +
			 clone.xz * rhs.zz;

		yx = clone.yx * rhs.xx +
			 clone.yy * rhs.yx +
			 clone.yz * rhs.zx;

		yy = clone.yx * rhs.xy +
			 clone.yy * rhs.yy +
			 clone.yz * rhs.zy;

		yz = clone.yx * rhs.xz +
			 clone.yy * rhs.yz +
			 clone.yz * rhs.zz;

		zx = clone.zx * rhs.xx +
			 clone.zy * rhs.yx +
			 clone.zz * rhs.zx;

		zy = clone.zx * rhs.xy +
			 clone.zy * rhs.yy +
			 clone.zz * rhs.zy;
		
		zz = clone.zx * rhs.xz +
			 clone.zy * rhs.yz +
			 clone.zz * rhs.zz;

		mFacing = acos(xx);
	}

	inline void identity()
	{
		mFacing = 0;
		xy = xz = yx = yz = zx = zy = 0.0f;
		xx = yy = zz = 1.0f;
	}

	void displayMatrix()
	{
		printf("%f, %f, %f\n", xx, xy, xz);
		printf("%f, %f, %f\n", yx, yy, yz);
		printf("%f, %f, %f\n", zx, zy, zz);
	}

	inline void translate(T dx, T dy)
	{
		zx += dx;
		zy += dy;
	}

	inline void translate(Vector2<T> vec)
	{
		zx += vec.x;
		zy += vec.y;
	}

	inline double getAngle() { return mFacing; }

	T xx;
	T xy;
	T xz;

	T yx;
	T yy;
	T yz;

	T zx;
	T zy;
	T zz;

protected:
	double mFacing;

};

typedef Matrix3x3<float> Matrix3x3f;
typedef Matrix3x3<double> Matrix3x3d;
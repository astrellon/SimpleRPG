#pragma once

#include <stdio.h>
#include <math.h>
#include "Vector2.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

class Matrix3x3
{
public:
	Matrix3x3(void) { identity(); }
	~Matrix3x3(void) {}

	void setRotation(double angle)
	{
		xx = cos(angle);
		yy = xx;
		yx = sin(angle);
		xy = -yx;
		zz = 1;
		xz = yz = 0;
		double twice = M_PI * 2.0;
		while(angle > M_PI)
			angle -= twice;
		
		while(angle <= -M_PI)
			angle += twice;

		mFacing = angle;
	}

	void rotate(double angle)
	{
		setRotation(angle + mFacing);
	}

	void transformVectorConst(Vector2 *vec)
	{
		double x = vec->x;
		double y = vec->y;
		vec->x = x * xx + y * xy;
		vec->y = x * yx + y * yy;
	}

	Vector2 transformVector(Vector2 vec)
	{
		return Vector2(vec.x * xx + vec.y * xy, vec.x * yx + vec.y * yy);
	}

	void multiply(Matrix3x3 rhs)
	{
		Matrix3x3 clone = *this;
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

	inline void translate(double dx, double dy)
	{
		zx += dx;
		zy += dy;
	}

	inline void translate(Vector2 vec)
	{
		zx += vec.x;
		zy += vec.y;
	}

	inline double getAngle() { return mFacing; }

	double xx;
	double xy;
	double xz;

	double yx;
	double yy;
	double yz;

	double zx;
	double zy;
	double zz;

protected:
	double mFacing;

};


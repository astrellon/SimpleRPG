#include "GameMath.h"

float math::nextDist(const float &min, const float &max, const float &mid)
{
	float total = max - min;
	if(abs(total) < 0.01)
	{
		return nextFloat() * total + min;
	}

	while(true)
	{
		double x = total * nextDouble() + min;
		double u = nextDouble();
        
		if(u < distF(x, min, max, mid))
		{
			return x;
		}
	}
}

int math::nextRoulette(const vector<float> &probabilities)
{
	if(probabilities.size() == 0)
	{
		return -1;
	}
	if(probabilities.size() == 1)
	{
		return 0;
	}
	float maxProb = -1.0f;
	for(int i = 0; i < probabilities.size(); i++)
	{
		maxProb = max(maxProb, probabilities[i]);
	}

	int size = probabilities.size();

	int count = 0;

	while(++count < 100)
	{
		double x = nextDouble() * size;
		double u = nextDouble() * maxProb;
		int index = floor(x);
		if(index >= size)
		{
			continue;
		}
        
		if(u < probabilities[index])
		{
			return x;
		}
	}

	return round(nextDouble() * size);
}

Vector2f math::getClosetToRay(Vector2f A, Vector2f B, Vector2f P, bool segmentClamp)
{
	Vector2f AP = P.sub(A);
	Vector2f AB = B.sub(A);
	float ab2 = AB.x*AB.x + AB.y*AB.y;
	float ap_ab = AP.x*AB.x + AP.y*AB.y;
	float t = ap_ab / ab2;
	if (segmentClamp)
	{
		if (t < 0.0f) t = 0.0f;
		else if (t > 1.0f) t = 1.0f;
	}
	AB.x *= t;
	AB.y *= t;
	Vector2f Closest = A.add(AB);

	return Closest;
}

Vector2f math::getClosetToRay(Vector2f origin, Vector2f direction, float length, Vector2f p)
{
	return getClosetToRay(origin, origin.add(direction.scale(length)), p);
}

float math::getDistanceToRay(Vector2f A, Vector2f B, Vector2f p, bool segmentClamp)
{
	Vector2f closest = getClosetToRay(A, B, p, segmentClamp);
	return p.sub(closest).length();
}
float math::getDistanceToRay(Vector2f origin, Vector2f direction, float length, Vector2f p)
{
	Vector2f closest = getClosetToRay(origin, origin.add(direction.scale(length)), p);
	return p.sub(closest).length();
}

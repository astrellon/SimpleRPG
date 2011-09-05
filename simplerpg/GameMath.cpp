#include "GameMath.h"

float math::nextDist(const float &min, const float &max, const float &mid)
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

Vector2f math::getClosetPoint(Vector2f A, Vector2f B, Vector2f P, bool segmentClamp)
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

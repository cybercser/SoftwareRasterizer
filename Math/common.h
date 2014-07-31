#ifndef COMMON_H
#define COMMON_H

#define FLOAT_EPSILON 1e-6
#define DTOR		0.017453 // convert degrees to radians, PI / 180
#define RTOD		57.29578 // convert radians to degrees, 180 / PI
#include <cmath>
#include <cassert>

#define ROUND(x) (int)((x)+0.5)

inline bool FloatEqual(float fp1, float fp2)
{
	return std::fabs(fp1 - fp2) < FLOAT_EPSILON;
}

inline float Clampf(float value, float minimum, float maximum)
{
	float ret = min(max(minimum, value), maximum);
	return ret;
}

inline float LERPf(float v1, float v2, float t)
{
	return v1 * (1 - t) + v2 * t;
}
#endif
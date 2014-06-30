#ifndef LIGHT_H
#define LIGHT_H

#include <string>

#include "Color.h"
#include "Math/vector3f.h"

class Light
{
public:
	enum LightType
	{
		LT_POINT = 0,
		LT_SPOT = 1,
		LT_DIRECTIONAL = 2
	};

	Light() :
		Type(LT_POINT),
		Ka(0.0f, 0.0f, 0.0f, 1.0f),
		Kd(1.0f, 1.0f, 1.0f, 1.0f),
		Ks(1.0f, 1.0f, 1.0f, 1.0f),
		Pos(0.0, 0.0, 1.0),
		Dir(0.0, 0.0, -1.0),
		C(1.0),
		L(0.0),
		Q(0.0)
	{
	}
	~Light() {}

	float GetAttenuation(float distance) // only valid for point light
	{
		return 1 / (C + L * distance + Q * distance * distance);
	}

	LightType Type;

	Color Ka; // Ambient
	Color Kd; // Diffuse
	Color Ks; // Specular

	Vector3f Pos;	// Position in world space
	Vector3f Dir;	// Direction in world space

	float Range;	// Cutoff range
	float Falloff;	// Falloff
	float C;		// attenuation constant
	float L;		// attenuation linear
	float Q;		// attenuation quadric
	float Theta;	// Inner angle of spotlight cone
	float Phi;      // Outer angle of spotlight cone
};
#endif // LIGHT_H

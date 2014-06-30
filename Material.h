#ifndef MATERIAL_H
#define MATERIAL_H


#include "Color.h"

class Material
{
public:
	Material() :
	  Ka(0.2f, 0.2f, 0.2f, 1.0f),
	  Kd(0.8f, 0.8f, 0.8f, 1.0f),
	  Ks(0.0f, 0.0f, 0.0f, 1.0f),
	  Ksh(0.0f),
	  Ke(0.0f, 0.0f, 0.0f, 1.0f)
	{}

	~Material() {}

	Color Ka;	// ambient
	Color Kd;	// diffuse
	Color Ks;	// specular
	float Ksh;	// shininess index
	Color Ke;	// emission
};
#endif // MATERIAL_H
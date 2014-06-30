#ifndef RASTERIZER_H
#define RASTERIZER_H

#include "Color.h"

#include "Math/vector4f.h"

// vertex format
typedef struct tVertex 
{
	Vector4f v;	// position
	Vector3f n; // normal
	//Vector2f t; // texture coordinate
	Color	c;
} Vertex;

// ����������
enum TriType
{
	TOP,
	BOTTOM,
	LEFT,
	RIGHT
};

// ��ɫģʽ
enum ShadeMode
{
	SHADE_FLAT					= 1,
	SHADE_GOURAUD				= 2,
	SHADE_PHONG					= 3,
	SHADE_FORCE_DWORD			= 0x7fffffff, /* force 32-bit size enum */
};

// ���ģʽ
enum FillMode
{
	FILL_POINT					= 1,
	FILL_SOLID					= 2,
	FILL_WIREFRAME				= 3,
	FILL_FORCE_DWORD			= 0x7fffffff, /* force 32-bit size enum */
};

// scanline rasterizer
class Rasterizer
{
public:
	Rasterizer();

	TriType GetTriType(Vertex* p0, Vertex* p1, Vertex* p2); // �ж�����������

	void RasterizeBottomTri(Vertex* p0, Vertex* p1, Vertex* p2, COLORREF** cb, float** zb, FillMode fm, ShadeMode sm);
	void RasterizeTopTri(Vertex* p0, Vertex* p1, Vertex* p2, COLORREF** cb, float** zb, FillMode fm, ShadeMode sm);
	void RasterizeLeftTri(Vertex* p0, Vertex* p1, Vertex* p2, COLORREF** cb, float** zb, FillMode fm, ShadeMode sm);
	void RasterizeRightTri(Vertex* p0, Vertex* p1, Vertex* p2, COLORREF** cb, float** zb, FillMode fm, ShadeMode sm);
};
#endif // RASTERIZER_H
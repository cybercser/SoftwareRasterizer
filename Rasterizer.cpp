#include "stdafx.h"
#include "Rasterizer.h"
#include "Math/common.h"

Rasterizer::Rasterizer()
{

}

TriType Rasterizer::GetTriType(Vertex* p0, Vertex* p1, Vertex* p2)
{
	TriType type;
	if (FloatEqual(p0->v.y, p1->v.y))
	{
		type = TOP;
	}
	else if (FloatEqual(p1->v.y, p2->v.y))
	{
		type = BOTTOM;
	}
	else
	{
		if (p1->v.y > p2->v.y)
			type = RIGHT;
		else
			type = LEFT;
	}
	return type;
}


/* case 1 平底三角形
*
*     p0
* 
*  p2      p1
*/
void Rasterizer::RasterizeBottomTri(Vertex* p0, Vertex* p1, Vertex* p2, COLORREF** cb, float** zb, FillMode fm, ShadeMode sm)
{
	float xs, xe;   // 端点 start end
	float dxy_left, dxy_right;  // 左右边斜率的倒数

	xs = xe = p0->v.x;
	float deltaY = 1 / (p2->v.y - p0->v.y); // 注意 为计算效率取倒数

	dxy_left = (p2->v.x - p0->v.x)  * deltaY;
	dxy_right = (p1->v.x - p0->v.x) * deltaY; 
	float t = 0;

	// 绘制每条扫描线
	for (int y = ROUND(p0->v.y); y <= ROUND(p1->v.y); ++y)
	{
		t = (y - p0->v.y) * deltaY;
		Color cxs = LERP(p0->c, p2->c, t);//deltaY * (p0->c * (p2->v.y - y) + p2->c * (y - p0->v.y));
		cxs.Clamp();
		cxs *= 255; 

		Color cxe = LERP(p0->c, p1->c, t);//deltaY * (p0->c * (p1->v.y - y) + p1->c * (y - p0->v.y));
		cxe.Clamp();
		cxe *= 255; 

		Color deltai(0, 0, 0);
		if (!FloatEqual(xs, xe))
			deltai = (cxe - cxs) / (xe - xs); // 颜色沿扫描线的变化率
		Color cxi = cxs;

		if ( fm == FILL_SOLID )
		{
			if ( sm == SHADE_FLAT ) // flat shading, faceted look
			{
				for (int x = ROUND(xs); x <= ROUND(xe); ++x)
				{
					cb[x][y] = RGB(p0->c.r * 255, p0->c.g * 255, p0->c.b * 255);
				}
			}
			else if ( sm == SHADE_GOURAUD ) // Gouraud shading, interpolation
			{
				for (int x = ROUND(xs); x <= ROUND(xe); ++x)
				{
					cb[x][y] = RGB(cxi.r, cxi.g, cxi.b);
					cxi += deltai;
				}
			}
		}
		else if (fm == FILL_WIREFRAME)
		{
			cb[ROUND(xs)][y] = RGB(cxs.r, cxs.g, cxs.b);
			cb[ROUND(xe)][y] = RGB(cxe.r, cxe.g, cxe.b);
		}

		xs += dxy_left;
		xe += dxy_right;
	}
}

/* case 0 平顶三角形
*
*  p0      p1
* 
*      p2
*/
void Rasterizer::RasterizeTopTri(Vertex* p0, Vertex* p1, Vertex* p2, COLORREF** cb, float** zb, FillMode fm, ShadeMode sm)
{
	float xs, xe;   // 端点 start end
	float dxy_left, dxy_right;  // 左右边斜率的倒数

	xs = p0->v.x;
	xe = p1->v.x;
	float deltaY = 1 / (p2->v.y - p0->v.y);

	dxy_left = deltaY * (p2->v.x - p0->v.x);
	dxy_right = deltaY * (p2->v.x - p1->v.x); 
	float t = 0;

	// 绘制每条扫描线
	for (int y = ROUND(p0->v.y); y <= ROUND(p2->v.y); ++y)
	{
		t = (y - p0->v.y) * deltaY;
		Color cxs = LERP(p0->c, p2->c, t);//deltaY * (p0->c * (p2->v.y - y) + p2->c * (y - p0->v.y));
		cxs.Clamp();
		cxs *= 255; 

		Color cxe = LERP(p1->c, p2->c, t);//deltaY * (p1->c * (p2->v.y - y) + p2->c * (y - p1->v.y));
		cxe.Clamp();
		cxe *= 255; 

		Color deltai(0, 0, 0);
		if (!FloatEqual(xs, xe))
			deltai = (cxe - cxs) / (xe - xs); // 颜色沿扫描线的变化率
		Color cxi = cxs;

		if ( fm == FILL_SOLID )
		{
			if ( sm == SHADE_FLAT )
			{
				for (int x = ROUND(xs); x <= ROUND(xe); ++x)
				{
					cb[x][y] = RGB(p0->c.r * 255, p0->c.g * 255, p0->c.b * 255);
				}
			}
			else if ( sm == SHADE_GOURAUD )
			{
				for (int x = ROUND(xs); x <= ROUND(xe); ++x)
				{
					cb[x][y] = RGB(cxi.r, cxi.g, cxi.b);
					cxi += deltai;
				}
			}
		}
		else if (fm == FILL_WIREFRAME)
		{
			cb[ROUND(xs)][y] = RGB(cxs.r, cxs.g, cxs.b);
			cb[ROUND(xe)][y] = RGB(cxe.r, cxe.g, cxe.b);
		}

		xs += dxy_left;
		xe += dxy_right;
	}
}

/* case 2 左三角形
*     p0
*
*   p       p1
*
*
*  p2
*/
void Rasterizer::RasterizeLeftTri(Vertex* p0, Vertex* p1, Vertex* p2, COLORREF** cb, float** zb, FillMode fm, ShadeMode sm)
{
	float xs, xe;   // 端点 start end
	float dxy_left, dxy_right;  // 左右边斜率的倒数
	Vertex p;	// 分割点
	float T; // 分割点参数
	Vector3f p0p1, p0p2;

	// 计算分割点p
	float deltaYp0p1 = 1 / (p1->v.y - p0->v.y);
	float deltaYp1p2 = 1 / (p2->v.y - p1->v.y);
	float deltaYp0p2 = 1 / (p2->v.y - p0->v.y);
	dxy_left = deltaYp0p2 * (p2->v.x - p0->v.x);
	dxy_right = deltaYp0p1 * (p1->v.x - p0->v.x); 
	float t;// 插值参数

	// p0p2直线参数方程
	p0p2 = (p2->v - p0->v).ToVector3f();// 方向
	p0p2.Normalize();
	p.v.y = p1->v.y;
	T = (p.v.y - p0->v.y) / p0p2.y;
	p.v.x = p0->v.x + T * p0p2.x;
	p.c = deltaYp0p2 * (p0->c * (p2->v.y - p.v.y) + p2->c * (p.v.y - p0->v.y));

	// 按平底三角形处理 p0p1p
	xs = xe = p0->v.x;
	// 绘制每条扫描线
	for (int y = ROUND(p0->v.y); y <= ROUND(p1->v.y); ++y)
	{
		t = deltaYp0p1 * (y - p0->v.y);
		Color cxs = LERP(p0->c, p.c, t);//deltaYp0p1 * (p0->c * (p.v.y - y) + p.c * (y - p0->v.y));
		cxs.Clamp();
		cxs *= 255; 

		Color cxe = LERP(p0->c, p1->c, t);//deltaYp0p1 * (p0->c * (p1->v.y - y) + p1->c * (y - p0->v.y));
		cxe.Clamp();
		cxe *= 255; 

		Color deltai(0, 0, 0);
		if (!FloatEqual(xs, xe))
			deltai = (cxe - cxs) / (xe - xs); // 颜色沿扫描线的变化率
		Color cxi = cxs;

		if ( fm == FILL_SOLID )
		{
			if ( sm == SHADE_FLAT )
			{
				for (int x = ROUND(xs); x <= ROUND(xe); ++x)
				{
					cb[x][y] = RGB(p0->c.r * 255, p0->c.g * 255, p0->c.b * 255);
				}
			}
			else if ( sm == SHADE_GOURAUD )
			{
				for (int x = ROUND(xs); x <= ROUND(xe); ++x)
				{
					cb[x][y] = RGB(cxi.r, cxi.g, cxi.b);
					cxi += deltai;
				}
			}
		}
		else if (fm == FILL_WIREFRAME)
		{
			cb[ROUND(xs)][y] = RGB(cxs.r, cxs.g, cxs.b);
			cb[ROUND(xe)][y] = RGB(cxe.r, cxe.g, cxe.b);
		}

		xs += dxy_left;
		xe += dxy_right;
	}

	// 按平顶三角形处理 pp1p2
	//RasterizeTopTri(&p, p1, p2);
	xs = p.v.x;
	xe = p1->v.x;
	dxy_right = deltaYp1p2 * (p2->v.x - p1->v.x); 
	// 绘制每条扫描线
	for (int y = ROUND(p1->v.y); y <= ROUND(p2->v.y); ++y)
	{
		t = deltaYp1p2 * (y - p.v.y);
		Color cxs = LERP(p.c, p2->c, t);//deltaYp1p2 * (p.c * (p2->v.y - y) + p2->c * (y - p.v.y));
		cxs.Clamp();
		cxs *= 255; 

		Color cxe = LERP(p1->c, p2->c, t);//deltaYp1p2 * (p1->c * (p2->v.y - y) + p2->c * (y - p1->v.y));
		cxe.Clamp();
		cxe *= 255; 

		Color deltai(0, 0, 0);
		if (!FloatEqual(xs, xe))
			deltai = (cxe - cxs) / (xe - xs); // 颜色沿扫描线的变化率
		Color cxi = cxs;

		if ( fm == FILL_SOLID )
		{
			if ( sm == SHADE_FLAT )
			{
				for (int x = ROUND(xs); x <= ROUND(xe); ++x)
				{
					cb[x][y] = RGB(p0->c.r * 255, p0->c.g * 255, p0->c.b * 255);
				}
			}
			else if ( sm == SHADE_GOURAUD )
			{
				for (int x = ROUND(xs); x <= ROUND(xe); ++x)
				{
					cb[x][y] = RGB(cxi.r, cxi.g, cxi.b);
					cxi += deltai;
				}
			}
		}
		else if (fm == FILL_WIREFRAME)
		{
			cb[ROUND(xs)][y] = RGB(cxs.r, cxs.g, cxs.b);
			cb[ROUND(xe)][y] = RGB(cxe.r, cxe.g, cxe.b);
		}

		xs += dxy_left;
		xe += dxy_right;
	}
}

/* case 3 右三角形
*
*     p0
*
*  p2      p
* 
*
*			p1
*/
void Rasterizer::RasterizeRightTri(Vertex* p0, Vertex* p1, Vertex* p2, COLORREF** cb, float** zb, FillMode fm, ShadeMode sm)
{
	float xs, xe;   // 端点 start end
	float dxy_left, dxy_right;  // 左右边斜率的倒数
	Vertex p;	// 分割点
	float T; // 分割点参数
	Vector3f p0p1, p0p2;

	// 计算分割点p
	float deltaYp0p2 = 1 / (p2->v.y - p0->v.y);
	float deltaYp0p1 = 1 / (p1->v.y - p0->v.y);
	float deltaYp1p2 = 1 / (p1->v.y - p2->v.y);
	dxy_left = deltaYp0p2 * (p2->v.x - p0->v.x);
	dxy_right = deltaYp0p1 * (p1->v.x - p0->v.x); 
	float t; // 插值参数

	// p0p1直线参数方程
	p0p1 = (p1->v - p0->v).ToVector3f();// 方向
	p0p1.Normalize();
	p.v.y = p2->v.y;
	T = (p.v.y - p0->v.y) / p0p1.y;
	p.v.x = p0->v.x + T * p0p1.x;
	p.c = deltaYp0p1 * (p0->c * (p1->v.y - p.v.y) + p1->c * (p.v.y - p0->v.y));

	// 按平底三角形处理 p0pp2
	xs = xe = p0->v.x;
	// 绘制每条扫描线
	for (int y = ROUND(p0->v.y); y <= ROUND(p2->v.y); ++y)
	{
		t = deltaYp0p2 * (y - p0->v.y);
		Color cxs = LERP(p0->c, p2->c, t);//deltaYp0p2 * (p0->c * (p2->v.y - y) + p2->c * (y - p0->v.y));
		cxs.Clamp();
		cxs *= 255; 

		Color cxe = LERP(p0->c, p.c, t);//deltaYp0p2 * (p0->c * (p.v.y - y) + p.c * (y - p0->v.y));
		cxe.Clamp();
		cxe *= 255; 

		Color deltai(0, 0, 0);
		if (!FloatEqual(xs, xe))
			deltai = (cxe - cxs) / (xe - xs); // 颜色沿扫描线的变化率
		Color cxi = cxs;

		if ( fm == FILL_SOLID )
		{
			if ( sm == SHADE_FLAT )
			{
				for (int x = ROUND(xs); x <= ROUND(xe); ++x)
				{
					cb[x][y] = RGB(p0->c.r * 255, p0->c.g * 255, p0->c.b * 255);
				}
			}
			else if ( sm == SHADE_GOURAUD )
			{
				for (int x = ROUND(xs); x <= ROUND(xe); ++x)
				{
					cb[x][y] = RGB(cxi.r, cxi.g, cxi.b);
					cxi += deltai;
				}
			}

		}
		else if (fm == FILL_WIREFRAME)
		{
			cb[ROUND(xs)][y] = RGB(cxs.r, cxs.g, cxs.b);
			cb[ROUND(xe)][y] = RGB(cxe.r, cxe.g, cxe.b);
		}

		xs += dxy_left;
		xe += dxy_right;
	}

	// 按平顶三角形处理 pp1p2
	xs = p2->v.x;
	xe = p.v.x;
	dxy_left = deltaYp1p2 * (p1->v.x - p2->v.x);
	// 绘制每条扫描线
	for (int y = ROUND(p2->v.y); y <= ROUND(p1->v.y); ++y)
	{
		t = deltaYp1p2 * (y - p2->v.y);
		Color cxs = LERP(p2->c, p1->c, t);//deltaYp1p2 * (p2->c * (p1->v.y - y) + p1->c * (y - p2->v.y));
		cxs.Clamp();
		cxs *= 255; 

		Color cxe = LERP(p1->c, p.c, t);//deltaYp1p2 * (p1->c * (y - p.v.y) + p.c * (p1->v.y - y));
		cxe.Clamp();
		cxe *= 255; 

		Color deltai(0, 0, 0);
		if (!FloatEqual(xs, xe))
			deltai = (cxe - cxs) / (xe - xs); // 颜色沿扫描线的变化率
		Color cxi = cxs;

		if ( fm == FILL_SOLID )
		{
			if ( sm == SHADE_FLAT )
			{
				for (int x = ROUND(xs); x <= ROUND(xe); ++x)
				{
					cb[x][y] = RGB(p0->c.r * 255, p0->c.g * 255, p0->c.b * 255);
				}
			}
			else if ( sm == SHADE_GOURAUD )
			{
				for (int x = ROUND(xs); x <= ROUND(xe); ++x)
				{
					cb[x][y] = RGB(cxi.r, cxi.g, cxi.b);
					cxi += deltai;
				}
			}
		}
		else if (fm == FILL_WIREFRAME)
		{
			cb[ROUND(xs)][y] = RGB(cxs.r, cxs.g, cxs.b);
			cb[ROUND(xe)][y] = RGB(cxe.r, cxe.g, cxe.b);
		}

		xs += dxy_left;
		xe += dxy_right;
	}
}


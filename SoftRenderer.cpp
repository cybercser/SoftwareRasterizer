#include "StdAfx.h"
#include "SoftRenderer.h"

#include <cmath>
#include <utility>

#include "Light.h"
#include "Material.h"
#include "Camera.h"
#include "Math\common.h"

#define _USE_MATH_DEFINES
#include <math.h>

SoftRenderer::SoftRenderer(void) : 
	m_pRas(new Rasterizer()),
	m_Ka(0.125f),
	m_pM(new Material()),
	m_uVertexNum(24),
	m_uPrimtiveNum(12),
	m_fXAngle(0.0f),
	m_fYAngle(0.0f),
	m_eFillMode(FillMode::FILL_SOLID),
	m_eShadeMode(ShadeMode::SHADE_GOURAUD),
	m_bLighting(false),
    m_bZEnable(true),
    m_bZWrite(true)
{
}

SoftRenderer::~SoftRenderer(void)
{
	for ( auto lit = m_Lights.begin(); lit != m_Lights.end(); ++lit )
	{
		delete *lit;
		*lit = nullptr;
	}
	DeleteVertexBuffer();
	DeleteIndexBuffer();
	for ( int i = 0; i < WINDOW_HEIGHT; ++i )
	{
		delete[] FCB[i];
		delete[] FZB[i];

		delete[] BCB[i];
		delete[] BZB[i];
	}

	delete[] FCB;
	delete[] FZB;
	delete[] BCB;
	delete[] BZB;

	delete m_pRas;
}

void SoftRenderer::Initialize()
{
	// InitGeometry
	CreateVertexBuffer();
	CreateIndexBuffer();
	// Camera, light, rasterizer initialization
	

	// Frame buffers
	FCB = new COLORREF*[WINDOW_WIDTH];
	FZB = new float*[WINDOW_WIDTH];
	BCB = new COLORREF*[WINDOW_WIDTH];
	BZB = new float*[WINDOW_WIDTH];

	for ( int i = 0; i < WINDOW_WIDTH; ++i )
	{
		FCB[i] = new COLORREF[WINDOW_HEIGHT];
		FZB[i] = new float[WINDOW_HEIGHT];

		BCB[i] = new COLORREF[WINDOW_HEIGHT];
		BZB[i] = new float[WINDOW_HEIGHT];
	}

    SetupLights();
}

void SoftRenderer::CreateVertexBuffer()
{
	// A simple triangle
    // A simple cube
    m_uVertexNum = 24;
	VB = new Vertex[m_uVertexNum];
	TVB = new Vertex[m_uVertexNum];

// 	VB[0].v = Vector4f(-1.0f, -1.0f, 0.0f); TVB[0].c = VB[0].c = Color(1.f, 0.f, 0.f, 1.f); VB[0].n = Vector3f(0.0f, 0.0f, 1.0f);
// 	VB[1].v = Vector4f(1.0f, -1.0f, 0.0f); TVB[1].c = VB[1].c = Color(0.f, 0.f, 1.f, 1.f); VB[1].n = Vector3f(0.0f, 0.0f, 1.0f);
// 	VB[2].v = Vector4f(0.0f, 1.0f, 0.0f); TVB[2].c = VB[2].c = Color(1.f, 1.f, 1.f, 1.f); VB[2].n = Vector3f(0.0f, 0.0f, 1.0f);
	//VB[3].v = Vector4f(0.0f, -3.0f, 0.0f); TVB[3].c = VB[3].c = Color(1.f, 0.f, 1.f, 1.f); VB[3].n = Vector3f(0.0f, 0.0f, 1.0f);

    // front and back
    VB[0].v =  Vector4f(-1.0f, -1.0f, -1.0f); TVB[0].c = VB[0].c = Color(1.f, 0.f, 0.f, 1.f); VB[0].n = Vector3f( 0.0f, 0.0f, -1.0f);
    VB[1].v =  Vector4f(-1.0f,  1.0f, -1.0f); TVB[1].c = VB[1].c = Color(0.f, 0.f, 1.f, 1.f); VB[1].n = Vector3f( 0.0f, 0.0f, -1.0f);
    VB[2].v =  Vector4f( 1.0f,  1.0f, -1.0f); TVB[2].c = VB[2].c = Color(1.f, 1.f, 1.f, 1.f); VB[2].n = Vector3f( 0.0f, 0.0f, -1.0f);
    VB[3].v =  Vector4f( 1.0f, -1.0f, -1.0f); TVB[3].c = VB[3].c = Color(1.f, 0.f, 1.f, 1.f); VB[3].n = Vector3f( 0.0f, 0.0f, -1.0f);
    VB[4].v =  Vector4f(-1.0f, -1.0f,  1.0f); TVB[4].c = VB[4].c = Color(1.f, 1.f, 0.f, 1.f); VB[4].n = Vector3f( 0.0f, 0.0f,  1.0f);
    VB[5].v =  Vector4f(-1.0f,  1.0f,  1.0f); TVB[5].c = VB[5].c = Color(0.f, 0.f, 0.f, 1.f); VB[5].n = Vector3f( 0.0f, 0.0f,  1.0f);
    VB[6].v =  Vector4f( 1.0f,  1.0f,  1.0f); TVB[6].c = VB[6].c = Color(0.f, 1.f, 0.f, 1.f); VB[6].n = Vector3f( 0.0f, 0.0f,  1.0f);
    VB[7].v =  Vector4f( 1.0f, -1.0f,  1.0f); TVB[7].c = VB[7].c = Color(1.f, 0.f, 1.f, 1.f); VB[7].n = Vector3f( 0.0f, 0.0f,  1.0f);

    // left and right
    VB[8].v  = Vector4f(-1.0f, -1.0f,  1.0f); TVB[8].c  = VB[8].c  = Color(1.f, 0.f, 0.f, 1.f); VB[8].n  = Vector3f(-1.0f, 0.0f,  0.0f);
    VB[9].v  = Vector4f(-1.0f,  1.0f,  1.0f); TVB[9].c  = VB[9].c  = Color(0.f, 0.f, 1.f, 1.f); VB[9].n  = Vector3f(-1.0f, 0.0f,  0.0f);
    VB[10].v = Vector4f(-1.0f,  1.0f, -1.0f); TVB[10].c = VB[10].c = Color(1.f, 1.f, 1.f, 1.f); VB[10].n = Vector3f(-1.0f, 0.0f,  0.0f);
    VB[11].v = Vector4f(-1.0f, -1.0f, -1.0f); TVB[11].c = VB[11].c = Color(1.f, 0.f, 1.f, 1.f); VB[11].n = Vector3f(-1.0f, 0.0f,  0.0f);
    VB[12].v = Vector4f( 1.0f, -1.0f, -1.0f); TVB[12].c = VB[12].c = Color(1.f, 1.f, 0.f, 1.f); VB[12].n = Vector3f( 1.0f, 0.0f,  0.0f);
    VB[13].v = Vector4f( 1.0f,  1.0f, -1.0f); TVB[13].c = VB[13].c = Color(0.f, 0.f, 0.f, 1.f); VB[13].n = Vector3f( 1.0f, 0.0f,  0.0f);
    VB[14].v = Vector4f( 1.0f,  1.0f,  1.0f); TVB[14].c = VB[14].c = Color(0.f, 1.f, 0.f, 1.f); VB[14].n = Vector3f( 1.0f, 0.0f,  0.0f);
    VB[15].v = Vector4f( 1.0f, -1.0f,  1.0f); TVB[15].c = VB[15].c = Color(1.f, 0.f, 1.f, 1.f); VB[15].n = Vector3f( 1.0f, 0.0f,  0.0f);

    // top and bottom
    VB[16].v = Vector4f(-1.0f,  1.0f, -1.0f); TVB[16].c = VB[16].c = Color(1.f, 0.f, 0.f, 1.f); VB[16].n = Vector3f( 0.0f, 1.0f,  0.0f);
    VB[17].v = Vector4f(-1.0f,  1.0f,  1.0f); TVB[17].c = VB[17].c = Color(0.f, 0.f, 1.f, 1.f); VB[17].n = Vector3f( 0.0f, 1.0f,  0.0f);
    VB[18].v = Vector4f( 1.0f,  1.0f,  1.0f); TVB[18].c = VB[18].c = Color(1.f, 1.f, 1.f, 1.f); VB[18].n = Vector3f( 0.0f, 1.0f,  0.0f);
    VB[19].v = Vector4f( 1.0f,  1.0f, -1.0f); TVB[19].c = VB[19].c = Color(1.f, 0.f, 1.f, 1.f); VB[19].n = Vector3f( 0.0f, 1.0f,  0.0f);
    VB[20].v = Vector4f(-1.0f, -1.0f, -1.0f); TVB[20].c = VB[20].c = Color(1.f, 1.f, 0.f, 1.f); VB[20].n = Vector3f( 0.0f,-1.0f,  0.0f);
    VB[21].v = Vector4f( 1.0f, -1.0f,  1.0f); TVB[21].c = VB[21].c = Color(0.f, 0.f, 0.f, 1.f); VB[21].n = Vector3f( 0.0f,-1.0f,  0.0f);
    VB[22].v = Vector4f(-1.0f, -1.0f,  1.0f); TVB[22].c = VB[22].c = Color(0.f, 1.f, 0.f, 1.f); VB[22].n = Vector3f( 0.0f,-1.0f,  0.0f);
    VB[23].v = Vector4f( 1.0f, -1.0f, -1.0f); TVB[23].c = VB[23].c = Color(1.f, 0.f, 1.f, 1.f); VB[23].n = Vector3f( 0.0f,-1.0f,  0.0f);
}

void SoftRenderer::DeleteVertexBuffer()
{
	delete[] VB;
	delete[] TVB;
}

void SoftRenderer::CreateIndexBuffer()
{
    m_uPrimtiveNum = 12;
	IB = new UINT16[m_uPrimtiveNum * 3];
// 	IB[0] = 0;
// 	IB[1] = 2;
// 	IB[2] = 1;

// 	IB[3] = 0;
// 	IB[4] = 1;
// 	IB[5] = 3;

    // front side
    IB[0] = 0; IB[1] = 1; IB[2] = 2;
    IB[3] = 0; IB[4] = 2; IB[5] = 3;

    // back side
    IB[6] = 4; IB[7] = 6; IB[8] = 5;
    IB[9] = 4; IB[10] = 7; IB[11] = 6;

    // left side
    IB[12] = 8; IB[13] = 9; IB[14] = 10;
    IB[15] = 8; IB[16] = 10; IB[17] = 11;

    // right side
    IB[18] = 12; IB[19] = 13; IB[20] = 14;
    IB[21] = 12; IB[22] = 14; IB[23] = 15;

    // top
    IB[24] = 16; IB[25] = 17; IB[26] = 18;
    IB[27] = 16; IB[28] = 18; IB[29] = 19;

    // bottom
    IB[30] = 20; IB[31] = 21; IB[32] = 22;
    IB[33] = 20; IB[34] = 23; IB[35] = 21;
}

void SoftRenderer::DeleteIndexBuffer()
{
	delete[] IB;
}

void SoftRenderer::SetupMatrices()
{
	// 	UINT iTime = timeGetTime() % 1000;
	// 	FLOAT fAngle = iTime * ( 2.0f * M_PI ) / 1000.0f;
	//m_fYAngle = M_PI_4;
	//m_model = RotateX(m_fXAngle);
	m_world *= RotateY(m_fYAngle);

	// Position and aim the camera
	Vector3f position(0.0f, 3.0f, -5.0f);
	Vector3f target(0.0f, 0.0f, 0.0f);
	Vector3f up(0.0f, 1.0f, 0.0f);
	m_view = LookAt(position, target, up);

	float ar = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);
	m_proj = Perspective(M_PI_4, ar, 1.0f, 100.0f);

	m_port = ViewPort(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void SoftRenderer::SetupLights()
{
	m_pM->Kd.r = m_pM->Ka.r = 1.0f;
	m_pM->Kd.g = m_pM->Ka.g = 1.0f;
	m_pM->Kd.b = m_pM->Ka.b = 0.0f;
	m_pM->Kd.a = m_pM->Ka.a = 1.0f;

	// 目前仅使用一个光源作为测试
	Light* l = new Light();
	l->Ka.r = l->Ka.g = l->Ka.r = 0.125f;

	l->Kd.r = 1.0f;
	l->Kd.g = 1.0f;
	l->Kd.b = 1.0f;
    l->Pos = Vector3f(-2.0f, -2.0f, -2.0f);
	l->Dir = Vector3f(0.0f, 0.0f, 1.0f);
	l->Range = 1000.0f;
	m_Lights.push_back(l);

	// 环境光
	m_Ka = 0.125f;
}

void SoftRenderer::Render()
{
	// Clear the backbuffer to a black color
	Clear(CLEAR_TARGET | CLEAR_ZBUFFER, RGB(255, 255, 255), 0, 0);

	// Setup the Lights and materials
	//SetupLights();

	// Setup the world, view, and projection matrices
	SetupMatrices();

	// BEGIN SCENE

	// set stream source
	// set FVF

	// Draw primitive
	// Per-vertex operations
	for ( size_t vi = 0; vi < m_uVertexNum; ++vi )
	{
		TL(vi); // transform and lighting (T&L)
	}

	// Per-primitive operations
	// rasterization
	for ( size_t pi = 0; pi < m_uPrimtiveNum; ++pi )
	{
		UINT32 index0 = IB[pi * 3 + 0];
		UINT32 index1 = IB[pi * 3 + 1];
		UINT32 index2 = IB[pi * 3 + 2];

		Vertex* p0 = &TVB[index0];
		Vertex* p1 = &TVB[index1];
		Vertex* p2 = &TVB[index2];

		// 对三个顶点排序
		// 保证p0位于最上面 p1在p2右侧
		Vertex* temp;
		if (p1->v.y < p0->v.y)
		{
			temp = p1;
			p1 = p0;
			p0 = temp;
		}
		if (p2->v.y < p0->v.y)
		{
			temp = p2;
			p2 = p0;
			p0 = temp;
		}
		if (p2->v.x > p1->v.x)
		{
			temp = p2;
			p2 = p1;
			p1 = temp;
		}

		// 判断三角形属于哪一种类型
		TriType type = m_pRas->GetTriType(p0, p1, p2);

		switch(type)
		{
		case TOP:
			/* case 0 平顶三角形
			*
			*  p0      p1
			* 
			*      p2
			*/
			m_pRas->RasterizeTopTri(p0, p1, p2, BCB, m_bZEnable ? BZB : nullptr, m_eFillMode, m_eShadeMode);
			break;
		case BOTTOM:
			/* case 1 平底三角形
			*
			*     p0
			* 
			*  p2      p1
			*/
			m_pRas->RasterizeBottomTri(p0, p1, p2, BCB, m_bZEnable ? BZB : nullptr, m_eFillMode, m_eShadeMode);
			break;
		case LEFT:
			/* case 2 左三角形
			*     p0
			*
			*   p       p1
			*
			*
			*  p2
			*/
			m_pRas->RasterizeLeftTri(p0, p1, p2, BCB, m_bZEnable ? BZB : nullptr, m_eFillMode, m_eShadeMode);
			break;
		case RIGHT:
			/* case 3 右三角形
			*
			*     p0
			*
			*  p2      p
			* 
			*
			*			p1
			*/
			m_pRas->RasterizeRightTri(p0, p1, p2, BCB, m_bZEnable ? BZB : nullptr, m_eFillMode, m_eShadeMode);
			break;
		}
	}

	// END SCENE
	// present the backbuffer contents to the display
}

void SoftRenderer::Clear(DWORD flags, COLORREF color, float z, DWORD stencil)
{
	if ( flags & CLEAR_TARGET )
	{
		for ( int i = 0; i < WINDOW_WIDTH; ++i )
			for ( int j = 0; j < WINDOW_HEIGHT; ++j )
				BCB[i][j] = color;	
	}

	if ( flags & CLEAR_ZBUFFER )
	{
		for ( int i = 0; i < WINDOW_WIDTH; ++i )
			for ( int j = 0; j < WINDOW_HEIGHT; ++j )
				BZB[i][j] = z;
	}
}

// eye space T&L
void SoftRenderer::TL(size_t vi)
{
	Matrix4x4 WV =  m_world * m_view;
	// vertex transformation
	TVB[vi].v = VB[vi].v * WV;
	// normal transform
	TVB[vi].n = (Vector4f(VB[vi].n, 0.0f) * WV).ToVector3f(); // 偷懒法 更稳妥的办法是用逆转置
	TVB[vi].n.Normalize();

	// lighting in camera space
	Color ambient, diffuse, specular;
    //m_bLighting = false;
	if ( m_bLighting )
	{
		// Phong reflection model
		ambient = m_Ka * m_pM->Ka;
		ambient.Clamp();

		for (auto Lit = m_Lights.begin(); Lit != m_Lights.end(); ++Lit)
		{
			Light* L = *Lit;
			// transform light into camera space
			Vector4f lightPosCS = Vector4f(L->Pos, 1.0f) * WV;
			Vector3f lightDirCS = (Vector4f(L->Dir, 0.0f) * WV).ToVector3f();
			lightDirCS.Normalize();

			float NdotL = DotProduct(lightDirCS, TVB[vi].n);
			diffuse = Clampf(NdotL, 0.0f, 1.0f) * L->Kd * m_pM->Kd;
			diffuse.Clamp();

// 			Vector3f v = m_ViewerPos - TVB[vi].v.ToVector3f(); // 视线方向
// 			v.Normalize();
// 
// 			Vector3f r = 2 * NdotL * TVB[vi].n - lightDirCS; // 反射方向
// 			specular = pow(Clampf(DotProduct(r, v), 0.0f, 1.0f), m_pM->Ksh) * L->Ks * m_pM->Ks;

			// TODO: Blinn-Phong
		}

		TVB[vi].c = ambient + diffuse;
		//TVB[vi].c = ambient + diffuse + specular;
		TVB[vi].c.Clamp();
	}

	// projection 
	TVB[vi].v = TVB[vi].v * m_proj;

	// perspective division -- NDC (*)
	TVB[vi].v /= TVB[vi].v.w;
//   	TVB[vi].v.x /= TVB[vi].v.w;
//   	TVB[vi].v.y /= TVB[vi].v.w;
//   	TVB[vi].v.z /= TVB[vi].v.w;

	// viewport transformation (*)
	TVB[vi].v = TVB[vi].v * m_port;
}

void SoftRenderer::SetXAngle(FLOAT radian)
{
	m_fXAngle = radian;
}

void SoftRenderer::SetYAngle(FLOAT radian)
{
	m_fYAngle = radian;
}

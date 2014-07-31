#pragma once

#include <vector>

#include "PhongShader/PhongShader.h"

#include "Math/vector2f.h"
#include "Math/vector3f.h"
#include "Math/matrix4x4.h"

#include "Rasterizer.h"
/*
 * Options for clearing
 */
#define CLEAR_TARGET            0x00000001l  /* Clear target surface */
#define CLEAR_ZBUFFER           0x00000002l  /* Clear target z buffer */
#define CLEAR_STENCIL           0x00000004l  /* Clear stencil planes */

enum ProjectionType
{
	PT_ORTHOGRAPHIC,
	PT_PERSPECTIVE
};

// forward declaration
class Light;
class Material;
class Camera;

class SoftRenderer
{
public:
	SoftRenderer(void);
	~SoftRenderer(void);

	void Initialize();
	void SetupMatrices();
	void SetupLights();
	void Render();

	//void SetStreamSource();
	void Clear(DWORD flags, COLORREF color,float Z, DWORD stencil);	// clear frame buffer

	void SetXAngle(FLOAT radian);
	void SetYAngle(FLOAT radian);

	/************************************************************************/
	/* Buffer Object                                                        */
	/************************************************************************/
	Vertex* VB;	// vertex buffer
	Vertex* TVB;	// temporary transformed vertex in post-projection space
	UINT16*	IB;	// index buffer

	// temp
	size_t m_uVertexNum;
	size_t m_uPrimtiveNum; // for now, primitive is triangle D3DPT_TRIANGLELIST

	/************************************************************************/
	/* Framebuffer (double buffering)                                       */
	/************************************************************************/
	COLORREF**	FCB; // color buffer (front)
	float**		FZB; // depth buffer (front)

	COLORREF**	BCB; // color buffer (back)
	float**		BZB; // depth buffer (back)

private:
	FLOAT m_fXAngle, m_fYAngle;
	/************************************************************************/
	/* Geometry                                                             */
	/************************************************************************/
	// Geometry Specification
	void CreateVertexBuffer(); 	
	void DeleteVertexBuffer();
	void CreateIndexBuffer();
	void DeleteIndexBuffer();

	// Per-Vertex Operations (T&L)
	void TL(size_t vi);

	// Primitive Assembly
	// Primitive Processing (Clip/Projection/Viewport/Cull)

	// Rasterization

	// Fragment Processing (Texture/Fog)
	// Per-Fragment Operations, ROP (Alpha Test/Depth Test/Blending/Dithering/Logic Operations)
	// Framebuffer Operations

	/************************************************************************/
	/* Image                                                                */
	/************************************************************************/

private:
	ProjectionType m_eProjType;

	Vector3f				m_ViewerPos;
	float					m_Ka;			// ambient
	std::vector<Light*>		m_Lights;
	Material*				m_pM;
	Camera*					m_pCam;
	Rasterizer*				m_pRas;

	/************************************************************************/
	/* Transformation matrices                                              */
	/************************************************************************/
	Matrix4x4 m_world;	// world transformation	: object space	--> world space
	Matrix4x4 m_view;	// viewing transformation	: world space	--> eye space
	Matrix4x4 m_proj;	// projection transformation: eye space		--> clip space
	Matrix4x4 m_port;	// viewport transformation  : clip space    --> screen space

	/************************************************************************/
	/* Render state                                                         */
	/************************************************************************/
	FillMode m_eFillMode;
	ShadeMode m_eShadeMode;
	bool m_bLighting;
	bool m_bCull;
	bool m_bTexture;
	bool m_bFog;
	bool m_bZEnable;
	bool m_bBlend;

    bool m_bZWrite;
};

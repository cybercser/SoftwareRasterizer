#pragma once

#define MAX_LOADSTRING 100

#include "resource.h"

class SoftRenderer;
class SoftRendererApp
{
public:
	SoftRendererApp(HINSTANCE hInstance, int nCmdShow);
	~SoftRendererApp(void);

	bool Init();
	int Run();

private:
	ATOM MyRegisterClass();
	BOOL InitInstance();

	void Exit();

private:
	/************************************************************************/
	/* Windows related stuff                                                */
	/************************************************************************/
	HINSTANCE	m_hInstance;						// current instance
	int			m_nCmdShow;							// the show command
	TCHAR		m_szTitle[MAX_LOADSTRING];			// The title bar text
	TCHAR		m_szWindowClass[MAX_LOADSTRING];	// the main window class name
	HWND		m_hWnd;								// the main window handle
	HACCEL		m_hAccelTable;						// the acceleartor table
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
#include "StdAfx.h"
#include "SoftRendererApp.h"
#include "SoftRenderer.h"

#define TIMER_ID 100

SoftRenderer*	g_pRenderer  = new SoftRenderer();
BOOL		g_LeftBtnDown = FALSE;
POINT		g_ptPrev;

SoftRendererApp::SoftRendererApp(HINSTANCE hInstance, int nCmdShow) : 
	m_hInstance(hInstance), 
	m_nCmdShow(nCmdShow)
{
	Init();
}


SoftRendererApp::~SoftRendererApp(void)
{
	Exit();
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM SoftRendererApp::MyRegisterClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_hInstance;
	wcex.hIcon			= LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_SOFTRENDERER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= GetStockBrush(BLACK_BRUSH);//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_SOFTRENDERER);
	wcex.lpszClassName	= m_szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL SoftRendererApp::InitInstance()
{
	//m_hInstance = hInstance; // Store instance handle in our global variable

 	m_hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, m_szWindowClass, m_szTitle, WS_OVERLAPPED | WS_SYSMENU,
 		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, m_hInstance, NULL);
//     m_hWnd = CreateWindow(m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX),
//         CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, m_hInstance, NULL);

	if (!m_hWnd)
	{
		return FALSE;
	}

	ShowWindow(m_hWnd, m_nCmdShow);
	UpdateWindow(m_hWnd);

	//SetTimer(m_hWnd, TIMER_ID, 15, NULL);

	return TRUE;
}

bool SoftRendererApp::Init()
{
	// Initialize global strings
	LoadString(m_hInstance, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING);
	LoadString(m_hInstance, IDC_SOFTRENDERER, m_szWindowClass, MAX_LOADSTRING);
	MyRegisterClass();

	// Perform application initialization:
	if (!InitInstance())
	{
		return FALSE;
	}

	m_hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDC_SOFTRENDERER));

	return TRUE;
}

int SoftRendererApp::Run()
{
	// Main message loop:
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while( msg.message != WM_QUIT )
	{
		if ( PeekMessage( &msg , NULL , 0U , 0U , PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			//g_pRenderer->Render();
		}
	}  

	return (int) msg.wParam;
}

void SoftRendererApp::Exit()
{

}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HPEN hPen, hOldPen;
	POINT pt;
	pt.x = pt.y = 0;
	float dx, dy;

	switch (message)
	{
	case WM_CREATE:
		// Initialize the global renderer
		g_pRenderer->Initialize();		
		break;
 	case WM_PAINT:
		g_pRenderer->Render();
		hdc = BeginPaint(hWnd, &ps);
// 		hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
// 		hOldPen = SelectPen(hdc, hPen);
// 		MoveToEx(hdc, g_pRenderer->m_pTVB[0].v.x, g_pRenderer->m_pTVB[0].v.y, NULL);
// 		LineTo(hdc, g_pRenderer->m_pTVB[1].v.x, g_pRenderer->m_pTVB[1].v.y);
// 		LineTo(hdc, g_pRenderer->m_pTVB[2].v.x, g_pRenderer->m_pTVB[2].v.y);
// 		LineTo(hdc, g_pRenderer->m_pTVB[0].v.x, g_pRenderer->m_pTVB[0].v.y);
// 		SelectPen(hdc, hOldPen);
// 		DeletePen(hPen);

		for ( int j = 0; j < WINDOW_HEIGHT; ++j )
		{
			for ( int i = 0; i < WINDOW_WIDTH; ++i )
			{
				SetPixel(hdc, i, j, g_pRenderer->BCB[i][j]);
			}
		}
		EndPaint(hWnd, &ps);
 		break;
	case WM_LBUTTONDOWN:
		g_ptPrev.x = GET_X_LPARAM(lParam);
		g_ptPrev.y = GET_Y_LPARAM(lParam);
		g_LeftBtnDown = TRUE;
		break;
	case WM_LBUTTONUP:
		g_LeftBtnDown = FALSE;
		g_ptPrev = pt;
		break;
	case WM_MOUSEMOVE:
		if ( g_LeftBtnDown )
		{
			pt.x = GET_X_LPARAM(lParam);
			pt.y = GET_Y_LPARAM(lParam);

			dx = float(pt.x - g_ptPrev.x) * 0.125;
			dy = float(pt.y - g_ptPrev.y) * 0.125;

			//g_pRenderer->SetXAngle(dy);
			g_pRenderer->SetYAngle(-dx);

			InvalidateRect(hWnd, NULL, TRUE);
		}

		break;
	case WM_KEYDOWN:
		if ( wParam == VK_ESCAPE )
			DestroyWindow(hWnd);
		break;
// 	case WM_TIMER:
// 		InvalidateRect(hWnd, NULL, TRUE);
// 		break;
// 	case WM_CLOSE:
// 		KillTimer(hWnd, TIMER_ID);
// 		DestroyWindow(hWnd);
// 		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
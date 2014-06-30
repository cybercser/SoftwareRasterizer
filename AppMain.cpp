// SoftRenderer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
//#include "AppMain.h"
#include "SoftRendererApp.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Main loop
	SoftRendererApp app(hInstance, nCmdShow);
	return app.Run();
}

// PhongShader.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "PhongShader.h"


// This is an example of an exported variable
PHONGSHADER_API int nPhongShader=0;

// This is an example of an exported function.
PHONGSHADER_API int fnPhongShader(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see PhongShader.h for the class definition
CPhongShader::CPhongShader()
{
	return;
}

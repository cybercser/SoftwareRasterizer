// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PHONGSHADER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PHONGSHADER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef PHONGSHADER_EXPORTS
#define PHONGSHADER_API __declspec(dllexport)
#else
#define PHONGSHADER_API __declspec(dllimport)
#endif

// This class is exported from the PhongShader.dll
class PHONGSHADER_API CPhongShader {
public:
	CPhongShader(void);
	// TODO: add your methods here.
};

extern PHONGSHADER_API int nPhongShader;

PHONGSHADER_API int fnPhongShader(void);

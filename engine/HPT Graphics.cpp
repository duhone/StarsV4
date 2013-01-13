#include "HPT graphics.h"
#include "HPTGraphicsEngineInternal.h"


HWND hwnd;
HINSTANCE hinstance;
HPTGraphicsEngineInternal *gengine = NULL;


HPTGraphicsEngine* GetHPTGraphicsEngine(int quality,bool widescreen)
{
	if(gengine == NULL)
	{
		gengine = new HPTGraphicsEngineInternal(quality,widescreen);
	}
//	gengine->AddRef();
	return gengine;
}

// This is the constructor of a class that has been exported.
// see HPT Graphics.h for the class definition

void HPTSetWindow(HINSTANCE hins,HWND arg)
{
	hinstance = hins;
	hwnd = arg;
}
#if !defined(AFX_HPTGRAPHICSENGINE_H__A5257160_9A43_4189_A662_1DA51FCD84CC__INCLUDED_)
#define AFX_HPTGRAPHICSENGINE_H__A5257160_9A43_4189_A662_1DA51FCD84CC__INCLUDED_
#include<TCHAR.h>
#include<windows.h>

#define LOW_QUALITY 0
#define MED_QUALITY 1
#define HIGH_QUALITY 2

class HPT3DObject {
public:
	HPT3DObject() {};
	virtual ~HPT3DObject() {};
	virtual void SetImage(int fart,int closet) = 0;
	virtual void Render() = 0;
	virtual void SetPositionAbsalute(int x,int y,int z) = 0;
	virtual void SetPositionRelative(int x,int y,int z) = 0;
	virtual void Release() = 0;
	virtual void Scale(float scale_factor) = 0;
	virtual void Position(float x,float y,float z) = 0;
	virtual void Rotate(float y,float z) = 0;
	virtual void SetColor(float r,float g,float b,float a) = 0;
	virtual void SetNoisePosition(float arg) = 0;
};

class HPT3DObjectP {
public:
	HPT3DObjectP() {};
	virtual ~HPT3DObjectP() {};
	virtual void SetImage(int ground,int sky,int ground_bump,int sky_bump,int night) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
	virtual void Scale(float scale_factor) = 0;
	virtual void Position(float x,float y,float z) = 0;
	virtual void SkyPosition(float x) = 0;
	virtual void SetLightVec(float x,float y,float z) = 0;
	virtual void Rotate(float y,float z) = 0;
	virtual void SetColor(float r,float g,float b,float a) = 0;
	virtual void AddRing(int tex) = 0;
	virtual void DisableRing() = 0;
	virtual void EnableSky() = 0;
	virtual void DisableSky() = 0;
	virtual void SetEarth() = 0;
	virtual void SetGas() = 0;
	virtual void DisableGas() = 0;
	virtual void SetMaterial(float diffuse,float specular) = 0;
};

class HPTGraphicsEngine {
public:
	HPTGraphicsEngine() {};
	virtual ~HPTGraphicsEngine() {};
	virtual int LoadHGF(TCHAR *name) = 0;
	virtual void Release() = 0;
	virtual HPT3DObject* Create3DObject() = 0;
	virtual HPT3DObjectP* Create3DObjectP() = 0;
	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;
//	virtual HPTGraphicsEngine& operator<<(HPTFont1* arg) = 0;
	virtual HPTGraphicsEngine& operator<<(char *arg) = 0;
	virtual HPTGraphicsEngine& operator<<(int arg) = 0;
	virtual HPTGraphicsEngine& operator<<(unsigned int arg) = 0;
	virtual HPTGraphicsEngine& operator<<(double arg) = 0;

	virtual HPTGraphicsEngine& Position(int x,int y) = 0;
};

HPTGraphicsEngine* GetHPTGraphicsEngine(int quality,bool widescreen);

void HPTSetWindow(HINSTANCE hins,HWND arg);

#endif
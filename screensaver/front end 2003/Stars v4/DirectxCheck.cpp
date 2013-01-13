#include "StdAfx.h"
#include ".\directxcheck.h"
#using <mscorlib.dll>
#include <d3d9.h>


DirectxCheck::DirectxCheck(void)
{
}

DirectxCheck::~DirectxCheck(void)
{
}

bool DirectxCheck::CheckShader11(void)
{
	IDirect3D9 *d3d; 
    D3DCAPS9 d3d9caps;
    int count;
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    d3d->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&d3d9caps);

	unsigned long temp;
    temp =  D3DPTFILTERCAPS_MAGFLINEAR | D3DPTFILTERCAPS_MINFLINEAR |D3DPTFILTERCAPS_MIPFLINEAR;
        
    if( (d3d9caps.PixelShaderVersion >= D3DPS_VERSION(1,1)) && ((d3d9caps.TextureFilterCaps & temp) == temp) )
    {
			d3d->Release();
			return true;
    }
    d3d->Release();

	return false;
}

bool DirectxCheck::CheckShadder20(void)
{
	IDirect3D9 *d3d; 
    D3DCAPS9 d3d9caps;
    int count;
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    d3d->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&d3d9caps);

	unsigned long temp;
    temp =  D3DPTFILTERCAPS_MAGFLINEAR | D3DPTFILTERCAPS_MINFLINEAR |D3DPTFILTERCAPS_MIPFLINEAR;
        
    if( (d3d9caps.PixelShaderVersion >= D3DPS_VERSION(2,0)) && ((d3d9caps.TextureFilterCaps & temp) == temp) )
    {
			d3d->Release();
			return true;
    }
    d3d->Release();

	return false;
}

bool DirectxCheck::checkDirectx9(void)
{
	IDirect3D9 *d3d; 
    D3DCAPS9 d3d9caps;
    int count;
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if( d3d == NULL)
    {
         MessageBox(NULL, "Stars is unable to run.  A DirectX 9.0 compatibale video card was not found.", "Stars 4.0 Error", MB_OK);
		 d3d->Release();
		 return false;
    }
	d3d->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&d3d9caps);
	if( !(d3d9caps.DevCaps & D3DDEVCAPS_HWRASTERIZATION))
    {
         MessageBox(NULL, "Stars is unable to run.  A DirectX 9.0 compatibale video card was not found.", "Stars 4.0 Error", MB_OK);
		 d3d->Release();
		 return false;
    }

    d3d->Release();

	return true;
}

bool DirectxCheck::checkMedTexture(void)
{
	IDirect3D9 *d3d; 
    D3DCAPS9 d3d9caps;
    int count;
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    d3d->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&d3d9caps);
	if( (d3d9caps.MaxTextureWidth >=512) && (d3d9caps.MaxTextureHeight >=512) )
    {
        return true;
    }
	return false;
}

bool DirectxCheck::checkHighTexture(void)
{
	IDirect3D9 *d3d; 
    D3DCAPS9 d3d9caps;
    int count;
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    d3d->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&d3d9caps);
	if( (d3d9caps.MaxTextureWidth >= 1024) && (d3d9caps.MaxTextureHeight >= 1024) )
    {
            return true;
    }
    return false;
}

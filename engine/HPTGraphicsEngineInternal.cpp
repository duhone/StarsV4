// HPTGraphicsEngineInternal.cpp: implementation of the HPTGraphicsEngineInternal class.
//
//////////////////////////////////////////////////////////////////////

#include "HPTGraphicsEngineInternal.h"
#include"HPT3DObjectInternal.h"
#include"HPT3DObjectPInternal.h"
#include<vector>
#include "zlib.h"

extern HPTGraphicsEngineInternal *gengine;
extern HWND hwnd;
extern HINSTANCE hinstance;

using namespace std;

//#define SPHERE_CUTS 20
//#define RING_CUTS 20
#define PI 3.141592f
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern char path[255];

extern char *hptstrcat(char *x,char *y);

HPTGraphicsEngineInternal::HPTGraphicsEngineInternal(int quality,bool widescreen)
{
	this->widescreen = widescreen;
	render_quality = quality;
	switch(quality)
	{
	case LOW_QUALITY:
		sphere_cuts = 15;
		ring_cuts = 15;
		break;
	case MED_QUALITY:
		sphere_cuts = 20;
		ring_cuts = 20;
		break;
	case HIGH_QUALITY:
		sphere_cuts = 30;
		ring_cuts = 30;
		break;
	}

	d3d = NULL;
	d3d_device = NULL;

	if( NULL == ( d3d = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	  MessageBox(NULL,"Could not create directx9 device, make sure directx9 is installed","error",MB_OK);

//find matching depth format
	D3DFORMAT depth_format = D3DFMT_D32;
	D3DDISPLAYMODE d3d_mode;
	d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d3d_mode);
	
	if(FAILED(d3d->CheckDeviceType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,d3d_mode.Format,d3d_mode.Format,TRUE)))
	{
		MessageBox(NULL,"Display settings not compatible with directx. try swithcing to 32 or 16 bit color for your desktop","error",MB_OK);
		d3d->Release();
		d3d = NULL;
		exit(0);

	}
	
	
	if(FAILED(d3d->CheckDepthStencilMatch(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,d3d_mode.Format,d3d_mode.Format,depth_format)))
	{
		depth_format = D3DFMT_D24X8;
		if(FAILED(d3d->CheckDepthStencilMatch(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,d3d_mode.Format,d3d_mode.Format,depth_format)))
		{
			depth_format = D3DFMT_D24S8;
			if(FAILED(d3d->CheckDepthStencilMatch(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,d3d_mode.Format,d3d_mode.Format,depth_format)))
			{
				depth_format = D3DFMT_D24FS8;
				if(FAILED(d3d->CheckDepthStencilMatch(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,d3d_mode.Format,d3d_mode.Format,depth_format)))
				{
					depth_format = D3DFMT_D16;
					if(FAILED(d3d->CheckDepthStencilMatch(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,d3d_mode.Format,d3d_mode.Format,depth_format)))
					{
						MessageBox(NULL,"Could not find a compatible depth buffer format","error",MB_OK);
						d3d->Release();
						d3d = NULL;
						exit(0);
					
					}
					
				}
				
			}
			
		}
		
	}
	texture32bpp = true;
	if(FAILED(d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,d3d_mode.Format,0,D3DRTYPE_TEXTURE,D3DFMT_A8R8G8B8)))
	{
		texture32bpp = false;
		if(FAILED(d3d->CheckDeviceFormat(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,d3d_mode.Format,0,D3DRTYPE_TEXTURE,D3DFMT_A4R4G4B4)))
		{
			MessageBox(NULL,"Device does not support required texture formats. Requires A4R4G4B4 or A8R8G8B8 format support","error",MB_OK);
			d3d->Release();
			d3d = NULL;
			exit(0);

		}

	}

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = d3d_mode.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = depth_format/*D3DFMT_D24X8*/;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if( FAILED( d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
                                  D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                  &d3dpp, &d3d_device ) ) )
	{
		MessageBox(NULL,"Could not create a directx9 window, make sure you have the latest drivers","error",MB_OK);
		d3d->Release();
		d3d = NULL;
		exit(0);
	}
	CreateSpriteVertexBuffer();
	CreateSphereVertexBuffer();
	CreateSphereVertexBufferLow();
	CreateRingVertexBuffer();
	LoadShaders();
/*	LPD3DXBUFFER tempvbufer;
	LPD3DXBUFFER emsg;
	if(D3DXAssembleShaderFromFile(hptstrcat(path,"sprite vertex shader.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreateVertexShader((const unsigned long*)tempvbufer->GetBufferPointer(),&sprite_vertex_shader);

	if(D3DXAssembleShaderFromFile(hptstrcat(path,"planet vertex shader.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreateVertexShader((const unsigned long*)tempvbufer->GetBufferPointer(),&planet_vertex_shader);

	if(D3DXAssembleShaderFromFile(hptstrcat(path,"star vertex shader.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreateVertexShader((const unsigned long*)tempvbufer->GetBufferPointer(),&star_vertex_shader);

	if(D3DXAssembleShaderFromFile(hptstrcat(path,"star pixel shader.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreatePixelShader((const unsigned long*)tempvbufer->GetBufferPointer(),&star_pixel_shader);

	if(D3DXAssembleShaderFromFile(hptstrcat(path,"planet vertex shader med.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreateVertexShader((const unsigned long*)tempvbufer->GetBufferPointer(),&planet_vertex_shader_med);

	if(D3DXAssembleShaderFromFile(hptstrcat(path,"planet pixel shader med.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreatePixelShader((const unsigned long*)tempvbufer->GetBufferPointer(),&planet_pixel_shader_med);

	if(D3DXAssembleShaderFromFile(hptstrcat(path,"star vertex shader high.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreateVertexShader((const unsigned long*)tempvbufer->GetBufferPointer(),&star_vertex_shader_high);

	if(D3DXAssembleShaderFromFile(hptstrcat(path,"star pixel shader high.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreatePixelShader((const unsigned long*)tempvbufer->GetBufferPointer(),&star_pixel_shader_high);

	if(D3DXAssembleShaderFromFile(hptstrcat(path,"planet vertex shader high.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreateVertexShader((const unsigned long*)tempvbufer->GetBufferPointer(),&planet_vertex_shader_high);

	if(D3DXAssembleShaderFromFile(hptstrcat(path,"planet pixel shader high.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreatePixelShader((const unsigned long*)tempvbufer->GetBufferPointer(),&planet_pixel_shader_high);
    
	if(D3DXAssembleShaderFromFile(hptstrcat(path,"planet vertex shader earth.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreateVertexShader((const unsigned long*)tempvbufer->GetBufferPointer(),&planet_vertex_shader_earth);

	if(D3DXAssembleShaderFromFile(hptstrcat(path,"planet pixel shader earth.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreatePixelShader((const unsigned long*)tempvbufer->GetBufferPointer(),&planet_pixel_shader_earth);
*/ 
	d3d_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW);
    d3d_device->SetRenderState( D3DRS_AMBIENT, 0x00000000 );
	d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE ,TRUE );
	d3d_device->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	d3d_device->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
	d3d_device->SetRenderState( D3DRS_ZENABLE, TRUE );
    d3d_device->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
//    d3d_device->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
 
	//D3DXMatrixIdentity(&matview);
	//D3DXMatrixIdentity(&matproj);
	D3DXMatrixLookAtLH(&matview, &D3DXVECTOR3( 0.0f, 0.0f, -1.0f ),
                              &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
                              &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
	if(widescreen) D3DXMatrixPerspectiveFovLH( &matproj, D3DX_PI/4, 1.85f, 1.0f, 1000000.0f );
	else D3DXMatrixPerspectiveFovLH( &matproj, D3DX_PI/4, 1.333333f, 1.0f, 1000000.0f );

//	D3DXMatrixPerspectiveLH(&matproj,3,4,1,1000000);
	d3d_device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	d3d_device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
	d3d_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
	d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
	d3d_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
/*	d3d_device->SetTextureStageState(1,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	d3d_device->SetTextureStageState(1,D3DTSS_COLORARG2,D3DTA_CURRENT);
	d3d_device->SetTextureStageState(1,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	d3d_device->SetTextureStageState(1,D3DTSS_ALPHAARG2,D3DTA_CURRENT);
	d3d_device->SetTextureStageState(2,D3DTSS_COLORARG1,D3DTA_DIFFUSE);
	d3d_device->SetTextureStageState(2,D3DTSS_COLORARG2,D3DTA_CURRENT);
	d3d_device->SetTextureStageState(2,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
	d3d_device->SetTextureStageState(2,D3DTSS_ALPHAARG2,D3DTA_CURRENT);
*/	switch(quality)
	{
	case LOW_QUALITY:
		d3d_device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
		d3d_device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
		d3d_device->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_POINT);
		break;
	case MED_QUALITY:
		d3d_device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
		d3d_device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
		d3d_device->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
		d3d_device->SetSamplerState(1,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
		d3d_device->SetSamplerState(1,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
		d3d_device->SetSamplerState(1,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
		break;
	case HIGH_QUALITY:
		d3d_device->SetSamplerState(0,D3DSAMP_MAXANISOTROPY,8);
		d3d_device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_ANISOTROPIC);
		d3d_device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_ANISOTROPIC);
		d3d_device->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
		d3d_device->SetSamplerState(1,D3DSAMP_MAXANISOTROPY,8);
		d3d_device->SetSamplerState(1,D3DSAMP_MAGFILTER,D3DTEXF_ANISOTROPIC);
		d3d_device->SetSamplerState(1,D3DSAMP_MINFILTER,D3DTEXF_ANISOTROPIC);
		d3d_device->SetSamplerState(1,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
		d3d_device->SetSamplerState(2,D3DSAMP_MAXANISOTROPY,8);
		d3d_device->SetSamplerState(2,D3DSAMP_MAGFILTER,D3DTEXF_ANISOTROPIC);
		d3d_device->SetSamplerState(2,D3DSAMP_MINFILTER,D3DTEXF_ANISOTROPIC);
		d3d_device->SetSamplerState(2,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
		break;
	}

	//font = CreateFont(0,0,0,0,700,false,false,false,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"Arial");
	D3DXCreateFont(d3d_device,14,14,700,1,false,ANSI_CHARSET,OUT_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"Arial",&d3dfont);
	ChangeGraphicsState(0);
}

HPTGraphicsEngineInternal::~HPTGraphicsEngineInternal()
{
	d3d_device->SetVertexShader(NULL);
	d3d_device->SetPixelShader(NULL);

	if(sprite_vertex_shader != NULL) sprite_vertex_shader->Release();
	if(sprite_vertex_buffer != NULL) sprite_vertex_buffer->Release();
	sprite_vertex_decl->Release();
	if(planet_vertex_shader != NULL) planet_vertex_shader->Release(); 
	if(star_vertex_shader != NULL) star_vertex_shader->Release(); 
	if(star_pixel_shader != NULL) star_pixel_shader->Release(); 
	if(star_vertex_shader_high != NULL) star_vertex_shader_high->Release(); 
	if(star_pixel_shader_high != NULL) star_pixel_shader_high->Release(); 
	if(planet_vertex_shader_med != NULL) planet_vertex_shader_med->Release(); 
	if(planet_pixel_shader_med != NULL) planet_pixel_shader_med->Release(); 
	if(planet_vertex_shader_high != NULL) planet_vertex_shader_high->Release(); 
	if(planet_pixel_shader_high != NULL) planet_pixel_shader_high->Release(); 
	if(planet_vertex_shader_earth != NULL) planet_vertex_shader_earth->Release(); 
	if(planet_pixel_shader_earth != NULL) planet_pixel_shader_earth->Release(); 

	if(sphere_vertex_buffer != NULL) sphere_vertex_buffer->Release();
	if(sphere_index_buffer != NULL) sphere_index_buffer->Release();
	if(sphere_vertex_buffer_low != NULL) sphere_vertex_buffer_low->Release();
	if(sphere_index_buffer_low != NULL) sphere_index_buffer_low->Release();
	if(ring_vertex_buffer != NULL) ring_vertex_buffer->Release();
	d3dfont->Release();

	delete[] entrys;

	if(d3d_device != NULL)
		d3d_device->Release();
	if(d3d != NULL)
		d3d->Release();
}

int HPTGraphicsEngineInternal::LoadHGF(TCHAR *name)
{

	HANDLE filehandle = CreateFile(name,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(filehandle == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL,name,_T("could not open HGF file"),MB_OK);
		return 0;
	}
	HANDLE filemapping = CreateFileMapping(filehandle,NULL,PAGE_READONLY | SEC_COMMIT,0,0,NULL);
	if(filemapping == NULL)
	{
		MessageBox(NULL,_T("name"),_T("could not map requested HGF file"),MB_OK);
		return 0;
	}
	void *filepointer = MapViewOfFile(filemapping,FILE_MAP_READ,0,0,0);
	if(filepointer == NULL)
	{
		MessageBox(NULL,_T("name"),_T("could not view requested HGF file"),MB_OK);
		return 0;
	}
	char *fptemp = (char*)filepointer;
	char temp[3];
	memcpy(temp,filepointer,3);
	if((temp[0] != 'H') || (temp[1] != 'G') || (temp[2] != 'F'))
	{
		MessageBox(NULL,_T("requested file is not a HGF file"),name,MB_OK);
		UnmapViewOfFile(filepointer);
		CloseHandle(filemapping);
		CloseHandle(filehandle);
		return 0;
	}
	fptemp += 3;
	int tempi;
	memcpy(&tempi,fptemp,4);
	if(tempi != 1)
	{
		MessageBox(NULL,_T("requested file is a HGF file version that is not recognized by this version of the HPT Graphics Engine"),name,MB_OK);
		UnmapViewOfFile(filepointer);
		CloseHandle(filemapping);
		CloseHandle(filehandle);
		return 0;
	}
	fptemp += sizeof(int);

	memcpy(&num_entrys,fptemp,4);
	entrys = new HPTTextureStruct[num_entrys];
	if(entrys == NULL) MessageBox(NULL,_T("Out of memory while loading HGF file"),_T("ERROR"),MB_OK);
	fptemp += sizeof(int);
		
	for(int count = 0;count < num_entrys;count++)
	{
		memcpy(&entrys[count].width,fptemp,4);
		//_stprintf(temps,_T("%d"),entrys[count].width);
		//MessageBox(NULL,temps,_T("error"),MB_OK);
		fptemp += sizeof(int);
		memcpy(&entrys[count].height,fptemp,4);
		fptemp += sizeof(int);
//		memcpy(&entrys[count].total_width,fptemp,4);
		fptemp += sizeof(int);
//		memcpy(&entrys[count].total_height,fptemp,4);
		fptemp += sizeof(int);
//		memcpy(&color_key_red,fptemp,4);
		fptemp += sizeof(int);
//		memcpy(&color_key_green,fptemp,4);
		fptemp += sizeof(int);
//		memcpy(&color_key_blue,fptemp,4);
		fptemp += sizeof(int);
//		entrys[count].color_key = 0;
//		entrys[count].color_key += color_key_red>>3;
/*		if(gengine->GetScreenFormat() == FORMAT565)
		{
			entrys[count].color_key = entrys[count].color_key<<6;
			entrys[count].color_key += color_key_green>>2;
		//	color_key = color_key<<6;
		}
		else
		{
			entrys[count].color_key = entrys[count].color_key<<5;
			entrys[count].color_key += color_key_green>>3;
			//color_key = color_key<<5;
		}
		entrys[count].color_key = entrys[count].color_key<<5;
		entrys[count].color_key += color_key_blue>>3;
*/
//		memcpy(&entrys[count].default_frame_rate,fptemp,4);
		fptemp += sizeof(float);
//		memcpy(&entrys[count].default_auto_animate,fptemp,sizeof(bool));
		fptemp += sizeof(bool);
		memcpy(&entrys[count].num_frame_sets,fptemp,sizeof(int));
		fptemp += sizeof(int);
		entrys[count].frames_per_set = new int[entrys[count].num_frame_sets];
		if(entrys[count].frames_per_set == NULL) MessageBox(NULL,_T("Out of memory while loading HGF file"),_T("ERROR"),MB_OK);
		for(int count2 = 0;count2 < entrys[count].num_frame_sets;count2++)
		{
			memcpy(&(entrys[count].frames_per_set[count2]),fptemp,sizeof(int));
			fptemp += sizeof(int);
		}
		//MessageBox(NULL,_T("mapped file"),_T("error"),MB_OK);
		int type;
		memcpy(&type,fptemp,sizeof(int));
		fptemp += sizeof(int);
		//MessageBox(NULL,_T("mapped file"),_T("error"),MB_OK);
		// 1=bmp  2=png  3=jpg
		unsigned char* tempd = (unsigned char*)fptemp;
		//MessageBox(NULL,_T("before"),_T("before"),MB_OK);
		//if(type == 1) ReadBitmap(count,tempd);
		if(type == 2)
		{
			//MessageBox(NULL,_T("before"),_T("before"),MB_OK);
			fptemp += entrys[count].ReadPNG(tempd);
			//MessageBox(NULL,_T("after"),_T("after"),MB_OK);
		}
		//fptemp = (char*)tempd;
	
	}


	UnmapViewOfFile(filepointer);
	CloseHandle(filemapping);
	CloseHandle(filehandle);





	return 0;
}
void HPTGraphicsEngineInternal::Release()
{
	delete this;

}

HPT3DObject* HPTGraphicsEngineInternal::Create3DObject()
{
	return new HPT3DObjectInternal();;
}

HPT3DObjectP* HPTGraphicsEngineInternal::Create3DObjectP()
{
	return new HPT3DObjectPInternal();;
}

void HPTGraphicsEngineInternal::BeginFrame()
{
	d3d_device->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,0,1.0f,0);
	d3d_device->BeginScene();
}

void HPTGraphicsEngineInternal::EndFrame()
{
	d3d_device->EndScene();
	d3d_device->Present(NULL,NULL,NULL,NULL);
}


//HPTGraphicsEngine& operator<<(HPTFont1* arg) = 0;
HPTGraphicsEngine& HPTGraphicsEngineInternal::operator<<(char *arg)
{
	d3d_device->SetPixelShader(NULL);
	d3d_device->SetVertexShader(NULL);
	RECT tempr;
	tempr.left = text_positionx;
	tempr.top = text_positiony;
	tempr.right = tempr.left + 1;
	tempr.bottom = tempr.top + 1;
	//d3dfont->Begin();
	d3dfont->DrawText(NULL,arg,-1,&tempr,DT_LEFT | DT_NOCLIP | DT_SINGLELINE,D3DXCOLOR(255,255,255,255));
	//d3dfont->End();
	ChangeGraphicsState(-1);
	return *this;
}

HPTGraphicsEngine& HPTGraphicsEngineInternal::operator<<(int arg)
{
	return *this;
	
}

HPTGraphicsEngine& HPTGraphicsEngineInternal::operator<<(unsigned int arg)
{
	return *this;
	
}

HPTGraphicsEngine& HPTGraphicsEngineInternal::operator<<(double arg)
{
		char temp[30];
	sprintf(temp,"%f",arg);
	int pos = 0;
	for(unsigned int count = 0;count < strlen(temp);count++)
	{
		if(temp[count] != '0') pos = count;
	}
	temp[pos+1] = 0;
	(*this) << temp;

	return *this;

}


HPTGraphicsEngine& HPTGraphicsEngineInternal::Position(int x,int y)
{
	text_positionx = x;
	text_positiony = y;

	return *this;

}

void HPTGraphicsEngineInternal::CreateSpriteVertexBuffer()
{
	sprite_vertexs[0].x = -1.5f;
	sprite_vertexs[0].y = -1.5f;
	sprite_vertexs[0].z = 0;
	sprite_vertexs[0].tu = 0;
	sprite_vertexs[0].tv = 0;

	sprite_vertexs[3].x = -1.5f;
	sprite_vertexs[3].y = 1.5f;
	sprite_vertexs[3].z = 0;
	sprite_vertexs[3].tu = 0;
	sprite_vertexs[3].tv = 1;

	sprite_vertexs[2].x = 1.5f;
	sprite_vertexs[2].y = 1.5f;
	sprite_vertexs[2].z = 0;
	sprite_vertexs[2].tu = 1;
	sprite_vertexs[2].tv = 1;

	sprite_vertexs[1].x = 1.5f;
	sprite_vertexs[1].y = -1.5f;
	sprite_vertexs[1].z = 0;
	sprite_vertexs[1].tu = 1;
	sprite_vertexs[1].tv = 0;

	SpriteVertex* tempv;
	d3d_device->CreateVertexBuffer(4 * sizeof(SpriteVertex),D3DUSAGE_WRITEONLY,0,D3DPOOL_DEFAULT,&sprite_vertex_buffer,NULL);
	sprite_vertex_buffer->Lock(0,0,(void**)&tempv,0/*D3DLOCK_DISCARD*/);
	memcpy(tempv,sprite_vertexs,4*sizeof(SpriteVertex));
	sprite_vertex_buffer->Unlock();


	D3DVERTEXELEMENT9 decl[] = 
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		D3DDECL_END()
	};

	d3d_device->CreateVertexDeclaration(decl,&sprite_vertex_decl);


}

void HPTGraphicsEngineInternal::RenderSprite(D3DXMATRIX& world,int texture,D3DXVECTOR4& color)
{
	D3DXMATRIX temp;
/*	D3DXMatrixIdentity(&temp);
	D3DXMatrixMultiply(&temp,&temp,&world);
	D3DXMatrixMultiply(&temp,&temp,&matproj);
	D3DXMatrixTranspose( &temp, &temp );
*/
	D3DXMatrixMultiply(&temp,&world,&matproj);
	D3DXMatrixTranspose( &temp, &temp );

	ChangeGraphicsState(0);
	d3d_device->SetVertexShaderConstantF(30,color,1);
	d3d_device->SetVertexShaderConstantF(0,temp,4);
	d3d_device->SetTexture(0,entrys[texture].texture);
	d3d_device->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
}

void HPTGraphicsEngineInternal::RenderRing(D3DXMATRIX& world,int texture,D3DXVECTOR3& light)
{
   d3d_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);
 	D3DXMATRIX temp;
	D3DXMatrixIdentity(&temp);
	D3DXMatrixMultiply(&temp,&temp,&world);
//	D3DXMatrixMultiply(&temp,&temp,&matview);
	D3DXMatrixMultiply(&temp,&temp,&matproj);
	D3DXMatrixTranspose( &temp, &temp );
	
	ChangeGraphicsState(3);
	d3d_device->SetVertexShaderConstantF(30,D3DXVECTOR4(1,1,1,1),1);
	d3d_device->SetVertexShaderConstantF(0,temp,4);
	d3d_device->SetTexture(0,entrys[texture].texture);
	d3d_device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,ring_size);
   d3d_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW);
 
}

void HPTGraphicsEngineInternal::RenderSphere(D3DXMATRIX& world,int texture,int texture2,D3DXVECTOR4& color,float np)
{
	D3DXMATRIX temp;
	
	D3DXMatrixMultiply(&temp,&world,&matview);
	D3DXMatrixMultiply(&temp,&temp,&matproj);
	D3DXMatrixTranspose( &temp, &temp );


	ChangeGraphicsState(1);
	d3d_device->SetVertexShaderConstantF(12,D3DXVECTOR4(np,np,np,np),1);
	d3d_device->SetVertexShaderConstantF(30,color,1);
	d3d_device->SetVertexShaderConstantF(0,temp,4);

	D3DXMatrixMultiply(&temp,&world,&matview);
	D3DXMatrixInverse(&temp,NULL, &temp);
	D3DXMatrixTranspose( &temp, &temp );
	
	d3d_device->SetVertexShaderConstantF(4,temp,4);
	d3d_device->SetTexture(0,entrys[texture].texture);
	d3d_device->SetTexture(1,entrys[texture2].texture);
	d3d_device->SetIndices(sphere_index_buffer);
	d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,sphere_v_size,0,sphere_size);

}

void HPTGraphicsEngineInternal::RenderSphereL(D3DXMATRIX& world,int texture,int texture2,D3DXVECTOR4& color,float np)
{
	D3DXMATRIX temp;
	
	D3DXMatrixMultiply(&temp,&world,&matview);
	D3DXMatrixMultiply(&temp,&temp,&matproj);
	D3DXMatrixTranspose( &temp, &temp );


	ChangeGraphicsState(5);
	d3d_device->SetVertexShaderConstantF(12,D3DXVECTOR4(np,np,np,np),1);
	d3d_device->SetVertexShaderConstantF(30,color,1);
	d3d_device->SetVertexShaderConstantF(0,temp,4);

	D3DXMatrixMultiply(&temp,&world,&matview);
	D3DXMatrixInverse(&temp,NULL, &temp);
	D3DXMatrixTranspose( &temp, &temp );
	
	d3d_device->SetVertexShaderConstantF(4,temp,4);
	d3d_device->SetTexture(0,entrys[texture].texture);
//	d3d_device->SetTexture(1,entrys[texture2].texture);
	
	
	
	d3d_device->SetIndices(sphere_index_buffer_low);
	d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,sphere_v_size_low,0,sphere_size_low);

}

void HPTGraphicsEngineInternal::RenderSphereP(D3DXMATRIX& world,int texture,int texture2,D3DXVECTOR3& light,float sky_pos,int night,float diffuse,float specular)
{
	D3DXMATRIX temp;
	D3DXMatrixIdentity(&temp);
	D3DXMatrixMultiply(&temp,&temp,&world);
	D3DXMatrixMultiply(&temp,&temp,&matproj);
	D3DXMatrixTranspose( &temp, &temp );
	
	ChangeGraphicsState(2);
	d3d_device->SetVertexShaderConstantF(10,D3DXVECTOR4(0.05f,0.05f,0.05f,1.0f),1);
	d3d_device->SetVertexShaderConstantF(11,light,1);
	d3d_device->SetVertexShaderConstantF(12,D3DXVECTOR4(sky_pos,0,0,0),1);
	d3d_device->SetVertexShaderConstantF(14,D3DXVECTOR4(0,0,-1,1),1);
	d3d_device->SetVertexShaderConstantF(20,D3DXVECTOR4(0,0,0,0),1);
	d3d_device->SetVertexShaderConstantF(21,D3DXVECTOR4(1,1,1,1),1);
	d3d_device->SetVertexShaderConstantF(22,D3DXVECTOR4(0.5f,0.5f,0.5f,1),1);
	d3d_device->SetPixelShaderConstantF(2,D3DXVECTOR4(0.05f,0.05f,0.05f,1.0f),1);
	d3d_device->SetPixelShaderConstantF(0,D3DXVECTOR4(0,0,0,0),1);
	d3d_device->SetPixelShaderConstantF(1,D3DXVECTOR4(1,1,1,1),1);
	d3d_device->SetPixelShaderConstantF(3,D3DXVECTOR4(diffuse,diffuse,diffuse,1.0f),1);
	d3d_device->SetPixelShaderConstantF(4,D3DXVECTOR4(specular,specular,specular,1.0f),1);
	d3d_device->SetPixelShaderConstantF(5,D3DXVECTOR4(0.6f,0.6f,0.6f,1.0f),1);
	d3d_device->SetVertexShaderConstantF(0,temp,4);
	d3d_device->SetTexture(0,entrys[texture].texture);
	d3d_device->SetTexture(1,entrys[texture2].texture);
	d3d_device->SetTexture(2,entrys[night].texture);
	
	//D3DXMatrixMultiply(&temp,&world,&matview);
	D3DXMatrixInverse(&temp,NULL, &world);
	D3DXMatrixTranspose( &temp, &temp );
	
	d3d_device->SetVertexShaderConstantF(4,temp,4);

	
	d3d_device->SetIndices(sphere_index_buffer);
	d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,sphere_v_size,0,sphere_size);
}

void HPTGraphicsEngineInternal::RenderSphereGas(D3DXMATRIX& world,int texture,int texture2,D3DXVECTOR3& light,float sky_pos,int night,float diffuse,float specular)
{
	D3DXMATRIX temp;
	D3DXMatrixIdentity(&temp);
	D3DXMatrixMultiply(&temp,&temp,&world);
	D3DXMatrixMultiply(&temp,&temp,&matproj);
	D3DXMatrixTranspose( &temp, &temp );
	
	ChangeGraphicsState(6);
	d3d_device->SetVertexShaderConstantF(10,D3DXVECTOR4(0.05f,0.05f,0.05f,1.0f),1);
	d3d_device->SetVertexShaderConstantF(11,light,1);
	d3d_device->SetVertexShaderConstantF(12,D3DXVECTOR4(sky_pos,0,0,0),1);
	d3d_device->SetVertexShaderConstantF(14,D3DXVECTOR4(0,0,-1,1),1);
	d3d_device->SetVertexShaderConstantF(20,D3DXVECTOR4(0,0,0,0),1);
	d3d_device->SetVertexShaderConstantF(21,D3DXVECTOR4(1,1,1,1),1);
	d3d_device->SetVertexShaderConstantF(22,D3DXVECTOR4(0.5f,0.5f,0.5f,1),1);
	d3d_device->SetPixelShaderConstantF(2,D3DXVECTOR4(0.05f,0.05f,0.05f,1.0f),1);
	d3d_device->SetPixelShaderConstantF(0,D3DXVECTOR4(0,0,0,0),1);
	d3d_device->SetPixelShaderConstantF(1,D3DXVECTOR4(1,1,1,1),1);
	d3d_device->SetPixelShaderConstantF(3,D3DXVECTOR4(diffuse,diffuse,diffuse,1.0f),1);
	d3d_device->SetPixelShaderConstantF(4,D3DXVECTOR4(specular,specular,specular,1.0f),1);
	d3d_device->SetPixelShaderConstantF(5,D3DXVECTOR4(0.6f,0.6f,0.6f,1.0f),1);
	d3d_device->SetVertexShaderConstantF(0,temp,4);
	d3d_device->SetTexture(0,entrys[texture].texture);
	d3d_device->SetTexture(1,entrys[texture2].texture);
	d3d_device->SetTexture(2,entrys[night].texture);
	
	//D3DXMatrixMultiply(&temp,&world,&matview);
	D3DXMatrixInverse(&temp,NULL, &world);
	D3DXMatrixTranspose( &temp, &temp );
	
	d3d_device->SetVertexShaderConstantF(4,temp,4);
	
	
	d3d_device->SetIndices(sphere_index_buffer);
	d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,sphere_v_size,0,sphere_size);
}
void HPTGraphicsEngineInternal::RenderSphereSky(D3DXMATRIX& world,int texture,int texture2,D3DXVECTOR3& light,float sky_pos,int night,float diffuse,float specular)
{
	D3DXMATRIX temp;
	D3DXMatrixIdentity(&temp);
	D3DXMatrixMultiply(&temp,&temp,&world);
	D3DXMatrixMultiply(&temp,&temp,&matproj);
	D3DXMatrixTranspose( &temp, &temp );
	
	ChangeGraphicsState(7);
	d3d_device->SetVertexShaderConstantF(10,D3DXVECTOR4(0.05f,0.05f,0.05f,1.0f),1);
	d3d_device->SetVertexShaderConstantF(11,light,1);
	d3d_device->SetVertexShaderConstantF(12,D3DXVECTOR4(sky_pos,0,0,0),1);
	d3d_device->SetVertexShaderConstantF(14,D3DXVECTOR4(0,0,-1,1),1);
	d3d_device->SetVertexShaderConstantF(20,D3DXVECTOR4(0,0,0,0),1);
	d3d_device->SetVertexShaderConstantF(21,D3DXVECTOR4(1,1,1,1),1);
	d3d_device->SetVertexShaderConstantF(22,D3DXVECTOR4(0.5f,0.5f,0.5f,1),1);
	d3d_device->SetPixelShaderConstantF(2,D3DXVECTOR4(0.05f,0.05f,0.05f,1.0f),1);
	d3d_device->SetPixelShaderConstantF(0,D3DXVECTOR4(0,0,0,0),1);
	d3d_device->SetPixelShaderConstantF(1,D3DXVECTOR4(1,1,1,1),1);
	d3d_device->SetPixelShaderConstantF(3,D3DXVECTOR4(diffuse,diffuse,diffuse,1.0f),1);
	d3d_device->SetPixelShaderConstantF(4,D3DXVECTOR4(specular,specular,specular,1.0f),1);
	d3d_device->SetPixelShaderConstantF(5,D3DXVECTOR4(0.6f,0.6f,0.6f,1.0f),1);
	d3d_device->SetVertexShaderConstantF(0,temp,4);
	d3d_device->SetTexture(0,entrys[texture].texture);
	d3d_device->SetTexture(1,entrys[texture2].texture);
	d3d_device->SetTexture(2,entrys[night].texture);
	
	
	//D3DXMatrixMultiply(&temp,&world,&matview);
	D3DXMatrixInverse(&temp,NULL, &world);
	D3DXMatrixTranspose( &temp, &temp );
	
	d3d_device->SetVertexShaderConstantF(4,temp,4);

	
	d3d_device->SetIndices(sphere_index_buffer);
	d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,sphere_v_size,0,sphere_size);
}

void HPTGraphicsEngineInternal::RenderSphereE(D3DXMATRIX& world,int texture,int texture2,D3DXVECTOR3& light,float sky_pos,int night,float diffuse,float specular)
{
/*	d3d_device->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_BLENDTEXTUREALPHA);
//	d3d_device->SetTextureStageState(1,D3DTSS_ALPHAOP,D3DTOP_BLENDTEXTUREALPHA);
	d3d_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
	d3d_device->SetTextureStageState(2,D3DTSS_COLOROP,D3DTOP_MODULATE);
*/
	D3DXMATRIX temp;
	D3DXMatrixIdentity(&temp);
	D3DXMatrixMultiply(&temp,&temp,&world);
//	D3DXMatrixMultiply(&temp,&temp,&matview);
	D3DXMatrixMultiply(&temp,&temp,&matproj);
	D3DXMatrixTranspose( &temp, &temp );

	
	

	ChangeGraphicsState(4);
	d3d_device->SetVertexShaderConstantF(10,D3DXVECTOR4(0.05f,0.05f,0.05f,1.0f),1);
	d3d_device->SetVertexShaderConstantF(11,light,1);
	d3d_device->SetVertexShaderConstantF(12,D3DXVECTOR4(sky_pos,0,0,0),1);
	d3d_device->SetVertexShaderConstantF(14,D3DXVECTOR4(0,0,-1,1),1);
	d3d_device->SetVertexShaderConstantF(20,D3DXVECTOR4(0,0,0,0),1);
	d3d_device->SetVertexShaderConstantF(21,D3DXVECTOR4(1,1,1,1),1);
	d3d_device->SetVertexShaderConstantF(22,D3DXVECTOR4(0.5f,0.5f,0.5f,1),1);
	d3d_device->SetPixelShaderConstantF(2,D3DXVECTOR4(0.05f,0.05f,0.05f,1.0f),1);
	d3d_device->SetPixelShaderConstantF(0,D3DXVECTOR4(0,0,0,0),1);
	d3d_device->SetPixelShaderConstantF(1,D3DXVECTOR4(1,1,1,1),1);
	d3d_device->SetPixelShaderConstantF(3,D3DXVECTOR4(0.8f,0.8f,0.8f,1.0f),1);
	d3d_device->SetPixelShaderConstantF(4,D3DXVECTOR4(0.4f,0.4f,0.4f,1.0f),1);
	d3d_device->SetPixelShaderConstantF(5,D3DXVECTOR4(0.6f,0.6f,0.6f,1.0f),1);
	d3d_device->SetVertexShaderConstantF(0,temp,4);
	d3d_device->SetTexture(0,entrys[texture].texture);
	d3d_device->SetTexture(1,entrys[texture2].texture);
	d3d_device->SetTexture(2,entrys[night].texture);

	//D3DXMatrixMultiply(&temp,&world,&matview);
	D3DXMatrixInverse(&temp,NULL, &world);
	D3DXMatrixTranspose( &temp, &temp );
	
	d3d_device->SetVertexShaderConstantF(4,temp,4);

	
	d3d_device->SetIndices(sphere_index_buffer);
	d3d_device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,sphere_v_size,0,sphere_size);
//	d3d_device->DrawPrimitive(D3DPT_POINTLIST,0,sphere_v_size);

/*	d3d_device->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_DISABLE);
	d3d_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
*/
}

void HPTGraphicsEngineInternal::CreateSphereVertexBuffer()
{
	vector<SpriteVertex> buffer;
	vector<unsigned short> ibuffer;

/*	for(float theta = 0;theta <= PI;theta += PI/SPHERE_CUTS)
	{
		buffer.push_back(SpriteVertex(0,cos(theta-PI),sin(theta),0.5f,(cos(theta)+1)/2));

	}
*/
	float cd,ct,sd,st;
	float am = PI/sphere_cuts;
	float pinv = 1/PI;
	float p2inv = 1/(2*PI);
	for(float delta = 0;delta <= 2*PI+0.1f;delta += am)
	{
		cd = cos(delta);
		sd = sin(delta);
		for(float theta = PI;theta >= -0.1f;theta -= am)
		{
			ct = cos(theta);
			st = sin(theta);
			buffer.push_back(SpriteVertex(st*sd,ct,-st*cd,  //position
				delta*p2inv,(theta)*pinv,   //texture
				st*cd,0,st*sd,   //tangent
				ct*sd,-st,-ct*cd));  //binormal
			if(theta == PI) continue;
			if(delta == 0) continue;
			ibuffer.push_back((buffer.size()-1)-1-(sphere_cuts+1));
			ibuffer.push_back((buffer.size()-1));
			ibuffer.push_back((buffer.size()-1)-(sphere_cuts+1));
			ibuffer.push_back((buffer.size()-1)-1-(sphere_cuts+1));
			ibuffer.push_back((buffer.size()-1)-1);
			ibuffer.push_back((buffer.size()-1));

		}
	}
	sphere_size = ibuffer.size()/3;
	sphere_v_size = buffer.size();

	SpriteVertex* tempv;

	d3d_device->CreateVertexBuffer(buffer.size() * sizeof(SpriteVertex),D3DUSAGE_WRITEONLY,0,D3DPOOL_DEFAULT,&sphere_vertex_buffer,NULL);
	sphere_vertex_buffer->Lock(0,0,(void**)&tempv,0/*D3DLOCK_DISCARD*/);
	memcpy(tempv,&(buffer[0]),buffer.size()*sizeof(SpriteVertex));
	sphere_vertex_buffer->Unlock();

	unsigned short* tempi;
	d3d_device->CreateIndexBuffer(sphere_size*6,D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_DEFAULT,&sphere_index_buffer,NULL);
	sphere_index_buffer->Lock(0,0,(void**)(&tempi),0/*D3DLOCK_DISCARD*/);
	memcpy(tempi,&(ibuffer[0]),ibuffer.size()*2);
	sphere_index_buffer->Unlock();

}

void HPTGraphicsEngineInternal::CreateSphereVertexBufferLow()
{
	vector<SpriteVertex> buffer;
	vector<unsigned short> ibuffer;
	int sphere_cuts = 5;
/*	for(float theta = 0;theta <= PI;theta += PI/SPHERE_CUTS)
	{
		buffer.push_back(SpriteVertex(0,cos(theta-PI),sin(theta),0.5f,(cos(theta)+1)/2));

	}
*/
	float cd,ct,sd,st;
	float am = PI/sphere_cuts;
	float pinv = 1/PI;
	float p2inv = 1/(2*PI);
	for(float delta = 0;delta <= 2*PI+0.1f;delta += am)
	{
		cd = cos(delta);
		sd = sin(delta);
		for(float theta = PI;theta >= -0.1f;theta -= am)
		{
			ct = cos(theta);
			st = sin(theta);
			buffer.push_back(SpriteVertex(st*sd,ct,-st*cd,  //position
				delta*p2inv,(theta)*pinv,   //texture
				st*cd,0,st*sd,   //tangent
				ct*sd,-st,-ct*cd));  //binormal
			if(theta == PI) continue;
			if(delta == 0) continue;
			ibuffer.push_back((buffer.size()-1)-1-(sphere_cuts+1));
			ibuffer.push_back((buffer.size()-1));
			ibuffer.push_back((buffer.size()-1)-(sphere_cuts+1));
			ibuffer.push_back((buffer.size()-1)-1-(sphere_cuts+1));
			ibuffer.push_back((buffer.size()-1)-1);
			ibuffer.push_back((buffer.size()-1));

		}
	}
	sphere_size_low = ibuffer.size()/3;
	sphere_v_size_low = buffer.size();

	SpriteVertex* tempv;

	d3d_device->CreateVertexBuffer(buffer.size() * sizeof(SpriteVertex),D3DUSAGE_WRITEONLY,0,D3DPOOL_DEFAULT,&sphere_vertex_buffer_low,NULL);
	sphere_vertex_buffer_low->Lock(0,0,(void**)&tempv,0/*D3DLOCK_DISCARD*/);
	memcpy(tempv,&(buffer[0]),buffer.size()*sizeof(SpriteVertex));
	sphere_vertex_buffer_low->Unlock();

	unsigned short* tempi;
	d3d_device->CreateIndexBuffer(sphere_size_low*6,D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_DEFAULT,&sphere_index_buffer_low,NULL);
	sphere_index_buffer_low->Lock(0,0,(void**)(&tempi),0/*D3DLOCK_DISCARD*/);
	memcpy(tempi,&(ibuffer[0]),ibuffer.size()*2);
	sphere_index_buffer_low->Unlock();

}

void HPTGraphicsEngineInternal::CreateRingVertexBuffer()
{
	vector<SpriteVertex> buffer;

	float cd,sd,aa;
	aa = PI/ring_cuts;
	for(float delta = 0;delta <= 2*PI+0.1f;delta += aa)
	{
		cd = cos(delta);
		sd = sin(delta);
			buffer.push_back(SpriteVertex(cd*1.2f,0,sd*1.2f,1,0,0,0,0,0,0,0));
			buffer.push_back(SpriteVertex(cd*2.2f,0,sd*2.2f,0,0,0,0,0,0,0,0));

	}
	ring_size = buffer.size() - 2;

	SpriteVertex* tempv;

	d3d_device->CreateVertexBuffer(buffer.size() * sizeof(SpriteVertex),D3DUSAGE_WRITEONLY,0,D3DPOOL_DEFAULT,&ring_vertex_buffer,NULL);
	ring_vertex_buffer->Lock(0,0,(void**)&tempv,0/*D3DLOCK_DISCARD*/);
	memcpy(tempv,&(buffer[0]),buffer.size()*sizeof(SpriteVertex));
	ring_vertex_buffer->Unlock();

}


void HPTGraphicsEngineInternal::ChangeGraphicsState(int arg)
{
	if(graphics_state == arg) return;
	if(render_quality == 0)
	{
		switch(arg)
		{
		case 0:
			d3d_device->SetVertexShader(sprite_vertex_shader);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sprite_vertex_buffer,0,sizeof(SpriteVertex));
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
			break;
		case 1:
			d3d_device->SetVertexShader(sprite_vertex_shader);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sphere_vertex_buffer,0,sizeof(SpriteVertex));
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
			break;
		case 2:	
			d3d_device->SetVertexShader(planet_vertex_shader);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sphere_vertex_buffer,0,sizeof(SpriteVertex));
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
			break;
		case 3:
			d3d_device->SetVertexShader(sprite_vertex_shader);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,ring_vertex_buffer,0,sizeof(SpriteVertex));
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
			break;
		case 4:	
			d3d_device->SetVertexShader(planet_vertex_shader);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sphere_vertex_buffer,0,sizeof(SpriteVertex));
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
			break;
		case 5:
			d3d_device->SetVertexShader(sprite_vertex_shader);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sphere_vertex_buffer_low,0,sizeof(SpriteVertex));
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
			break;
		case 6:	
			d3d_device->SetVertexShader(planet_vertex_shader);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sphere_vertex_buffer,0,sizeof(SpriteVertex));
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
			break;
		case 7:	
			d3d_device->SetVertexShader(planet_vertex_shader);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sphere_vertex_buffer,0,sizeof(SpriteVertex));
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
			break;

		}
	}
	if(render_quality == 1)
	{
//	OutputDebugString("change state");

		switch(arg)
		{
		case 0: //sprite
			d3d_device->SetVertexShader(sprite_vertex_shader);
			d3d_device->SetPixelShader(NULL);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sprite_vertex_buffer,0,sizeof(SpriteVertex));
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
			break;
		case 1: //star
			d3d_device->SetVertexShader(star_vertex_shader);
			d3d_device->SetPixelShader(star_pixel_shader);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sphere_vertex_buffer,0,sizeof(SpriteVertex));
			d3d_device->SetVertexShaderConstantF(10,D3DXVECTOR4(0.15f,0.15f,0.15f,1.0f),1);
			d3d_device->SetVertexShaderConstantF(14,D3DXVECTOR4(0.0f,0.0f,-1.0f,1.0f),1);
			d3d_device->SetVertexShaderConstantF(22,D3DXVECTOR4(0.5f,0.5f,0.5f,1.0f),1);
			d3d_device->SetPixelShaderConstantF(0,D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f),1);
			d3d_device->SetPixelShaderConstantF(1,D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f),1);
			d3d_device->SetPixelShaderConstantF(2,D3DXVECTOR4(0.2f,0.2f,0.2f,1.0f),1);
			d3d_device->SetPixelShaderConstantF(3,D3DXVECTOR4(0.1f,0.1f,0.1f,1.0f),1);
			break;
		case 2:	  //planet
			d3d_device->SetVertexShader(planet_vertex_shader_med);
			d3d_device->SetPixelShader(planet_pixel_shader_med);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sphere_vertex_buffer,0,sizeof(SpriteVertex));
			break;
		case 3:  //ring
			d3d_device->SetVertexShader(sprite_vertex_shader);
			d3d_device->SetPixelShader(NULL);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,ring_vertex_buffer,0,sizeof(SpriteVertex));
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
			break;
		case 4:	  //planet
			d3d_device->SetVertexShader(planet_vertex_shader_med);
			d3d_device->SetPixelShader(planet_pixel_shader_med);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sphere_vertex_buffer,0,sizeof(SpriteVertex));
			break;
		case 5: //star
			d3d_device->SetVertexShader(star_vertex_shader);
			d3d_device->SetPixelShader(star_pixel_shader);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sphere_vertex_buffer_low,0,sizeof(SpriteVertex));
			d3d_device->SetVertexShaderConstantF(10,D3DXVECTOR4(0.15f,0.15f,0.15f,1.0f),1);
			d3d_device->SetVertexShaderConstantF(14,D3DXVECTOR4(0.0f,0.0f,-1.0f,1.0f),1);
			d3d_device->SetVertexShaderConstantF(22,D3DXVECTOR4(0.5f,0.5f,0.5f,1.0f),1);
			d3d_device->SetPixelShaderConstantF(0,D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f),1);
			d3d_device->SetPixelShaderConstantF(1,D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f),1);
			d3d_device->SetPixelShaderConstantF(2,D3DXVECTOR4(0.2f,0.2f,0.2f,1.0f),1);
			d3d_device->SetPixelShaderConstantF(3,D3DXVECTOR4(0.1f,0.1f,0.1f,1.0f),1);
			break;
		case 6:	  //gas
			d3d_device->SetVertexShader(planet_vertex_shader_med);
			d3d_device->SetPixelShader(planet_pixel_shader_med);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sphere_vertex_buffer,0,sizeof(SpriteVertex));
			break;
		case 7:	  //sky
			d3d_device->SetVertexShader(planet_vertex_shader_med);
			d3d_device->SetPixelShader(planet_pixel_shader_med);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sphere_vertex_buffer,0,sizeof(SpriteVertex));
			break;

		}
	}	
	if(render_quality == 2)
	{
//	OutputDebugString("change state");

		switch(arg)
		{
		case 0: //sprite
			d3d_device->SetVertexShader(sprite_vertex_shader);
			d3d_device->SetPixelShader(NULL);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sprite_vertex_buffer,0,sizeof(SpriteVertex));
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
			break;
		case 1: //star
			d3d_device->SetVertexShader(star_vertex_shader_high);
			d3d_device->SetPixelShader(star_pixel_shader_high);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sphere_vertex_buffer,0,sizeof(SpriteVertex));
			d3d_device->SetVertexShaderConstantF(10,D3DXVECTOR4(0.15f,0.15f,0.15f,1.0f),1);
			d3d_device->SetVertexShaderConstantF(20,D3DXVECTOR4(0,0,0,0),1);
			d3d_device->SetVertexShaderConstantF(21,D3DXVECTOR4(1,1,1,1),1);
			d3d_device->SetVertexShaderConstantF(14,D3DXVECTOR4(0.0f,0.0f,-1.0f,1.0f),1);
			d3d_device->SetVertexShaderConstantF(22,D3DXVECTOR4(0.5f,0.5f,0.5f,1.0f),1);
			d3d_device->SetVertexShaderConstantF(32,D3DXVECTOR4(0.5f,0.5f,0.5f,1.0f),1);
			d3d_device->SetPixelShaderConstantF(0,D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f),1);
			d3d_device->SetPixelShaderConstantF(1,D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f),1);
			d3d_device->SetPixelShaderConstantF(2,D3DXVECTOR4(0.2f,0.2f,0.2f,1.0f),1);
			d3d_device->SetPixelShaderConstantF(3,D3DXVECTOR4(0.1f,0.1f,0.1f,1.0f),1);
			d3d_device->SetPixelShaderConstantF(4,D3DXVECTOR4(0.5f,0.5f,0.5f,1.0f),1);
			//OutputDebugString("change state 1");
			break;
		case 2:	  //planet
			d3d_device->SetVertexShader(planet_vertex_shader_high);
			d3d_device->SetPixelShader(planet_pixel_shader_high);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sphere_vertex_buffer,0,sizeof(SpriteVertex));
			break;
		case 3:  //ring
			d3d_device->SetVertexShader(sprite_vertex_shader);
			d3d_device->SetPixelShader(NULL);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,ring_vertex_buffer,0,sizeof(SpriteVertex));
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
			d3d_device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
			break;
		case 4:	  //planet
			d3d_device->SetVertexShader(planet_vertex_shader_earth);
			d3d_device->SetPixelShader(planet_pixel_shader_earth);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sphere_vertex_buffer,0,sizeof(SpriteVertex));
			break;
		case 5: //star
			d3d_device->SetVertexShader(star_vertex_shader);
			d3d_device->SetPixelShader(star_pixel_shader);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sphere_vertex_buffer_low,0,sizeof(SpriteVertex));
			d3d_device->SetVertexShaderConstantF(10,D3DXVECTOR4(0.15f,0.15f,0.15f,1.0f),1);
			d3d_device->SetVertexShaderConstantF(14,D3DXVECTOR4(0.0f,0.0f,-1.0f,1.0f),1);
			d3d_device->SetVertexShaderConstantF(22,D3DXVECTOR4(0.5f,0.5f,0.5f,1.0f),1);
			d3d_device->SetPixelShaderConstantF(0,D3DXVECTOR4(0.0f,0.0f,0.0f,0.0f),1);
			d3d_device->SetPixelShaderConstantF(1,D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f),1);
			d3d_device->SetPixelShaderConstantF(2,D3DXVECTOR4(0.2f,0.2f,0.2f,1.0f),1);
			d3d_device->SetPixelShaderConstantF(3,D3DXVECTOR4(0.1f,0.1f,0.1f,1.0f),1);
			break;
		case 6:	  //gas
			d3d_device->SetVertexShader(planet_vertex_shader_gas);
			d3d_device->SetPixelShader(planet_pixel_shader_gas);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sphere_vertex_buffer,0,sizeof(SpriteVertex));
			d3d_device->SetVertexShaderConstantF(14,D3DXVECTOR4(0.0f,0.0f,-1.0f,1.0f),1);
			break;
		case 7:	  //sky
			d3d_device->SetVertexShader(planet_vertex_shader_sky);
			d3d_device->SetPixelShader(planet_pixel_shader_sky);
			d3d_device->SetVertexDeclaration(sprite_vertex_decl);
			d3d_device->SetStreamSource(0,sphere_vertex_buffer,0,sizeof(SpriteVertex));
			d3d_device->SetVertexShaderConstantF(14,D3DXVECTOR4(0.0f,0.0f,-1.0f,1.0f),1);
			break;

		}
	}	
	graphics_state = arg;
}

void HPTGraphicsEngineInternal::LoadShaders()
{
//	LPD3DXBUFFER tempvbufer;
//	LPD3DXBUFFER emsg;
/*	if(D3DXAssembleShaderFromFile(hptstrcat(path,"sprite vertex shader.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreateVertexShader((const unsigned long*)tempvbufer->GetBufferPointer(),&sprite_vertex_shader);
*/
/*	if(D3DXAssembleShaderFromFile(hptstrcat(path,"planet vertex shader.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreateVertexShader((const unsigned long*)tempvbufer->GetBufferPointer(),&planet_vertex_shader);
*/
/*	if(D3DXAssembleShaderFromFile(hptstrcat(path,"star vertex shader.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreateVertexShader((const unsigned long*)tempvbufer->GetBufferPointer(),&star_vertex_shader);
*/
/*	if(D3DXAssembleShaderFromFile(hptstrcat(path,"star pixel shader.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreatePixelShader((const unsigned long*)tempvbufer->GetBufferPointer(),&star_pixel_shader);
*/
/*	if(D3DXAssembleShaderFromFile(hptstrcat(path,"planet vertex shader med.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreateVertexShader((const unsigned long*)tempvbufer->GetBufferPointer(),&planet_vertex_shader_med);

	if(D3DXAssembleShaderFromFile(hptstrcat(path,"planet pixel shader med.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreatePixelShader((const unsigned long*)tempvbufer->GetBufferPointer(),&planet_pixel_shader_med);
*/
/*	if(D3DXAssembleShaderFromFile(hptstrcat(path,"star vertex shader high.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreateVertexShader((const unsigned long*)tempvbufer->GetBufferPointer(),&star_vertex_shader_high);

	if(D3DXAssembleShaderFromFile(hptstrcat(path,"star pixel shader high.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreatePixelShader((const unsigned long*)tempvbufer->GetBufferPointer(),&star_pixel_shader_high);
*/
/*	if(D3DXAssembleShaderFromFile(hptstrcat(path,"planet vertex shader high.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreateVertexShader((const unsigned long*)tempvbufer->GetBufferPointer(),&planet_vertex_shader_high);

	if(D3DXAssembleShaderFromFile(hptstrcat(path,"planet pixel shader high.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreatePixelShader((const unsigned long*)tempvbufer->GetBufferPointer(),&planet_pixel_shader_high);
 */   
/*	if(D3DXAssembleShaderFromFile(hptstrcat(path,"planet vertex shader earth.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreateVertexShader((const unsigned long*)tempvbufer->GetBufferPointer(),&planet_vertex_shader_earth);

	if(D3DXAssembleShaderFromFile(hptstrcat(path,"planet pixel shader earth.txt"),NULL,NULL,0,&tempvbufer,&emsg) != D3D_OK)
	{
		MessageBox(NULL,(const char*)emsg->GetBufferPointer(),"error",MB_OK);
	}
	d3d_device->CreatePixelShader((const unsigned long*)tempvbufer->GetBufferPointer(),&planet_pixel_shader_earth);
*/


	HANDLE filehandle = CreateFile(hptstrcat(path,"shaders.hsf"),GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(filehandle == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL,hptstrcat(path,"shaders.hsf"),_T("could not open shader.hsf file"),MB_OK);
		return;
	}
	HANDLE filemapping = CreateFileMapping(filehandle,NULL,PAGE_READONLY | SEC_COMMIT,0,0,NULL);
	if(filemapping == NULL)
	{
		MessageBox(NULL,hptstrcat(path,"shaders.hsf"),_T("could not map requested HSF file"),MB_OK);
		return;
	}
	void *filepointer = MapViewOfFile(filemapping,FILE_MAP_READ,0,0,0);
	if(filepointer == NULL)
	{
		MessageBox(NULL,hptstrcat(path,"shaders.hsf"),_T("could not view requested HSF file"),MB_OK);
		return;
	}
	unsigned char *fptemp = (unsigned char*)filepointer;

	unsigned long uncompressed_size;
	memcpy(&uncompressed_size,fptemp,4);
	fptemp += 4;
	int zlib_size;
	memcpy(&zlib_size,fptemp,4);
	fptemp += 4;
	//LPD3DXBUFFER file_buffer;
	//D3DXCreateBuffer(uncompressed_size,&file_buffer);
	unsigned char *uncompressed_file;
	uncompressed_file = new unsigned char[uncompressed_size];

	uncompress(uncompressed_file,&uncompressed_size,fptemp,zlib_size);

	unsigned char *ucf_pnt = uncompressed_file;
	ucf_pnt += 4;
	unsigned char *shader;
	unsigned int shader_size;
	
	shader_size = (*((unsigned int*)ucf_pnt));
	ucf_pnt += 4;
	shader = new unsigned char[shader_size];
	memcpy(shader,ucf_pnt,shader_size);
	ucf_pnt += shader_size;
	d3d_device->CreateVertexShader((const unsigned long*)shader,&sprite_vertex_shader);
	delete[] shader;

	shader_size = (*((unsigned int*)ucf_pnt));
	ucf_pnt += 4;
	shader = new unsigned char[shader_size];
	memcpy(shader,ucf_pnt,shader_size);
	ucf_pnt += shader_size;
	d3d_device->CreateVertexShader((const unsigned long*)shader,&planet_vertex_shader);
	delete[] shader;

	shader_size = (*((unsigned int*)ucf_pnt));
	ucf_pnt += 4;
	shader = new unsigned char[shader_size];
	memcpy(shader,ucf_pnt,shader_size);
	ucf_pnt += shader_size;
	d3d_device->CreateVertexShader((const unsigned long*)shader,&star_vertex_shader);
	delete[] shader;

	shader_size = (*((unsigned int*)ucf_pnt));
	ucf_pnt += 4;
	shader = new unsigned char[shader_size];
	memcpy(shader,ucf_pnt,shader_size);
	ucf_pnt += shader_size;
	d3d_device->CreatePixelShader((const unsigned long*)shader,&star_pixel_shader);
	delete[] shader;

	shader_size = (*((unsigned int*)ucf_pnt));
	ucf_pnt += 4;
	shader = new unsigned char[shader_size];
	memcpy(shader,ucf_pnt,shader_size);
	ucf_pnt += shader_size;
	d3d_device->CreateVertexShader((const unsigned long*)shader,&planet_vertex_shader_med);
	delete[] shader;
	
	shader_size = (*((unsigned int*)ucf_pnt));
	ucf_pnt += 4;
	shader = new unsigned char[shader_size];
	memcpy(shader,ucf_pnt,shader_size);
	ucf_pnt += shader_size;
	d3d_device->CreatePixelShader((const unsigned long*)shader,&planet_pixel_shader_med);
	delete[] shader;
	
	shader_size = (*((unsigned int*)ucf_pnt));
	ucf_pnt += 4;
	shader = new unsigned char[shader_size];
	memcpy(shader,ucf_pnt,shader_size);
	ucf_pnt += shader_size;
	d3d_device->CreateVertexShader((const unsigned long*)shader,&star_vertex_shader_high);
	delete[] shader;

	shader_size = (*((unsigned int*)ucf_pnt));
	ucf_pnt += 4;
	shader = new unsigned char[shader_size];
	memcpy(shader,ucf_pnt,shader_size);
	ucf_pnt += shader_size;
	d3d_device->CreatePixelShader((const unsigned long*)shader,&star_pixel_shader_high);
	delete[] shader;
	
	shader_size = (*((unsigned int*)ucf_pnt));
	ucf_pnt += 4;
	shader = new unsigned char[shader_size];
	memcpy(shader,ucf_pnt,shader_size);
	ucf_pnt += shader_size;
	d3d_device->CreateVertexShader((const unsigned long*)shader,&planet_vertex_shader_high);
	delete[] shader;
	
	shader_size = (*((unsigned int*)ucf_pnt));
	ucf_pnt += 4;
	shader = new unsigned char[shader_size];
	memcpy(shader,ucf_pnt,shader_size);
	ucf_pnt += shader_size;
	d3d_device->CreatePixelShader((const unsigned long*)shader,&planet_pixel_shader_high);
	delete[] shader;
	
	shader_size = (*((unsigned int*)ucf_pnt));
	ucf_pnt += 4;
	shader = new unsigned char[shader_size];
	memcpy(shader,ucf_pnt,shader_size);
	ucf_pnt += shader_size;
	d3d_device->CreateVertexShader((const unsigned long*)shader,&planet_vertex_shader_earth);
	delete[] shader;
	
	shader_size = (*((unsigned int*)ucf_pnt));
	ucf_pnt += 4;
	shader = new unsigned char[shader_size];
	memcpy(shader,ucf_pnt,shader_size);
	ucf_pnt += shader_size;
	d3d_device->CreatePixelShader((const unsigned long*)shader,&planet_pixel_shader_earth);
	delete[] shader;


	shader_size = (*((unsigned int*)ucf_pnt));
	ucf_pnt += 4;
	shader = new unsigned char[shader_size];
	memcpy(shader,ucf_pnt,shader_size);
	ucf_pnt += shader_size;
	d3d_device->CreateVertexShader((const unsigned long*)shader,&planet_vertex_shader_gas);
	delete[] shader;
	
	shader_size = (*((unsigned int*)ucf_pnt));
	ucf_pnt += 4;
	shader = new unsigned char[shader_size];
	memcpy(shader,ucf_pnt,shader_size);
	ucf_pnt += shader_size;
	d3d_device->CreatePixelShader((const unsigned long*)shader,&planet_pixel_shader_gas);
	delete[] shader;

	shader_size = (*((unsigned int*)ucf_pnt));
	ucf_pnt += 4;
	shader = new unsigned char[shader_size];
	memcpy(shader,ucf_pnt,shader_size);
	ucf_pnt += shader_size;
	d3d_device->CreateVertexShader((const unsigned long*)shader,&planet_vertex_shader_sky);
	delete[] shader;
	
	shader_size = (*((unsigned int*)ucf_pnt));
	ucf_pnt += 4;
	shader = new unsigned char[shader_size];
	memcpy(shader,ucf_pnt,shader_size);
	ucf_pnt += shader_size;
	d3d_device->CreatePixelShader((const unsigned long*)shader,&planet_pixel_shader_sky);
	delete[] shader;

	
	
	
	UnmapViewOfFile(filepointer);
	CloseHandle(filemapping);
	CloseHandle(filehandle);
	delete[] uncompressed_file;

}

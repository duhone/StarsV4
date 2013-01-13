// HPT3DObjectInternal.cpp: implementation of the HPT3DObjectInternal class.
//
//////////////////////////////////////////////////////////////////////

#include "HPT3DObjectInternal.h"
#include "HPTGraphicsEngineInternal.h"

extern HPTGraphicsEngineInternal *gengine;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HPT3DObjectInternal::HPT3DObjectInternal()
{
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&rot_worldy);
	D3DXMatrixIdentity(&rot_worldz);
	int temp = gengine->GetQuality();
	if(temp == 0) quality_distance = 800.0f*800.0f*800.0f;
	if(temp == 1) quality_distance = 2000.0f*2000.0f*2000.0f;
	if(temp == 2) quality_distance = 2500.0f*2500.0f*2500.0f;
	if(temp == 0) quality_distance_low = 2000.0f*2000.0f*2000.0f;
	if(temp == 1) quality_distance_low = 4000.0f*4000.0f*4000.0f;
	if(temp == 2) quality_distance_low = 6000.0f*6000.0f*6000.0f;

}

HPT3DObjectInternal::~HPT3DObjectInternal()
{

}

void HPT3DObjectInternal::SetImage(int fart,int closet)
{
	far_texture = fart;
	close_texture = closet;

}

void HPT3DObjectInternal::Render()
{
	float dist;
	dist = 	world(3,0)*world(3,0) + world(3,1)*world(3,1) + world(3,2)*world(3,2);
	
	if(dist >= quality_distance_low)
		gengine->RenderSprite(world,far_texture,color);
	else if(dist >= quality_distance)
	{
		gengine->GetD3DDevice()->SetRenderState( D3DRS_ZENABLE, FALSE );
		gengine->GetD3DDevice()->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
		gengine->RenderSprite(world,far_texture,color);
		gengine->GetD3DDevice()->SetRenderState( D3DRS_ZENABLE, TRUE );
		gengine->GetD3DDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);

		D3DXMatrixMultiply(&full,&rot_worldy,&rot_worldz);
		D3DXMatrixMultiply(&full,&full,&world);
		gengine->RenderSphereL(full,close_texture,24,color,noise_position);
	}
	else
	{
		gengine->GetD3DDevice()->SetRenderState( D3DRS_ZENABLE, FALSE );
		gengine->GetD3DDevice()->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
		gengine->RenderSprite(world,far_texture,color);
		gengine->GetD3DDevice()->SetRenderState( D3DRS_ZENABLE, TRUE );
		gengine->GetD3DDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);

		D3DXMatrixMultiply(&full,&rot_worldy,&rot_worldz);
		D3DXMatrixMultiply(&full,&full,&world);
		gengine->RenderSphere(full,close_texture,24,color,noise_position);
	}

}

void HPT3DObjectInternal::SetPositionAbsalute(int x,int y,int z)
{

}

void HPT3DObjectInternal::SetPositionRelative(int x,int y,int z)
{

}

void HPT3DObjectInternal::Release()
{
	delete this;
}

void HPT3DObjectInternal::Scale(float scale_factor)
{
	world(0,0) = scale_factor;
	world(1,1) = scale_factor;
	world(2,2) = scale_factor;

}

void HPT3DObjectInternal::Position(float x,float y,float z)
{
	world(3,0) = x;
	world(3,1) = y;
	world(3,2) = z;
}

void HPT3DObjectInternal::Rotate(float y,float z)
{
	D3DXMatrixRotationY(&rot_worldy,y);
	D3DXMatrixRotationY(&rot_worldz,z);

}

void HPT3DObjectInternal::SetColor(float r,float g,float b,float a)
{
	color.x = r;
	color.y = g;
	color.z = b;
	color.w = a;
}


void HPT3DObjectInternal::SetNoisePosition(float arg)
{
	noise_position = arg;
}

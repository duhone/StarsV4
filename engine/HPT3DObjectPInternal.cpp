// HPT3DObjectPInternal.cpp: implementation of the HPT3DObjectPInternal class.
//
//////////////////////////////////////////////////////////////////////

#include "HPT3DObjectPInternal.h"
#include "HPTGraphicsEngineInternal.h"

extern HPTGraphicsEngineInternal *gengine;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HPT3DObjectPInternal::HPT3DObjectPInternal()
{
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&rot_worldy);
	D3DXMatrixIdentity(&rot_worldz);
	draw_ring = false;
	draw_sky = false;
	render_earth = false;
	render_gas = false;
}

HPT3DObjectPInternal::~HPT3DObjectPInternal()
{

}

void HPT3DObjectPInternal::SetImage(int ground,int sky,int ground_bump,int sky_bump,int night)
{
	ground_texture = ground;
	sky_texture = sky;
	ground_bump_texture = ground_bump;
	sky_bump_texture = sky_bump;
	night_texture = night;
}

void HPT3DObjectPInternal::Render()
{
	D3DXMATRIX tempry;
	D3DXMatrixRotationX(&tempry,-0.25f);

		D3DXMatrixMultiply(&rot_world,&rot_worldy,&tempry);
		D3DXMatrixMultiply(&rot_world,&rot_world,&rot_worldz);
		D3DXMatrixMultiply(&full,&rot_world,&world);
		D3DXMatrixInverse(&rot_world,NULL,&rot_world);
		D3DXVec3TransformNormal(&light_vec_trans,&light_vec,&rot_world);
		if(render_earth)
			gengine->RenderSphereE(full,ground_texture,ground_bump_texture,light_vec_trans,0/*sky_pos*/,night_texture,diffuse_amount,specular_amount);
		else
		{
			if(render_gas)
				gengine->RenderSphereGas(full,ground_texture,ground_bump_texture,light_vec_trans,0/*sky_pos*/,night_texture,diffuse_amount,specular_amount);
			else
				gengine->RenderSphereP(full,ground_texture,ground_bump_texture,light_vec_trans,0/*sky_pos*/,night_texture,diffuse_amount,specular_amount);
		}
	if(draw_sky)
	{
		gengine->GetD3DDevice()->SetRenderState( D3DRS_ZENABLE, FALSE );
		gengine->GetD3DDevice()->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);

		D3DXMATRIX temps = world;
		temps(0,0) = world(0,0)*1.05f;
		temps(1,1) = world(1,1)*1.05f;
		temps(2,2) = world(2,2)*1.05f;
		D3DXMatrixRotationX(&tempry,-0.25f);

		D3DXMatrixMultiply(&rot_world,&rot_worldy,&tempry);
		D3DXMatrixMultiply(&rot_world,&rot_world,&rot_worldz);
		D3DXMatrixMultiply(&full,&rot_world,&temps);
		D3DXMatrixInverse(&rot_world,NULL,&rot_world);
		D3DXVec3TransformNormal(&light_vec_trans,&light_vec,&rot_world);
		gengine->RenderSphereSky(full,sky_texture,sky_bump_texture,light_vec_trans,sky_pos,sky_texture,diffuse_amount*0.8f,specular_amount*1.4f);
		gengine->GetD3DDevice()->SetRenderState( D3DRS_ZENABLE, TRUE );
		gengine->GetD3DDevice()->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	}
		if(draw_ring)
			gengine->RenderRing(full,ring_tex,light_vec_trans);

}

void HPT3DObjectPInternal::Release()
{
	delete this;
}

void HPT3DObjectPInternal::Scale(float scale_factor)
{
	world(0,0) = scale_factor;
	world(1,1) = scale_factor;
	world(2,2) = scale_factor;

}

void HPT3DObjectPInternal::Position(float x,float y,float z)
{
	world(3,0) = x;
	world(3,1) = y;
	world(3,2) = z;
}

void HPT3DObjectPInternal::SkyPosition(float x)
{
	sky_pos = x;
}

void HPT3DObjectPInternal::Rotate(float y,float z)
{
	D3DXMatrixRotationY(&rot_worldy,y);
	D3DXMatrixRotationZ(&rot_worldz,z);

}

void HPT3DObjectPInternal::SetColor(float r,float g,float b,float a)
{
	color.x = r;
	color.y = g;
	color.z = b;
	color.w = 1.0f;
}

void HPT3DObjectPInternal::SetLightVec(float x,float y,float z)
{
	light_vec.x = x;
	light_vec.y = y;
	light_vec.z = z;
	D3DXVec3Normalize(&light_vec,&light_vec);
}

void HPT3DObjectPInternal::AddRing(int tex)
{
	draw_ring = true;
	ring_tex = tex;
}

void HPT3DObjectPInternal::DisableRing()
{
	draw_ring = false;
}

void HPT3DObjectPInternal::SetMaterial(float diffuse,float specular)
{
	diffuse_amount = diffuse;
	specular_amount = specular;


}

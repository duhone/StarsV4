// HPT3DObjectPInternal.h: interface for the HPT3DObjectPInternal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HPT3DOBJECTPINTERNAL_H__FD1E28E2_96C7_49EB_BE3E_943B2439DF34__INCLUDED_)
#define AFX_HPT3DOBJECTPINTERNAL_H__FD1E28E2_96C7_49EB_BE3E_943B2439DF34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HPT Graphics.h"
#include <d3dx9.h>

class HPT3DObjectPInternal : public HPT3DObjectP  
{
public:
	HPT3DObjectPInternal();
	virtual ~HPT3DObjectPInternal();
	virtual void SetImage(int ground,int sky,int ground_bump,int sky_bump,int night);
	virtual void Render();
	virtual void Release();
	virtual void Scale(float scale_factor);
	virtual void Position(float x,float y,float z);
	virtual void SkyPosition(float x);
	virtual void Rotate(float y,float z);
	virtual void SetColor(float r,float g,float b,float a);
	virtual void SetLightVec(float x,float y,float z);
	virtual void AddRing(int tex);
	virtual void DisableRing();
	virtual void EnableSky() {draw_sky = true;};
	virtual void DisableSky() {draw_sky = false; render_earth = false;};
	virtual void SetEarth() {render_earth = true;};
	virtual void SetGas() {render_gas = true;};
	virtual void SetMaterial(float diffuse,float specular);
	virtual void DisableGas() {render_gas = false;};

private:
	float diffuse_amount,specular_amount;
	bool render_earth;
	bool render_gas;
	//bool render_earth;
	int night_texture;
	int sky_bump_texture;
	int ground_bump_texture;
	bool draw_sky;
	D3DXMATRIX world;
	D3DXMATRIX rot_worldz;
	D3DXMATRIX rot_worldy;
	D3DXMATRIX rot_world;
	D3DXMATRIX full;
	D3DXVECTOR4 color;
	D3DXVECTOR3 light_vec;
	D3DXVECTOR3 light_vec_trans;
	int ground_texture;
	int sky_texture;
	int ring_tex;
	bool draw_ring;
	float sky_pos;
};

#endif // !defined(AFX_HPT3DOBJECTPINTERNAL_H__FD1E28E2_96C7_49EB_BE3E_943B2439DF34__INCLUDED_)

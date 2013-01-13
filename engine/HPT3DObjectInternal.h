// HPT3DObjectInternal.h: interface for the HPT3DObjectInternal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HPT3DOBJECTINTERNAL_H__A305D4D5_E232_425E_A570_32168CB9AAAC__INCLUDED_)
#define AFX_HPT3DOBJECTINTERNAL_H__A305D4D5_E232_425E_A570_32168CB9AAAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HPT Graphics.h"
#include <d3dx9.h>

//#define QUALITY_DISTANCE 20000000000.0f

class HPT3DObjectInternal : public HPT3DObject  
{
public:
	HPT3DObjectInternal();
	virtual ~HPT3DObjectInternal();
	virtual void SetImage(int fart,int closet);
	virtual void Render();
	virtual void SetPositionAbsalute(int x,int y,int z);
	virtual void SetPositionRelative(int x,int y,int z);
	virtual void Release();
	virtual void Scale(float scale_factor);
	virtual void Position(float x,float y,float z);
	virtual void SetColor(float r,float g,float b,float a);
	virtual void Rotate(float y,float z);
	virtual void SetNoisePosition(float arg);

private:
	float quality_distance;
	float quality_distance_low;
	int noise_texture;
	float noise_position;
	D3DXMATRIX world;
	D3DXMATRIX rot_worldz;
	D3DXMATRIX rot_worldy;
	D3DXMATRIX full;
	D3DXVECTOR4 color;
	int far_texture;
	int close_texture;
};

#endif // !defined(AFX_HPT3DOBJECTINTERNAL_H__A305D4D5_E232_425E_A570_32168CB9AAAC__INCLUDED_)

// Planet.h: interface for the Planet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLANET_H__1FFED9AE_C5C3_4CB7_8C01_83D348478435__INCLUDED_)
#define AFX_PLANET_H__1FFED9AE_C5C3_4CB7_8C01_83D348478435__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "hpt graphics.h"
#include<math.h>
#include <stdlib.h>

class Planet  
{
public:
private:
	float noise_position;
	float minsize;
	float x,y,z;
//	float xs,ys;
	HPT3DObjectP* sprite;
	HPT3DObject* star_sprite;
//	TRANSFORM *trans;
//	float rs,gs,bs;
//	float r,g,b;
	int type;
	float size;
//	float temps,tempz;
	bool done;
	float sky_velocity;
	float sky_position;
	bool demo_planet;
public:
	void createnewstar();

//	void Move();
	Planet();
	virtual ~Planet();
	inline void MoveSky(float time)
	{
		noise_position += time*0.01f;
		star_sprite->SetNoisePosition(noise_position);
		sky_position += sky_velocity * time;
		sprite->SkyPosition(sky_position);

	};
	inline void movestar(float time)
	{
		z -= time;
		if((z < -200-5*size)) done = true;;
		//if(((fabs(x)-size*30) > z*0.602284749f) || ((fabs(y)-size*30) > z*0.464213562f)) done = true;;
		if(type != 0) sprite->Position(x,y,z);
		else star_sprite->Position(x,y,z);
	};

	inline void drawstar()
	{
		if(type != 0) sprite->Render();
		else star_sprite->Render();
	};
	inline float GetZ() {return z;};
	inline bool IsDone() {return done;};
};

#endif // !defined(AFX_PLANET_H__1FFED9AE_C5C3_4CB7_8C01_83D348478435__INCLUDED_)

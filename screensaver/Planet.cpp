// Planet.cpp: implementation of the Planet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Planet.h"
extern HPTGraphicsEngine *gengine;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define red 3
#define yellow 1
#define blue 2
#define white 0

Planet::Planet()
{
	sprite = gengine->Create3DObjectP();
	star_sprite = gengine->Create3DObject();
	createnewstar();
	sprite->Scale(size);
	sprite->Position(x,y,z);
	sprite->SetImage(2,3,2,3,2);
//	sprite->SetColor(r,g,b,1);
	sprite->SkyPosition(0.1f);
	sprite->SetColor(0.01f,0.01f,0.01f,0.0f);
	sprite->SetLightVec(-1,0,-0.1f);
	sprite->Rotate(2.4f,0.0f);

	star_sprite->Scale(size);
	star_sprite->Position(x,y,z);
	star_sprite->SetImage(0,1);
//	sprite->SetColor(r,g,b,1);
	star_sprite->Rotate(2.4f,0.0f);
	demo_planet = false;

}

Planet::~Planet()
{
	sprite->Release();
	star_sprite->Release();
}

void Planet::createnewstar()
{
	noise_position = 0;
	sprite->DisableSky();
	sprite->DisableRing();
	sprite->DisableGas();
	//size = (float)((rand() % 4500) + 5001);
	size = 7500;
	size *= 0.05f;

	type = rand() % 11;
	//type = 7;
	int ctype;
/*#if defined DEMO
	if(demo_planet) type = 11;
#endif*/
	switch(type)
	{
	case 0:	//star
		//sprite->SetImage(1,5);
		size = (float)((rand() % 4500) + 5001);
		size *= 0.05f;
		size *= 5;
		ctype = rand() % 7;
		if(ctype == 0 || ctype == 1 || ctype == 2) ctype = yellow;
		else if(ctype == 3 || ctype == 4) ctype = white;
		else if(ctype == 5) ctype = blue;
		else if(ctype == 6) ctype = red;
		float color1,color2;
		//ctype = yellow;
		color1 = 0.6f;
		color2 = color1 + (rand() % 3000) / (float)10000;
		//color2 = 0.6f;
		float r,g,b;
		switch(ctype)
		{
		case red:
			r = 1.0f;
			g = color2+0.05f;
			b = color2+0.05f;
			break;
		case yellow:
			r = 1.0f;
			g = 0.96f;
			b = color2-0.1f;
			break;
		case blue:
			r = color2+0.1f;
			g = color2+0.1f;
			b = 1.0f;
			break;
		case white:
			r = 1.0f;
			g = 1.0f;
			b = 1.0f;
			break;
		};
		star_sprite->SetColor(r,g,b,1);

		break;
	case 1: //mecury
		sprite->SetImage(4,2,5,3,2);
		sprite->SetMaterial(0.95f,0.1f);
		size *= 0.8f;
		break;
	case 2: //venus
		sprite->SetImage(6,2,5,3,2);
		sprite->SetMaterial(0.85f,0.25f);
		size *= 1;
		break;
	case 3: //earth
		sprite->SetImage(8,10,9,11,25);
		sprite->SetMaterial(0.8f,0.35f);
		sprite->EnableSky();
		sprite->SetEarth();
		break;
	case 4: //moon
		sprite->SetImage(12,2,13,3,2);
		sprite->SetMaterial(0.95f,0.1f);
		size *= 0.4f;
		break;
	case 5: //mars
		sprite->SetImage(14,2,15,3,2);
		sprite->SetMaterial(0.8f,0.35f);
		size *= 0.7f;
		break;
	case 6: //jupitor
		sprite->SetImage(16,2,3,3,2);
		sprite->SetMaterial(0.8f,0.3f);
		sprite->SetGas();
		size *= 7;
		break;
	case 7: //saturn
		sprite->SetImage(17,2,3,3,2);
		sprite->SetMaterial(0.8f,0.3f);
		sprite->AddRing(18);
		sprite->SetGas();
		size *= 6;
		break;
	case 8: //uranus
		sprite->SetImage(19,2,3,3,2);
		sprite->SetMaterial(0.8f,0.3f);
		sprite->AddRing(20);
		sprite->SetGas();
		size *= 3;
		break;
	case 9: //neptune
		sprite->SetImage(21,2,3,3,2);
		sprite->SetMaterial(0.8f,0.3f);
		sprite->SetGas();
		size *= 2.5f;
		break;
	case 10: //pluto
		sprite->SetImage(22,2,23,3,2);
		sprite->SetMaterial(0.95f,0.1f);
		size *= 0.35f;
		break;
	case 11: //demo planet
		sprite->SetImage(26,2,27,3,2);
		sprite->SetMaterial(0.75f,0.5f);
		size *= 2;
		break;


	};
	done = false;
	x = (float)((rand() % 2000) - 1000);
	y = (float)((rand() % 2000) - 1000);

	z = 1000000;

/*	if(((fabs(x)-size) <= 5) && ((fabs(y)-size) <= 5))
	{
		if(x < 0) x = -size-4;
		else x = size+4;
		if(y < 0) y = -size-4;
		else y = size+4;
	}*/
	float magnitude = sqrt(x*x +y*y);
	x = x/magnitude;
	y = y/magnitude;
	x = x*size*1.06f;
	y = y*size*1.06f;
	sprite->Scale(size);
	star_sprite->Scale(size);

/*	type = rand() % 7;
	if(type == 0 || type == 1 || type == 2) type = yellow;
	if(type == 3 || type == 4) type = white;
	if(type == 5) type = blue;
	if(type == 6) type = red;
	float color1,color2;
	color1 = 0.6f;
	color2 = color1 + (rand() % 3900) / (float)10000;
*/	size = (float)((rand() % 4500) + 5001);
	size *= 0.05f;
	//size = 9501*0.05f;

	if((type != 8) && (type != 11))
		sprite->Rotate((rand() % 6280) / (float)1000,((rand() % 500) / (float)1000)-0.25f);
	else if(type == 8)
		sprite->Rotate((rand() % 6280) / (float)1000,((rand() % 1000) / (float)1000)+1.00f);
	else if(type == 11)
		sprite->Rotate(3.14f/2,0);

	float angle1 = (rand() % 4000) / 1000.0f;
	float angle2 = (rand() % 3140) / 1000.0f;
	if(type != 11) sprite->SetLightVec(/*-1.0f,-0.0f,-0.2f*/(float)cos(angle2),((rand()%400) - 200)/1000.0f,(float)-sin(angle1)-0.1f);
		else sprite->SetLightVec(0.0f,0.0f,-2.0f);
	sky_position = 0;
	sky_velocity = ((rand() % 70) + 20)/10000.0f;

	star_sprite->Rotate((rand() % 6280) / (float)1000,(rand() % 6280) / (float)1000);
	demo_planet = true;

}


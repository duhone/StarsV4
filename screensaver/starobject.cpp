
#include "stdafx.h"
#include "starobject.h"
#include <stdlib.h>
#include <math.h>
//#include "display.h"

#define red 3
#define yellow 1
#define blue 2
#define white 0


extern HPTGraphicsEngine *gengine;


/*extern TEXTURE_OBJECT *starpic;	
extern DISPLAY_ENGINE *dengine;
extern vector<TEXTURE_OBJECT*> planet_textures;*/


Star::Star()
{
/*	sprite = dengine->CreateSpriteObject();
	trans = dengine->CreateTransform();
	sprite->AttachTextureObject(starpic);
	sprite->AttachTransform(trans);*/
	size_mult = 1;
	sprite = gengine->Create3DObject();
	createnewstar(true);
	sprite->Scale(size);
	sprite->Position(x,y,z);
	sprite->SetImage(0,1);
	sprite->SetColor(r,g,b,1);
//	sprite->Rotate(1.5f,0.5f);

}

Star::~Star()
{
	sprite->Release();

}

void Star::createnewstar(bool first)
{

	if(first) 
	{
		x = (float)((rand() % 7000)*200 - 700000);
		y = (float)((rand() % 7000)*200 - 700000);

		z = (float)((rand() % 1000)*1000 + 1);
		/*x = 0;
		y = 0;
		z = 1000000;*/
	}
	else
	{
		x = (float)((rand() % 7000)*200 - 700000);
		y = (float)((rand() % 7000)*200 - 700000);

		z = 1000000;

	}
	if((fabs(x) <= 4000) && (fabs(y) <= 4000))
	{
		if(x < 0) x = -4000;
		else x = 4000;
		if(y < 0) y = -4000;
		else y = 4000;
	}
	/*else if(fabs(y) <= 4000) 
	{
		if(y > -4000) y = -4000;
		else y = 4000;
	}*/

	type = rand() % 7;
	if(type == 0 || type == 1 || type == 2) type = yellow;
	if(type == 3 || type == 4) type = white;
	if(type == 5) type = blue;
	if(type == 6) type = red;
	float color1,color2;
	color1 = 0.6f;
	color2 = color1 + (rand() % 3900) / (float)10000;
	size = (float)((rand() % 7500) + 2001);
	size *= 0.08f * size_mult;
	switch(type)
	{
	case red:
/*		r = color2;
		g = color1;
		b = color1;
*/		r = 1.0f;
		g = color2;
		b = color2;
		break;
	case yellow:
/*		r = color2;
		g = color2*0.98f;
		b = color1;
*/		r = 1.0f;
		g = 0.98f;
		b = color2;
		break;
	case blue:
/*		r = color1;
		g = color1;
		b = color2;
*/		r = color2;
		g = color2;
		b = 1.0f;
		break;
	case white:
/*		r = color2;
		g = color2;
		b = color2;*/
		r = 1.0f;
		g = 1.0f;
		b = 1.0f;
		break;
	}

	sprite->Rotate((rand() % 6280) / (float)1000,(rand() % 6280) / (float)1000);
	sprite->Scale(size);
	noise_position = 0;


}


/*void Star::setres(int w,int h)
{
	minsize = 350.0f/(h*70);
}
*/


/*void Star::Move()
{
	if(z != 0.0f) tempz = 1 / z;
	else tempz = 999999999.0f;
	xs = (x *tempz);
	ys = (y *tempz);
	temps = size*tempz;
	if(temps < minsize) temps = minsize;
	tempz = 200000.0f*tempz + 0.2f;

	if(tempz > 1.1f) tempz = 1.1f;
	rs = r * tempz;
	gs = g * tempz;
	bs = b * tempz;

	if(!(ys > -2 && ys <= 2 && xs > -3 && xs <= 3 && z >= 1))
	{
		createnewstar(false);
		project();
	}
	trans->Translate(xs,ys,z*0.0001);
	trans->Scale(temps,temps);
	sprite->SetColor(rs,gs,bs,1);

}
*/
#include "hpt graphics.h"
#include<math.h>
#include <stdlib.h>

extern int quality;
extern bool widescreen;


class Star
{
private:
	float noise_position;
	float minsize;
	float x,y,z;
//	float xs,ys;
	HPT3DObject* sprite;
//	TRANSFORM *trans;
//	float rs,gs,bs;
	float r,g,b;
	int type;
	float size;
	float size_mult;
//	float temps,tempz;
protected:
	void createnewstar(bool first);
public:
//	void Move();
	Star();
	~Star();
	inline bool movestar(float time)
	{
		float tempmax;
		if(widescreen) tempmax = 0.78f;
		else tempmax = 0.57735027f;
		if(quality == 2)
		{
			noise_position += time*10.1f;
			sprite->SetNoisePosition(noise_position);
		}
		z -= time;
		if((z < 1))
		{
			createnewstar(false);
			sprite->Position(x,y,z);
			return true;
		}
		if(((fabs(x) - 2*size) > z*tempmax/*0.78f/*0.57735027f*/) || ((fabs(y) - 2*size) > z*0.414213562f))
		{
			createnewstar(false);
			sprite->Position(x,y,z);
			return true;
		}
		sprite->Position(x,y,z);
		return false;
	};
	inline bool movestarsetup(float time)
	{
		float tempmax;
		if(widescreen) tempmax = 0.78f;
		else tempmax = 0.57735027f;
		//noise_position += time*10.1f;
		//sprite->SetNoisePosition(noise_position);
		z -= time;
		if((z < 1))
		{
			createnewstar(false);
			//sprite->Position(x,y,z);
			return true;
		}
		if(((fabs(x)-size*2) > z*tempmax/*0.78f/*0.57735027f*/) || ((fabs(y)-size*2) > z*0.414213562f))
		{
			createnewstar(false);
			//sprite->Position(x,y,z);
			return true;
		}
		//sprite->Position(x,y,z);
		return false;
	};
	inline void SetSizeMult(float arg)
	{
		size_mult = arg;
	}
	inline bool movestarturn(float ztime,float xtime,float ytime)
	{
		float tempmax;
		if(widescreen) tempmax = 0.78f;
		else tempmax = 0.57735027f;
		float tempmax2;
		if(widescreen) tempmax2 = 0.79f;
		else tempmax2 = 0.58735027f;
		z -= ztime;
		x += xtime*z*0.000005f;
		y += ytime*z*0.000005f;
		if((z < 1))
		{
			createnewstar(false);
			sprite->Position(x,y,z);
			return true;
		}
		//if((z < 1)) createnewstar(false);
		//if((fabs(x) > z*0.602284749f) || (fabs(y) > z*0.464213562f)) createnewstar(false);
		if(xtime > 0)
		{
			if(((x-size*2) > z*tempmax2/*0.79f/*0.58735027f*/))
			{
				if((rand() %2 ) == 0)
				{
					x = -z*tempmax/*0.78f/*0.57735027f*/-size*2;
					sprite->Position(x,y,z);
					return false;
				}
				else
				{
					createnewstar(false);
					sprite->Position(x,y,z);
					return true;
				}
			}
			if(((x+size*2) < -z*tempmax/*0.78f/*0.57735027f*/))
			{
				createnewstar(false);
				sprite->Position(x,y,z);
				return true;
			}
			/*if((fabs(y) > z*0.414213562f))
			{
				createnewstar(false);
				sprite->Position(x,y,z);
				return true;
			}*/
		}
		if(xtime < 0)
		{
			if(((x+size*2) < -z*tempmax2/*0.79f/*0.58735027f*/))
			{
				if((rand() %2 ) == 0)
				{
					x = z*tempmax/*0.78f/*0.57735027f*/+size*2;
					sprite->Position(x,y,z);
					return false;
				}
				else
				{
					createnewstar(false);
					sprite->Position(x,y,z);
					return true;
				}
			}
			if(((x-size*2) > z*tempmax/*0.78f/*0.57735027f*/))
			{
				createnewstar(false);
				sprite->Position(x,y,z);
				return true;
			}
			/*if((fabs(y) > z*0.414213562f))
			{
				createnewstar(false);
				sprite->Position(x,y,z);
				return true;
			}*/
		}	
		if(ytime > 0)
		{
			if(((y-size*2) > z*0.424213562f))
			{
				if((rand() %2 ) == 0)
				{
					y = -z*0.414213562f-size*2;
					sprite->Position(x,y,z);
					return false;
				}
				else
				{
					createnewstar(false);
					sprite->Position(x,y,z);
					return true;
				}
			}
			if(((y+size*2) < -z*0.414213562f))
			{
				createnewstar(false);
				sprite->Position(x,y,z);
				return true;
			}
			/* if((fabs(x) > z*0.552284749f))
			{
				createnewstar(false);
				sprite->Position(x,y,z);
				return true;
			}*/
		}
		if(ytime < 0)
		{
			if(((y+size*2) < -z*0.424213562f))
			{
				if((rand() %2 ) == 0)
				{
					y = z*0.414213562f+size*2;
					sprite->Position(x,y,z);
					return false;
				}
				else
				{
					createnewstar(false);
					sprite->Position(x,y,z);
					return true;
				}
			}
			if(((y-size*2) > z*0.414213562f))
			{
				createnewstar(false);
				sprite->Position(x,y,z);
				return true;
			}
			/* if((fabs(x) > z*0.552284749f))
			{
				createnewstar(false);
				sprite->Position(x,y,z);
				return true;
			}*/
		}
		sprite->Position(x,y,z);
		return false;
	};
	inline void drawstar() {sprite->Render();};
//	void setres(int w,int h);
};



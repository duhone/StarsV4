#if !defined(AFX_HPTTEXTURESTRUCT_H__84A8343D_A0F8_4CA3_B61B_6A3A8EAF4BD4__INCLUDED_)
#define AFX_HPTTEXTURESTRUCT_H__84A8343D_A0F8_4CA3_B61B_6A3A8EAF4BD4__INCLUDED_

#include<windows.h>
#include<d3d9.h>

struct HPTTextureStruct
{
	int width;
	int height;
	//int data_size;
	int num_frame_sets;
	int *frames_per_set;
	IDirect3DTexture9* texture;
	HPTTextureStruct()
	{
		frames_per_set = NULL;
	}
	~HPTTextureStruct()
	{
		if(frames_per_set != NULL) delete frames_per_set;
		texture->Release();
	}
public:
	int ReadPNG(unsigned char* stream);
private:
	void ReadNormalPNGLine(unsigned char* dest,unsigned char* src,int width);
	void ReadSubPNGLine(unsigned char* dest,unsigned char* src,int width);
	void ReadUpPNGLine(unsigned char* dest,unsigned char* src,int width,int pitch);
	void ReadAvgPNGLineFirst(unsigned char* dest,unsigned char* src,int width);
	void ReadAvgPNGLine(unsigned char* dest,unsigned char* src,int width,int pitch);
	//unsigned char PaethPredictor(unsigned char a,unsigned char b,unsigned char c);
	_inline unsigned char HPTTextureStruct::PaethPredictor(unsigned char a,unsigned char b,unsigned char c)
	{
        // a = left, b = above, c = upper left
        unsigned int p = a + b - c;        // initial estimate
        unsigned int pa = abs((long)(p - a));      // distances to a, b, c
        unsigned int pb = abs((long)(p - b));
        unsigned int pc = abs((long)(p - c));
       // return nearest of a,b,c,
        // breaking ties in order a,b,c.
        if((pa <= pb) && (pa <= pc)) return a;
        else if(pb <= pc) return b;
        else return c;
	};
	void ReadPaethPNGLine(unsigned char* dest,unsigned char* src,int width,int pitch,int ycount);

};

#endif

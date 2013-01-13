#include"HPTTextureStruct.h"
#include "zlib.h"
#include"HPTGraphicsEngineInternal.h"

extern HPTGraphicsEngineInternal *gengine;

int HPTTextureStruct::ReadPNG(unsigned char* stream)
{

	int bytes_read = 0;
//	z_stream zlib_stream;
	memcpy(&bytes_read,stream,4);
	unsigned long size = width*height*4+height;
	unsigned char *tdata = new unsigned char[size];
	if(tdata == NULL) MessageBox(NULL,"Out of memory while creating sprites","ERROR",MB_OK);

	stream += 4;
	uncompress(tdata,&size,stream,bytes_read);
	if(gengine->GetTexture32bpp())
	{
		gengine->GetD3DDevice()->CreateTexture(width,height,0,0/*D3DUSAGE_AUTOGENMIPMAP*/,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED,&texture,NULL);
		D3DLOCKED_RECT templr;

		texture->LockRect(0,&templr,NULL,D3DLOCK_DISCARD);
		unsigned char* src = tdata;
		unsigned char* dest = (unsigned char*)templr.pBits;
		unsigned char line_type;
		for(int ycount = 0;ycount < height;ycount++)
		{
			line_type = (*src);
			src++;
			switch(line_type)
			{
			case 0:
				ReadNormalPNGLine(dest,src,width);
				break;
			case 1:
				ReadSubPNGLine(dest,src,width);
				break;
			case 2:
				if(ycount == 0) ReadNormalPNGLine(dest,src,width);
				else ReadUpPNGLine(dest,src,width,templr.Pitch);
				break;
			case 3:
				if(ycount == 0) ReadAvgPNGLineFirst(dest,src,width);
				else ReadAvgPNGLine(dest,src,width,templr.Pitch);
				break;
			case 4:
				ReadPaethPNGLine(dest,src,width,templr.Pitch,ycount);
				break;
			default:
				ReadNormalPNGLine(dest,src,width);
				break;
			}
			dest += 4*width;
			src += 4*width;

			dest += templr.Pitch - (width*4);
		}

		texture->UnlockRect(0);
	}
	else
	{
		gengine->GetD3DDevice()->CreateTexture(width,height,0,0/*D3DUSAGE_AUTOGENMIPMAP*/,D3DFMT_A4R4G4B4,D3DPOOL_MANAGED,&texture,NULL);
		D3DLOCKED_RECT templr;

//		texture->LockRect(0,&templr,NULL,D3DLOCK_DISCARD);
		unsigned char* src = tdata;
		unsigned char* tempdest = new unsigned char[width*height*4]/*(unsigned char*)templr.pBits*/;
		unsigned char* dest = tempdest;
		unsigned char line_type;
		for(int ycount = 0;ycount < height;ycount++)
		{
			line_type = (*src);
			src++;
			switch(line_type)
			{
			case 0:
				ReadNormalPNGLine(dest,src,width);
				break;
			case 1:
				ReadSubPNGLine(dest,src,width);
				break;
			case 2:
				if(ycount == 0) ReadNormalPNGLine(dest,src,width);
				else ReadUpPNGLine(dest,src,width,width*4);
				break;
			case 3:
				if(ycount == 0) ReadAvgPNGLineFirst(dest,src,width);
				else ReadAvgPNGLine(dest,src,width,width*4);
				break;
			case 4:
				ReadPaethPNGLine(dest,src,width,width*4,ycount);
				break;
			default:
				ReadNormalPNGLine(dest,src,width);
				break;
			}
			dest += 4*width;
			src += 4*width;

//			dest += templr.Pitch - (width*4);
		}
		texture->LockRect(0,&templr,NULL,D3DLOCK_DISCARD);
		src = tempdest;
		unsigned short* dests = (unsigned short*)templr.pBits;
		for(int ycount = 0;ycount < height;ycount++)
		{
			for(int xcount = 0;xcount < width;xcount++)
			{
				unsigned short temp;
				temp = 0;
				temp += (src[3]>>4);
				temp = temp << 4;
				temp += (src[2]>>4);
				temp = temp << 4;
				temp += (src[1]>>4);
				temp = temp << 4;
				temp += (src[0]>>4);

				dests[0] = temp;
				dests += 1;
				src += 4;
			}
			dests += (templr.Pitch - (width*2))>>1;

		}
		texture->UnlockRect(0);
		delete[] tempdest;
	}
	D3DXFilterTexture(texture,NULL,D3DX_DEFAULT,D3DX_DEFAULT);

	delete[] tdata;

	return bytes_read+4;
}

void HPTTextureStruct::ReadNormalPNGLine(unsigned char *dest, unsigned char *src,int width)
{
		for(int xcount = 0;xcount < width;xcount++)
		{
			(*dest) = (*(src+2));
			(*(dest+1)) = (*(src+1));
			(*(dest+2)) = (*(src));
			(*(dest+3)) = (*(src+3));
			dest += 4;
			src += 4;
		}

}

void HPTTextureStruct::ReadSubPNGLine(unsigned char* dest,unsigned char* src,int width)
{
		(*dest) = (*(src+2));
		(*(dest+1)) = (*(src+1));
		(*(dest+2)) = (*(src));
		(*(dest+3)) = (*(src+3));
		dest += 4;
		src += 4;
		for(int xcount = 1;xcount < width;xcount++)
		{
			(*dest) = (*(src+2)) + (*(dest-4));
			(*(dest+1)) = (*(src+1)) + (*(dest-3));
			(*(dest+2)) = (*(src)) + (*(dest-2));
			(*(dest+3)) = (*(src+3)) + (*(dest-1));
			dest += 4;
			src += 4;
		}

}

void HPTTextureStruct::ReadUpPNGLine(unsigned char* dest,unsigned char* src,int width,int pitch)
{
		for(int xcount = 0;xcount < width;xcount++)
		{
			(*dest) = (*(src+2)) + (*(dest-pitch));
			(*(dest+1)) = (*(src+1)) + (*(dest-pitch+1));
			(*(dest+2)) = (*(src)) + (*(dest-pitch+2));
			(*(dest+3)) = (*(src+3)) + (*(dest-pitch+3));
			dest += 4;
			src += 4;
		}

}

void HPTTextureStruct::ReadAvgPNGLineFirst(unsigned char* dest,unsigned char* src,int width)
{
		(*dest) = (*(src+2));
		(*(dest+1)) = (*(src+1));
		(*(dest+2)) = (*(src));
		(*(dest+3)) = (*(src+3));
		dest += 4;
		src += 4;
		for(int xcount = 1;xcount < width;xcount++)
		{
			(*dest) = (*(src+2)) + ((*(dest-4))>>1);
			(*(dest+1)) = (*(src+1)) + ((*(dest-3))>>1);
			(*(dest+2)) = (*(src)) + ((*(dest-2))>>1);
			(*(dest+3)) = (*(src+3)) + ((*(dest-1))>>1);
			dest += 4;
			src += 4;
		}

}

void HPTTextureStruct::ReadAvgPNGLine(unsigned char* dest,unsigned char* src,int width,int pitch)
{

	if(width == 0) return;

	dest[0] = src[2] + ((dest[-pitch])>>1);
	dest[1] = src[1] + ((dest[-pitch+1])>>1);
	dest[2] = src[0] + ((dest[-pitch+2])>>1);
	dest[3] = src[3] + ((dest[-pitch+3])>>1);
	if(width == 1) return;

	dest += 4;
	src += 4;
	unsigned int average;
/*	unsigned int mask = 0x0ffffffff;
	pitch = -pitch;
	_asm
	{
		mov ecx,width
		dec ecx
		mov edi,dest
		mov esi,src
		mov eax,pitch

		pxor mm5,mm5
		movd mm4,mask
		punpcklbw mm4,mm5
begin:
		pxor mm3,mm3
		movd mm0,[edi+eax]
		movd mm1,[edi-4]
		movd mm2,[esi]
		punpcklbw mm0,mm3
		punpcklbw mm1,mm3
		punpcklbw mm2,mm3
		paddw mm0,mm1
		psrlw mm0,1
		paddw mm0,mm2
		pand mm0,mm4
		packuswb mm0,mm3

//		paddb mm0,mm2
		movd [esi],mm0

		add esi,4
		add edi,4
		
		dec ecx
//		add ecx,1
//		cmp ecx,width
		jnz begin
		emms
	}

*/
/*	for(int xcount = 1;xcount < width;xcount++)
	{
		average = ((*(dest-pitch)) + (unsigned int)(*(dest-4)));
		(*dest) = (*(src+2)) + (average>>1);
		average = ((*(dest-pitch+1)) + (unsigned int)(*(dest-3)));
		(*(dest+1)) = (*(src+1)) + (average>>1);
		average = ((*(dest-pitch+2)) + (unsigned int)(*(dest-2)));
		(*(dest+2)) = (*(src)) + (average>>1);
		average = ((*(dest-pitch+3)) + (unsigned int)(*(dest-1)));
		(*(dest+3)) = (*(src+3)) + (average>>1);
		dest += 4;
		src += 4;
	}*/
	for(int xcount = 1;xcount < width;xcount++)
	{
		average = dest[-pitch] + (unsigned int)dest[-4];
		dest[0] = src[2] + (average>>1);
		average = dest[-pitch+1] + (unsigned int)dest[-3];
		dest[1] = src[1] + (average>>1);
		average = dest[-pitch+2] + (unsigned int)dest[-2];
		dest[2] = src[0] + (average>>1);
		average = dest[-pitch+3] + (unsigned int)dest[-1];
		dest[3] = src[3] + (average>>1);
		dest += 4;
		src += 4;
	}
}

/*unsigned char HPTTextureStruct::PaethPredictor(unsigned char a,unsigned char b,unsigned char c)
{
        // a = left, b = above, c = upper left
        unsigned int p = a + b - c;        // initial estimate
        unsigned int pa = abs(p - a);      // distances to a, b, c
        unsigned int pb = abs(p - b);
        unsigned int pc = abs(p - c);
        // return nearest of a,b,c,
        // breaking ties in order a,b,c.
        if((pa <= pb) && (pa <= pc)) return a;
        else if(pb <= pc) return b;
        else return c;
}
*/
void HPTTextureStruct::ReadPaethPNGLine(unsigned char* dest,unsigned char* src,int width,int pitch,int ycount)
{
	unsigned char a,b,c;
	if(width == 0) return;
	if(ycount == 0)
	{
		b = 0;
		c = 0;
		a = 0;
		
		dest[0] = (src[2]) + PaethPredictor(a,b,c);
		dest[1] = (src[1]) + PaethPredictor(a,b,c);
		dest[2] = (src[0]) + PaethPredictor(a,b,c);
		dest[3] = (src[3]) + PaethPredictor(a,b,c);

		dest += 4;
		src += 4;
		if(width == 1) return;

		for(int xcount = 1;xcount < width;xcount++)
		{
			b = 0;
			c = 0;

			
			a = (dest[-4]);
			dest[0] = (src[2]) + PaethPredictor(a,b,c);

			a = (dest[-3]);
			dest[1] = (src[1]) + PaethPredictor(a,b,c);

			a = (dest[-2]);
			dest[2] = (src[0]) + PaethPredictor(a,b,c);

			a = (dest[-1]);
			dest[3] = (src[3]) + PaethPredictor(a,b,c);

			dest += 4;
			src += 4;
		}
	}
	else
	{

		a = 0;
		b = (dest[-pitch]);
		c = 0;
		dest[0] = (src[2]) + PaethPredictor(a,b,c);

		b = (dest[-pitch+1]);
		(dest[1]) = (src[1]) + PaethPredictor(a,b,c);

		b = (dest[-pitch+2]);
		(dest[2]) = (src[0]) + PaethPredictor(a,b,c);

		b = (dest[-pitch+3]);
		(dest[3]) = (src[3]) + PaethPredictor(a,b,c);
		dest += 4;
		src += 4;
		if(width == 1) return;

		for(int xcount = 1;xcount < width;xcount++)
		{
			a = (dest[-4]);
			b = (dest[-pitch]);
			c = (dest[-pitch-4]);
			dest[0] = (src[2]) + PaethPredictor(a,b,c);

			a = (dest[-3]);
			b = (dest[-pitch+1]);
			c = (dest[-pitch-3]);
			(dest[1]) = (src[1]) + PaethPredictor(a,b,c);

			a = (dest[-2]);
			b = (dest[-pitch+2]);
			c = (dest[-pitch-2]);
			(dest[2]) = (src[0]) + PaethPredictor(a,b,c);

			a = (dest[-1]);
			b = (dest[-pitch+3]);
			c = (dest[-pitch-1]);
			(dest[3]) = (src[3]) + PaethPredictor(a,b,c);
			dest += 4;
			src += 4;
		}


	}
/*	
	
	for(int xcount = 0;xcount < width;xcount++)
	{
		if(xcount == 0) a = 0;
		else a = (*(dest-4));
		if(ycount == 0) b = 0;
		else b = (*(dest-pitch));
		if((xcount == 0) || (ycount == 0)) c = 0;
		else c = (*(dest-pitch-4));
		(*dest) = (*(src+2)) + PaethPredictor(a,b,c);

		if(xcount == 0) a = 0;
		else a = (*(dest-3));
		if(ycount == 0) b = 0;
		else b = (*(dest-pitch+1));
		if((xcount == 0) || (ycount == 0)) c = 0;
		else c = (*(dest-pitch-3));
		(*(dest+1)) = (*(src+1)) + PaethPredictor(a,b,c);

		if(xcount == 0) a = 0;
		else a = (*(dest-2));
		if(ycount == 0) b = 0;
		else b = (*(dest-pitch+2));
		if((xcount == 0) || (ycount == 0)) c = 0;
		else c = (*(dest-pitch-2));
		(*(dest+2)) = (*(src)) + PaethPredictor(a,b,c);

		if(xcount == 0) a = 0;
		else a = (*(dest-1));
		if(ycount == 0) b = 0;
		else b = (*(dest-pitch+3));
		if((xcount == 0) || (ycount == 0)) c = 0;
		else c = (*(dest-pitch-1));
		(*(dest+3)) = (*(src+3)) + PaethPredictor(a,b,c);
		dest += 4;
		src += 4;
	}

*/
}

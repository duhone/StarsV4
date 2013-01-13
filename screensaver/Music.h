// Music.h: interface for the Music class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUSIC_H__6DDC9451_3018_4979_825E_F8DD87D1C52D__INCLUDED_)
#define AFX_MUSIC_H__6DDC9451_3018_4979_825E_F8DD87D1C52D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <mmsystem.h>


class Music  
{
	unsigned short *data;
	unsigned short *data2;
	_int64 total_length;
	static int nchannels;
	static int current_section;
    
	int sample_rate;
	static OggVorbis_File vf;
	HWAVEOUT waveout;
	WAVEFORMATEX wFormatEx;
	static WAVEHDR header1;
	static WAVEHDR header2;
	static bool h1;
	static bool done;
public:
	Music(char path[]);
	virtual ~Music();
	static void CALLBACK Callback(HWAVEOUT hwo,UINT uMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2);

};

#endif // !defined(AFX_MUSIC_H__6DDC9451_3018_4979_825E_F8DD87D1C52D__INCLUDED_)

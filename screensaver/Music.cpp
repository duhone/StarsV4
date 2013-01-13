// Music.cpp: implementation of the Music class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Music.h"
#include <io.h>
#include <fcntl.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define BUFFER_SIZE 8192

bool Music::h1 = true;
bool Music::done = false;
WAVEHDR Music::header1;
WAVEHDR Music::header2;
int Music::nchannels;
int Music::current_section;
OggVorbis_File Music::vf;
	
Music::Music(char path[])
{
	int eof=0;
	_setmode( _fileno( stdin ), _O_BINARY );
    _setmode( _fileno( stdout ), _O_BINARY );

	FILE *filep;
	char filename[300];
	strcpy(filename,path);
	strcat(filename,"/music.ogg");
	filep = fopen(filename,"rb");

	if(ov_open(filep, &vf, NULL, 0) < 0)
	{
      fprintf(stderr,"music.ogg is invalid.\n");
      exit(1);
	}

	vorbis_info *vi=ov_info(&vf,-1);
    total_length = (ov_pcm_total(&vf,-1));
	nchannels = vi->channels;
    sample_rate = vi->rate;
	data = new unsigned short[BUFFER_SIZE*nchannels];

	data2 = new unsigned short[BUFFER_SIZE*nchannels];

	memset(data,0,BUFFER_SIZE*nchannels);
	memset(data2,0,BUFFER_SIZE*nchannels);

	wFormatEx.wFormatTag = WAVE_FORMAT_PCM;
	wFormatEx.nChannels = vi->channels;
	wFormatEx.nSamplesPerSec = vi->rate;
	wFormatEx.nAvgBytesPerSec = vi->rate*2*vi->channels;
	wFormatEx.nBlockAlign = 2*vi->channels;
	wFormatEx.wBitsPerSample = 16;
	wFormatEx.cbSize = 0;

	header1.lpData = (char*)data;
	header1.dwBufferLength = BUFFER_SIZE*nchannels*2;
	header1.dwBytesRecorded = BUFFER_SIZE*nchannels*2;
	header2.lpData = (char*)data2;
	header2.dwBufferLength = BUFFER_SIZE*nchannels*2;
	header2.dwBytesRecorded = BUFFER_SIZE*nchannels*2;


	/* Open Wave Device */
	//hr = waveOutOpen(&wOutDevice, WAVE_MAPPER, &wFormatEx,(DWORD)HPTSoundCallback,CREATE_SUSPENDED,CALLBACK_FUNCTION |WAVE_ALLOWSYNC);

	if (waveOutOpen((LPHWAVEOUT)&waveout, WAVE_MAPPER,&wFormatEx, 
                    (DWORD)Callback, 0L, CALLBACK_FUNCTION | WAVE_ALLOWSYNC)) 
    { 
       
    } 

	long bytes_read = 0;
	long temp;
	while(bytes_read < BUFFER_SIZE*nchannels*2)
	{
		temp = ov_read(&vf, header1.lpData+bytes_read, BUFFER_SIZE*nchannels*2-bytes_read,0,2,1,&current_section);
		bytes_read += temp;
		if(temp == 0)
			ov_pcm_seek(&vf,0);
	}
	
	bytes_read = 0;
	temp;
	while(bytes_read < BUFFER_SIZE*nchannels*2)
	{
		temp = ov_read(&vf, header2.lpData+bytes_read, BUFFER_SIZE*nchannels*2-bytes_read,0,2,1,&current_section);
		bytes_read += temp;
		if(temp == 0)
			ov_pcm_seek(&vf,0);
	}


	waveOutPrepareHeader(waveout,&header1,sizeof(WAVEHDR));
	waveOutPrepareHeader(waveout,&header2,sizeof(WAVEHDR));
	waveOutWrite(waveout,&header1,sizeof(WAVEHDR));
	waveOutWrite(waveout,&header2,sizeof(WAVEHDR));
	h1 = true;
}

void CALLBACK Music::Callback(HWAVEOUT hwo,UINT uMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2)
{
	if(done) return;
	if(uMsg == WOM_DONE)
	{
		if(h1)
		{
			waveOutUnprepareHeader(hwo,&header1,sizeof(WAVEHDR));
			long bytes_read = 0;
			long temp;
			while(bytes_read < BUFFER_SIZE*nchannels*2)
			{
				temp = ov_read(&vf, header1.lpData+bytes_read, BUFFER_SIZE*nchannels*2-bytes_read,0,2,1,&current_section);
				bytes_read += temp;
				if(temp == 0)
					ov_pcm_seek(&vf,0);
			}

			waveOutPrepareHeader(hwo,&header1,sizeof(WAVEHDR));
			waveOutWrite(hwo,&header1,sizeof(WAVEHDR));
			h1 = false;
		}
		else
		{
			waveOutUnprepareHeader(hwo,&header2,sizeof(WAVEHDR));
			long bytes_read = 0;
			long temp;
			while(bytes_read < BUFFER_SIZE*nchannels*2)
			{
				temp = ov_read(&vf, header2.lpData+bytes_read, BUFFER_SIZE*nchannels*2-bytes_read,0,2,1,&current_section);
				bytes_read += temp;
				if(temp == 0)
					ov_pcm_seek(&vf,0);
			}

			waveOutPrepareHeader(hwo,&header2,sizeof(WAVEHDR));
			waveOutWrite(hwo,&header2,sizeof(WAVEHDR));
			h1 = true;
		}

	}
}


Music::~Music()
{
	done = true;
	waveOutReset(waveout);
	waveOutClose(waveout);
	ov_clear(&vf);

}

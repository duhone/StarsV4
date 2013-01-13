// shader assembler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<d3dx9.h>
#include<stdio.h>
#include<vector>
#include "ShaderFile.h"
#include"zlib.h"

using namespace std;

vector<ShaderFile> shaderfiles;
	
int main(int argc, char* argv[])
{
	FILE *inputfile;
	inputfile = fopen(argv[1],"rb");
	char shaderfilename[256];
	memset(shaderfilename,0,256);
	char achar[256];
	achar[1] = 0;
	int result = fscanf(inputfile,"%c",achar);
	while(result!=EOF)
	{
		if(achar[0] == '\n')
		{
			ShaderFile shaderfile(shaderfilename);
			shaderfiles.push_back(shaderfile);
			memset(shaderfilename,0,256);
		}
		else
		{
			if((isalpha(achar[0])) || (achar[0] == ' ') || (achar[0] == '.')) strcat(shaderfilename,achar);
		}
		result = fscanf(inputfile,"%c",achar);
	
	}

	for(int count = 0;count < shaderfiles.size();count++)
	{
		shaderfiles[count].Compile();
	}
	int uncompsize = 4;
	uncompsize += 4*shaderfiles.size();
	for(count = 0;count < shaderfiles.size();count++)
	{
		uncompsize += shaderfiles[count].getCompiledSize();
	}
	
	unsigned char* compileddata = new unsigned char[uncompsize];
	int pos = 4;
	for(count = 0;count < shaderfiles.size();count++)
	{
		int temp = shaderfiles[count].getCompiledSize();
		memcpy(compileddata+pos,(unsigned char*)&temp,4);
		pos += 4;

		memcpy(compileddata+pos,shaderfiles[count].getCompiledData(),temp);
		pos += temp;
	}
	
    int compresseddatasize = uncompsize * .001 + uncompsize + 12;
    unsigned char* compresseddata = new unsigned char[compresseddatasize];
	compress2(compresseddata, (unsigned long*)&compresseddatasize, compileddata, (unsigned long)uncompsize,9);
	
	FILE* outfile = fopen("shaders.hsf","wb");
	fwrite(&uncompsize,sizeof(int),1,outfile);
	fwrite(&compresseddatasize,sizeof(int),1,outfile);
	fwrite(compresseddata,sizeof(char),compresseddatasize,outfile);
	fclose(outfile);

	return 0;
}

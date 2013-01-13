// ShaderFile.cpp: implementation of the ShaderFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShaderFile.h"
#include<d3dx9.h>
#include<stdio.h>
#include "zlib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ShaderFile::ShaderFile(char name[])
{
	strcpy(filename,name);
}

ShaderFile::~ShaderFile()
{

}

void ShaderFile::Compile()
{
	LPD3DXBUFFER shader;
	LPD3DXBUFFER errors;
	D3DXAssembleShaderFromFile(filename,NULL,NULL,0,&shader,NULL);
	//printf("%s",errors->GetBufferPointer());
	
	compileddatasize = shader->GetBufferSize();
	compileddata = new unsigned char[compileddatasize];
	memcpy(compileddata,shader->GetBufferPointer(),compileddatasize);

	
}

void ShaderFile::Compress()
{
	
    compresseddatasize = compileddatasize * .001 + compileddatasize + 12;
    compresseddata = new unsigned char[compresseddatasize];
    compress2(compresseddata, (unsigned long*)&compresseddatasize, compileddata, (unsigned long)compileddatasize,9);

}

void ShaderFile::Write(FILE *file)
{

}

int ShaderFile::getCompiledSize()
{
	return compileddatasize;
}

unsigned char* ShaderFile::getCompiledData()
{
	return compileddata;
}

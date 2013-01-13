// ShaderFile.h: interface for the ShaderFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHADERFILE_H__24D3502C_0348_452A_994B_51F56F8EB56E__INCLUDED_)
#define AFX_SHADERFILE_H__24D3502C_0348_452A_994B_51F56F8EB56E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<stdio.h>
#include<stdlib.h>

class ShaderFile  
{
	char *filedata;
	int filedatasize;
	unsigned char *compileddata;
	int compileddatasize;
	unsigned char *compresseddata;
	int compresseddatasize;
	char filename[256];
public:
	unsigned char* getCompiledData();
	int getCompiledSize();
	void Write(FILE *file);
	void Compress();
	void Compile();
	ShaderFile(char name[]);
	virtual ~ShaderFile();

};

#endif // !defined(AFX_SHADERFILE_H__24D3502C_0348_452A_994B_51F56F8EB56E__INCLUDED_)

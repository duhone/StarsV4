//---------------------------------------------------------------------------

#include <vcl.h>
#include <io.h>
#pragma hdrstop

#include "Unit1.h"
#include "zlib.h"

#include "shader.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::InsertFile1Click(TObject *Sender)
{
        AnsiString temp;
        VertexFile* tempvf;
        if(OpenFile->Execute())
        {
                DWORD dummy;

                vertex_shaders->Items->Add(OpenFile->FileName);
                tempvf = new VertexFile;
                tempvf->filename = OpenFile->FileName;
                tempvf->hfile = CreateFile(OpenFile->FileName.c_str(),GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                tempvf->filesize = GetFileSize(tempvf->hfile, NULL);
                ReadFile(tempvf->hfile, tempvf->data, tempvf->filesize, &dummy, NULL);
                //AssembleShader(tempvf->data, (int*)&tempvf->filesize, tempvf->assembled_data, &tempvf->assembled_size);

                vertex_files.push_back(tempvf);
        }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Build1Click(TObject *Sender)
{
        if(SaveFile->Execute())
        {
                //determine the size of all the files;
                vector<VertexFile*>::iterator iter;
                int size=0;
                for(iter = vertex_files.begin(); iter != vertex_files.end(); iter++)
                {
                        size += (*iter)->filesize;
                }

                //add on an extra int for each file as well as one to store the # of files
                size += vertex_files.size() * sizeof(int);
                size += sizeof(int);

                char* data = new char[size];
                char* temp = data;

                //layout the uncompressed data in memory
                DWORD dummy;

                int num_files = vertex_files.size();
                memcpy(temp, &num_files, sizeof(int));   //write out the # of files
                temp += sizeof(int);

                for(iter = vertex_files.begin(); iter != vertex_files.end(); iter++)
                {
                        memcpy(temp, &(*iter)->filesize, sizeof(int));  //write out the file size
                        temp += sizeof(int);

                        ReadFile((*iter)->hfile, temp, (*iter)->filesize, &dummy, NULL);  //write out the file data
                        temp+=(*iter)->filesize;
                }

                //zlib the data
                int zlib_size;
                char *zlib_data;

                zlib_size = size * .001 + size + 12;
                zlib_data = new unsigned char[zlib_size];
                compress2(zlib_data, (unsigned long*)&zlib_size, data, (unsigned long)size,9);

                //open the dialog file and write out the data
                FILE* fp;


                fp=fopen(SaveFile->FileName.c_str(), "wb");
                fwrite(&size, sizeof(int), 1, fp); //size of data uncompressed
                fwrite(&zlib_size, sizeof(int), 1, fp); //size of data compressed. ie the size of the file - 4bytes
                fwrite(zlib_data, zlib_size, 1, fp);

                fclose(fp);
                delete[] zlib_data;
                delete[] data;

        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DeleteFile1Click(TObject *Sender)
{
        if(vertex_shaders->ItemIndex >= 0)
        {


                vector<VertexFile*>::iterator iter;
                iter = vertex_files.begin();
                advance(iter, vertex_shaders->ItemIndex);

                delete (*iter);
                vertex_files.erase(iter);

                vertex_shaders->Items->Delete(vertex_shaders->ItemIndex);
                
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
        vector<VertexFile*>::iterator iter;
        VertexFile* temp;
        if(vertex_files.empty()) return;

        for( int i = 0; i < vertex_files.size(); i++)
        {
                temp = vertex_files[i];
                delete temp;
        }

        vertex_files.clear();
}
//---------------------------------------------------------------------------




VertexFile::~VertexFile()
{
       if(data != 0) delete[] data;

       //TODO: Add your source code here
}

VertexFile::VertexFile()
{
        data = 0;
        filesize = 0;
        //TODO: Add your source code here
}

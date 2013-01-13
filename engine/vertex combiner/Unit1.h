//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <vector.h>


//---------------------------------------------------------------------------
class VertexFile
{
public:
        
        AnsiString filename;
        DWORD filesize;
        //int assembled_size;
        char* data;
        //char* assembled_data;
        HANDLE hfile;
        ~VertexFile();
        VertexFile();

};


class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TStatusBar *StatusBar;
        TPopupMenu *PopupMenu1;
        TMenuItem *InsertFile1;
        TMenuItem *DeleteFile1;
        TMenuItem *Build1;
        TOpenDialog *OpenFile;
        TSaveDialog *SaveFile;
        TListBox *vertex_shaders;
        void __fastcall InsertFile1Click(TObject *Sender);
        void __fastcall Build1Click(TObject *Sender);
        void __fastcall DeleteFile1Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        vector<VertexFile*> vertex_files;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

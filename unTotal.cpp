//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "unTotal.h"

#include "unGeneral.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tfm3 *fm3;
//---------------------------------------------------------------------------
__fastcall Tfm3::Tfm3(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tfm3::FormClose(TObject *Sender, TCloseAction &Action)
{
    Action = caHide;    
}
//---------------------------------------------------------------------------
void __fastcall Tfm3::FormResize(TObject *Sender)
{
    img->Picture->Bitmap->Width = img->Width;
    img->Picture->Bitmap->Height = img->Height;
	fm->RedrawGraph();
}
//---------------------------------------------------------------------------

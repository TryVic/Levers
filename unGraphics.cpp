//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "unGraphics.h"

#include "unGeneral.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmGraphics *fmGraphics;
//---------------------------------------------------------------------------
__fastcall TfmGraphics::TfmGraphics(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmGraphics::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	Action = caHide;	
}
//---------------------------------------------------------------------------
void __fastcall TfmGraphics::FormCreate(TObject *Sender)
{
	fm->InitGraphics();
}
//---------------------------------------------------------------------------
void __fastcall TfmGraphics::cbAutoClick(TObject *Sender)
{
	fm->SetAutoCalc(cbAuto->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TfmGraphics::btRecalcClick(TObject *Sender)
{
	try
    {
		fm->RebuildGraphics();
    }
    catch (...)
    {
    	return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfmGraphics::FormResize(TObject *Sender)
{
	Image->Picture->Bitmap->Width = Image->Width;
    Image->Picture->Bitmap->Height = Image->Height;
	fm->RedrawGraph();
}
//---------------------------------------------------------------------------
void __fastcall TfmGraphics::btGraphClick(TObject *Sender)
{
    Tag = dynamic_cast<TSpeedButton*> (Sender)->Tag;
    fm->SetGraphSelect(Tag);
	fm->RedrawGraph();
    fm->Repaint();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#ifndef unGraphicsH
#define unGraphicsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>

//---------------------------------------------------------------------------
class TfmGraphics : public TForm
{
__published:	// IDE-managed Components
	TImage *Image;
	TPanel *pnControl;
	TCheckBox *cbAuto;
	TButton *btRecalc;
    TPanel *pnButton;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall cbAutoClick(TObject *Sender);
	void __fastcall btRecalcClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btGraphClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfmGraphics(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmGraphics *fmGraphics;
//---------------------------------------------------------------------------
#endif

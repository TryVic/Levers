//---------------------------------------------------------------------------

#ifndef unTotalH
#define unTotalH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class Tfm3 : public TForm
{
__published:	// IDE-managed Components
    TImage *img;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormResize(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall Tfm3(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tfm3 *fm3;
//---------------------------------------------------------------------------
#endif

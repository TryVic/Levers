//---------------------------------------------------------------------------

#ifndef unGeneralH
#define unGeneralH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>

#include "unVariant.h"
//---------------------------------------------------------------------------
struct TFieldControl
{
	TLabel	*Label;
    TEdit	*Edit;
    TUpDown	*UD;
};

struct TStaticInfo
{

};
//---------------------------------------------------------------------------
class Tfm : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnImage;
	TPanel *pnControl;
	TGroupBox *gbParam;
	TGroupBox *gbAngle;
	TEdit *edAngle;
	TUpDown *udAngle;
	TLabel *lbHint;
	TLabel *lbSpeed;
	TTrackBar *tbAngleSpeed;
	TLabel *lbAngleSpeed;
	TGroupBox *gbView;
	TCheckBox *cbAnimation;
	TCheckBox *cbSize;
	TCheckBox *cbAngle;
	TCheckBox *cbText;
	TGroupBox *gbWindows;
	TButton *btGraphics;
	TTimer *Timer;
	TScrollBar *sbX;
	TScrollBar *sbY;
	TPanel *pn;
	TImage *image;
	TLabel *lbScale;
	TComboBox *cbScale;
    TCheckBox *cbForce;
    TButton *Button1;
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall TimerTimer(TObject *Sender);
	void __fastcall cbAnimationClick(TObject *Sender);
	void __fastcall tbAngleSpeedChange(TObject *Sender);
	void __fastcall udAngleClick(TObject *Sender, TUDBtnType Button);
	void __fastcall cbSizeClick(TObject *Sender);
	void __fastcall cbAngleClick(TObject *Sender);
	void __fastcall cbTextClick(TObject *Sender);
	void __fastcall sbXChange(TObject *Sender);
	void __fastcall btGraphicsClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall cbScaleChange(TObject *Sender);
    void __fastcall cbForceClick(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall Tfm(TComponent* Owner);

    void InitModel (void);
    void InitGraphics (void);

    void SetAutoCalc (bool vValue);
    void SetGraphSelect (int vGraph);

    void DrawBackground (void);
    void DrawLinks (void);
    void DrawJoints (void);
    void DrawSizes (void);
    void DrawAngles (void);
    void DrawText (void);
    void DrawForce (void);

    void CalcForce(TGraphInfo &vInfo, double &vX, double &vY);

    void RebuildGraphics (void);
    void DrawOneGraphic (TRect vRect, TGraphInfo &vInfo, TGraphView vView);
    void DrawGraphTrace (TRect vRect, TGraphInfo &vInfo, TGraphView vView,
        double vAngle);
    void DrawTotal (TRect vRect, TGraphView vView);
    void RedrawGraph (void);
    void UpdateGraph (void);

    void __fastcall udChangeParam (TObject *Sender, TUDBtnType Button);
};
//---------------------------------------------------------------------------
extern PACKAGE Tfm *fm;
//---------------------------------------------------------------------------
#endif

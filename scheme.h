//---------------------------------------------------------------------------
#ifndef schemeH
#define schemeH
//---------------------------------------------------------------------------
#include <graphics.hpp>
#include "kinematics.h"
//---------------------------------------------------------------------------
void DrawRocker(TCanvas *vCanvas, TPointd vCenter, float vAngle,
	int vLeft, int vRight, int vRadius);
void DrawRocker(TCanvas *vCanvas, TPointd vCenter, TPointd vEdge1,
	TPointd vEdge2, int vRadius);
void DrawAxis(TCanvas *vCanvas, TPointd vStart, TPointd vEnd, int vSize);
//---------------------------------------------------------------------------
void DrawMount(TCanvas *vCanvas, TPointd vJoint, float vAngle, int vSize);
void DrawGuide(TCanvas *vCanvas, TPointd vBegin, float vAngle, int vSize);
void DrawTranslationalJoint(TCanvas *vCanvas, TPointd vCenter,
	int vWidth, int vHeight, float vAngle);
void DrawRotationalJoint(TCanvas *vCanvas, TPointd vCenter, int vRadius);
//---------------------------------------------------------------------------
void DrawSize(TCanvas *vCanvas, TPointd vP1, TPointd vP2, int vLength,
	float vAngle, int vDisplace, BYTE vUp, AnsiString vCaption);
void DrawAngle(TCanvas *vCanvas, TPointd vCenter, TPointd vStart, TPointd vEnd,
	int vRadius, AnsiString vCaption);
void DrawArrow(TCanvas *vCanvas, TPointd vCenter, double vAngle, double vLength);
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif

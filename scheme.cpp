//---------------------------------------------------------------------------
#pragma hdrstop

#include "scheme.h"

#include <math.h>
#include <math.hpp>

#include <Dialogs.hpp>

#include "kinematics.h"
//---------------------------------------------------------------------------
#define BV_RAD		M_PI/180.
#define BV_30_RAD   0.5235987755983
//---------------------------------------------------------------------------
void DrawRocker(TCanvas *vCanvas, TPointd vCenter, float vAngle,
	int vLeft, int vRight, int vRadius)
{
	TPoint	p[3];
    double	a1 = vAngle, a2 = a1+M_PI;

    p[0] = Point(Round(vRight*cos(a1)), Round(vRight*sin(a1)));
    p[1] = Point(Round(vLeft*cos(a2)), Round(vLeft*sin(a2)));
    p[2] = Point(Round(vRadius*cos(a1)), Round(vRadius*sin(a1)));

    vCanvas->MoveTo(vCenter.x+p[2].x, vCenter.y+p[2].y);
    vCanvas->LineTo(vCenter.x+p[0].x, vCenter.y+p[0].y);
    vCanvas->MoveTo(vCenter.x-p[2].x, vCenter.y-p[2].y);
	vCanvas->LineTo(vCenter.x+p[1].x, vCenter.y+p[1].y);

    vCanvas->Arc(vCenter.x-vRadius, vCenter.y-vRadius,
    	vCenter.x+vRadius, vCenter.y+vRadius,
        vCenter.x+p[0].x, vCenter.y+p[0].y,
        vCenter.x+p[1].x, vCenter.y+p[1].y);
}
//---------------------------------------------------------------------------
void DrawRocker(TCanvas *vCanvas, TPointd vCenter, TPointd vEdge1,
	TPointd vEdge2, int vRadius)
{
    vCanvas->MoveTo(vEdge1.x, vEdge1.y);
    vCanvas->LineTo(vCenter.x, vCenter.y);
	vCanvas->LineTo(vEdge2.x, vEdge2.y);

    vCanvas->Arc(vCenter.x-vRadius, vCenter.y-vRadius,
    	vCenter.x+vRadius, vCenter.y+vRadius,
        vEdge2.x, vEdge2.y,
        vEdge1.x, vEdge1.y);
}
//---------------------------------------------------------------------------
void DrawAxis(TCanvas *vCanvas, TPointd vStart, TPointd vEnd, int vSize)
{
	double	a = GetAngle(vEnd.x-vStart.x, vEnd.y-vStart.y);

    vCanvas->Pen->Width = 2;
    vCanvas->Pen->Style = psSolid;
    vCanvas->MoveTo(Round(vStart.x), Round(vStart.y));
    vCanvas->LineTo(Round(vStart.x+vSize*cos(a)),
    	Round(vStart.y+vSize*sin(a)));
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void DrawMount(TCanvas *vCanvas, TPointd vJoint, float vAngle, int vSize)
{
	TPoint	p[3];

    p[0] = Point(Round(vJoint.x), Round(vJoint.y));
    p[1] = Point(Round(vJoint.x+vSize*cos(vAngle-BV_30_RAD)),
    	Round(vJoint.y+vSize*sin(vAngle-BV_30_RAD)));
    p[2] = Point(Round(vJoint.x+vSize*cos(vAngle+BV_30_RAD)),
    	Round(vJoint.y+vSize*sin(vAngle+BV_30_RAD)));

    vCanvas->Pen->Width = 2;
    vCanvas->Brush->Style = bsFDiagonal;
    vCanvas->Polygon(p, 3);
}
//---------------------------------------------------------------------------
void DrawGuide(TCanvas *vCanvas, TPointd vBegin, float vAngle, int vSize)
{
	TPoint	p[4];

    p[0] = Point(Round(vBegin.x-(vSize/2.)*cos(vAngle)),
    	Round(vBegin.y-(vSize/2.)*sin(vAngle)));
    p[1] = Point(Round(p[0].x+5*cos(vAngle-M_PI_2)),
    	Round(p[0].y+5*sin(vAngle-M_PI_2)));
    p[2] = Point(Round(p[1].x+vSize*cos(vAngle)),
    	Round(p[1].y+vSize*sin(vAngle)));
    p[3] = Point(Round(p[0].x+vSize*cos(vAngle)),
    	Round(p[0].y+vSize*sin(vAngle)));

    vCanvas->Pen->Style = psClear;
    vCanvas->Brush->Style = bsFDiagonal;
    vCanvas->Polygon(p, 4);

    vCanvas->Pen->Width = 2;
    vCanvas->Pen->Style = psSolid;
    vCanvas->MoveTo(p[0].x, p[0].y);
    vCanvas->LineTo(p[3].x, p[3].y);
}
//---------------------------------------------------------------------------
void DrawTranslationalJoint(TCanvas *vCanvas, TPointd vCenter,
	int vWidth, int vHeight, float vAngle)
{
	double	a = GetAngle(vWidth/2., vHeight/2.);
    double	l = GetDistance(vWidth/2., vHeight/2.);
    double	x1,y1,x2,y2;
	TPoint	p[4];

    x1 = l*cos(a+vAngle); y1 = l*sin(a+vAngle);
    x2 = l*cos(-a+vAngle); y2 = l*sin(-a+vAngle);
    p[0] = Point(Round(vCenter.x+x1), Round(vCenter.y+y1));
	p[1] = Point(Round(vCenter.x+x2), Round(vCenter.y+y2));
    p[2] = Point(Round(vCenter.x-x1), Round(vCenter.y-y1));
	p[3] = Point(Round(vCenter.x-x2), Round(vCenter.y-y2));

    vCanvas->Brush->Style = bsSolid;
    vCanvas->Pen->Width = 2;
    vCanvas->Polygon(p, 4);
}
//---------------------------------------------------------------------------
void DrawRotationalJoint(TCanvas *vCanvas, TPointd vCenter, int vRadius)
{
    vCanvas->Brush->Style = bsSolid;
    vCanvas->Pen->Width = 2;
    vCanvas->Ellipse(vCenter.x-vRadius, vCenter.y-vRadius,
    	vCenter.x+vRadius, vCenter.y+vRadius);
}
//---------------------------------------------------------------------------
void DrawSize(TCanvas *vCanvas, TPointd vP1, TPointd vP2, int vLength,
	float vAngle, int vDisplace, BYTE vUp, AnsiString vCaption)
{
	TPointd	pLD, pRD, pLB, pRB;
	int     len,
    		w2 = vCanvas->TextWidth(vCaption)/2,
    		h = vCanvas->TextHeight(vCaption), h2 = h/2;
    double	a1 = vAngle, a2, c2, s2, c1, s1;

    if (vUp != 0) { len = vLength; }
    	else { len = -vLength; a1 += M_PI; }

    c1 = cos(a1); s1 = sin(a1);
    a2 = a1 + M_PI_2; c2 = cos(a2); s2 = sin(a2);
    pLD = Pointd(vP1.x+vDisplace*c2, vP1.y+vDisplace*s2);
    pRD = Pointd(pLD.x+len*c1, pLD.y+len*s1);
    pLB = Pointd(vP1.x+(vDisplace+h2)*c2, vP1.y+(vDisplace+h2)*s2);
    a2 = GetAngle(pRD.x-vP2.x, pRD.y-vP2.y);
    pRB = Pointd(pRD.x+h2*cos(a2), pRD.y+h2*sin(a2));

    vCanvas->Pen->Width = 1;
    vCanvas->Brush->Style = bsSolid;

    // Left bracket
	vCanvas->MoveTo(Round(vP1.x), Round(vP1.y));
	vCanvas->LineTo(Round(pLB.x), Round(pLB.y));
	// Right bracket
	vCanvas->MoveTo(Round(vP2.x), Round(vP2.y));
	vCanvas->LineTo(Round(pRB.x), Round(pRB.y));
    // Line Length
    vCanvas->MoveTo(Round(pLD.x), Round(pLD.y));
	vCanvas->LineTo(Round(pRD.x), Round(pRD.y));
	// Text
	vCanvas->TextOutA(Round(pLD.x+(len/2.)*c1-w2),
    	Round(pLD.y+(len/2.)*s1+h2), vCaption);
}
//---------------------------------------------------------------------------
void DrawAngle(TCanvas *vCanvas, TPointd vCenter, TPointd vStart, TPointd vEnd,
	int vRadius, AnsiString vCaption)
{
	int		w = vCanvas->TextWidth(vCaption),
    	h = vCanvas->TextHeight(vCaption);

    double	endAngle = GetAngle(vEnd.x,vEnd.y),
    	xe = Round((vRadius+5)*cos(endAngle)),
    	ye = Round((vRadius+5)*sin(endAngle));

    double	startAngle = GetAngle(vStart.x,vStart.y),
    	xs = Round((vRadius+5)*cos(startAngle)),
    	ys = Round((vRadius+5)*sin(startAngle));

	double	temp = fabs(endAngle-startAngle),
    	angle = (endAngle>=startAngle)?(endAngle+startAngle)/2.:
    	(endAngle+startAngle)/2.+M_PI;

    vCanvas->Pen->Width = 1;
    vCanvas->Brush->Style = bsSolid;

    vCanvas->MoveTo(vCenter.x, vCenter.y);
	vCanvas->LineTo(vCenter.x+xs, vCenter.y+ys);
    vCanvas->MoveTo(vCenter.x, vCenter.y);
	vCanvas->LineTo(vCenter.x+xe, vCenter.y+ye);

    if (temp>0.04 && temp<M_2PI-0.04) vCanvas->Arc(
    	vCenter.x-vRadius, vCenter.y-vRadius,
    	vCenter.x+vRadius, vCenter.y+vRadius,
        vCenter.x+xs, vCenter.y+ys,
        vCenter.x+xe, vCenter.y+ye);

    vCanvas->TextOutA(Round(vCenter.x+vRadius*cos(angle)-w/2.),
    	Round(vCenter.y+vRadius*sin(angle)+h/2.), vCaption);
}
//---------------------------------------------------------------------------
void DrawArrow (TCanvas *vCanvas, TPointd vCenter, double vAngle, double vLength)
{
    TPoint  p[3];
    int     x, y;

    x = Round(vCenter.x+vLength*cos(vAngle));
    y = Round(vCenter.y+vLength*sin(vAngle));
    p[0] = Point(x, y);

    x = Round(p[0].x-5*cos(vAngle-BV_30_RAD));
    y = Round(p[0].y-5*sin(vAngle-BV_30_RAD));
    p[1] = Point(x, y);

    x = Round(p[0].x-5*cos(vAngle+BV_30_RAD));
    y = Round(p[0].y-5*sin(vAngle+BV_30_RAD));
    p[2] = Point(x, y);

    vCanvas->Brush->Style = bsSolid;
    vCanvas->Pen->Width = 1;

    vCanvas->MoveTo(vCenter.x, vCenter.y);
    vCanvas->LineTo(p[0].x, p[0].y);

    vCanvas->Polygon(p, 3);
}
//---------------------------------------------------------------------------
#pragma package(smart_init)


//---------------------------------------------------------------------------

#ifndef unVariantH
#define unVariantH

#include <classes.hpp>

#include "kinematics.h"
#include "scheme.h"
//---------------------------------------------------------------------------
#define	BV_OX		    65534
#define BV_OY		    65535

#define BV_OUT          65534
#define BV_IGNORED      65535
#define BV_DIGIT        2
// 2 - 225*Пи/30
#define BV_ANGLE_SPEED  23.564926446717
#define BV_SPEED_2      555.30575843922
#define BV_G            9.87
#define BV_FORCE_WORK   500000
#define BV_STONE_WEIGHT 500
#define BV_LEVER_WEIGHT 300
//---------------------------------------------------------------------------
enum TLeverKind {lkEdge, lkRocker, lkTriangle, lkAxis, lkStone};
enum TJointKind {jkNone, jkRotational, jkTranslational};
enum TElementKind {ekMount, ekGuide};
enum TAngleKind {akVertical, akHorizontal, akFree};
enum TGraphKind {gkPointX, gkPointY, gkUnit};
enum TGraphView {gvValue, gvSpeed, gvAccel, gvForce, gvMoment, gvInertia,
    gvWork, gvTransitive, gvEpsilon, gvTransitiveOpt};

struct TVertexInfo
{
	AnsiString	Name;		// название точки
	TJointKind	kind;		// тип соединения (вращательное, поступательное)
    TAngleKind	angleKind;	// угол прорисовки
    TPointd		point;
	WORD		index;		// индекс точки угла
    double		angle;		// угол из последней точки в эту
};

struct TLeverInfo
{
	TLeverKind	kind;
    WORD		p1;
    WORD		p2;
    WORD		p3;
    short int	value;
};

struct TDistanceInfo
{
	AnsiString	Caption;
    WORD		min;
    WORD		max;
    WORD		current;
};

struct TSchemeInfo
{
	TElementKind	kind;
	WORD		point;
    double		angle;
};

struct TSchemeSize
{
	TAngleKind	kind;
    BYTE		up;
	WORD		p1;
    WORD		p2;
	WORD		distance;		// Ссылка на длину
    short int	displace;		// Смещение
};

struct TSchemeAngle
{
	WORD		p1;
    WORD		p2;
    WORD		p3;
};

struct TGraphData
{
    TPointd position;
	double	value;	// для звена-угл, для точки-координата
    double	speed;
    double  accel;
    double	accelX;
    double  accelY;
    double  forceX;
    double  forceY;
    double  energy;
};

struct TGraphInfo
{
	AnsiString	Caption;
    TGraphKind	kind;
    TGraphData  data[360];

	double	    minValue, minSpeed, minAccel, minForce;
    double	    maxValue, maxSpeed, maxAccel, maxForce;

    WORD		point;      // Точка которая двигается
    WORD		center;     // Точка относительно которой вращается

    WORD        pBase;      // Точка опоры                       1
    WORD        pForceIn1;  // Действующая сила на точку 1       2   1
    WORD        pForceIn2;  // Действующая сила на точку 2
    WORD        pForceOut;  // Компенсирующая сила с точки       3

    WORD        lever;      // Звено для которого ищутся силы    4   2
    WORD        graphIn1;   // График с которого берется сила 1  5   3
    WORD        graphIn2;   // График с которого берется сила 2
};

struct TMechanicData
{
    double      Moment;
    double      Inertia;
    double      Epsilon;
    double      Work;
    double      Transitive;
    double      TransitiveOpt;
};

struct TMechanic
{
    TMechanicData   data[360];
    double      maxMoment, minMoment;
    double      maxInertia, minInertia;
    double      maxWork, minWork;
    double      minTransitive, maxTransitive;
    double      minTransitiveOpt, maxTransitiveOpt;
    double      minEpsilon, maxEpsilon;
    double      averageMoment;
};

struct TLeverParam
{
    int         doWork;     // -1 - отрицательное, 1 - положительное
    bool        dirRotation;// 0 - против, 1 - по часовой

	WORD		countVertexs;
    WORD		countDistances;
    WORD		countLevers;
    WORD		countElements;
    WORD		countSizes;
    WORD		countAngles;
    WORD		countGraphics;

    TVertexInfo	*Vertexs;
    TDistanceInfo *Distances;
    TLeverInfo	*Levers;
    TSchemeInfo	*Elements;
    TSchemeSize	*Sizes;
    TSchemeAngle*Angles;
    TGraphInfo	*Graphics;
    TMechanic   TotalInfo;
};
//---------------------------------------------------------------------------
void GetParameters (TLeverParam &vParam);
void ReleaseParameters (TLeverParam &vParam);
void CalcMechanic (double vAngle, TVertexInfo *vVertex, TDistanceInfo *vDistances);
void UpdateAxis (TLeverParam &vParam);
//---------------------------------------------------------------------------

void UpdateAxis(TLeverParam &vParam)
{
}
/**/
//---------------------------------------------------------------------------
TVertexInfo VertexInfo(AnsiString vName, TJointKind vJoint,
	TAngleKind vAngleKind, WORD vIndex = 0)
{
	TVertexInfo	t; t.Name = vName; t.kind = vJoint;
    t.angleKind = vAngleKind; t.index = vIndex; return t;
}
//---------------------------------------------------------------------------
TDistanceInfo DistanceInfo(AnsiString vCaption, WORD vMin, WORD vMax,
	WORD vCurrent)
{
	TDistanceInfo	t; t.Caption = vCaption; t.min = vMin; t.max = vMax;
    t.current = vCurrent; return t;
}
//---------------------------------------------------------------------------
TLeverInfo __inline LeverInfo(TLeverKind vLeverKind, WORD vP1, WORD vP2,
	WORD vP3 = 0, WORD vValue = 0)
{
	TLeverInfo	t; t.kind = vLeverKind; t.value = vValue;
    t.p1 = vP1; t.p2 = vP2; t.p3 = vP3; return t;
}
//---------------------------------------------------------------------------
TSchemeInfo __inline ElementInfo(TElementKind vKind, WORD vPoint,
	double vAngle)
{
	TSchemeInfo t; t.kind = vKind; t.point = vPoint; t.angle = vAngle; return t;
}
//---------------------------------------------------------------------------
TSchemeSize __inline SizeInfo(TAngleKind vKind, BYTE vUp, WORD vP1, WORD vP2,
	WORD vDistance, WORD vDisplace = 20)
{
	TSchemeSize t; t.kind = vKind; t.up = vUp; t.displace = vDisplace;
    t.p1 = vP1; t.p2 = vP2; t.distance = vDistance; return t;
}
//---------------------------------------------------------------------------
TSchemeAngle __inline AngleInfo(WORD vP1, WORD vP2, WORD vP3)
{
	TSchemeAngle t; t.p1 = vP1; t.p2 = vP2; t.p3 = vP3; return t;
}
//---------------------------------------------------------------------------
TGraphInfo GraphInfo(AnsiString vCaption, TGraphKind vKind,
	WORD vPoint, WORD vCenter = 0)
{
	TGraphInfo t;

    t.pForceIn1 = t.pForceIn2 = t.pForceOut = 0;
    t.lever = t.graphIn1 = t.graphIn2 = 0;
    t.kind = vKind; t.point = vPoint; t.Caption = vCaption;
    t.center = vCenter; return t;
}
//---------------------------------------------------------------------------
#define FORCE_FREE_EDGE(gr, gIn, lev)  ForceInfo(gr, BV_IGNORED, BV_IGNORED, \
    BV_IGNORED, BV_IGNORED, (gIn), BV_IGNORED, (lev))

#define FORCE_ROCKER(gr, pBase, pIn, pOut, gIn, lev)    ForceInfo(gr, (pBase), \
    (pIn), BV_IGNORED, (pOut), (gIn), BV_IGNORED, (lev))

#define FORCE_STONE(gr, gIn, lev)   ForceInfo(gr, BV_IGNORED, BV_IGNORED, \
    BV_IGNORED, BV_IGNORED, (gIn), BV_IGNORED, (lev))

#define FORCE_TRIANGLE(gr, pBase, pIn, pOut, gIn, lev)  ForceInfo(gr, (pBase), \
    (pIn), BV_IGNORED, (pOut), (gIn), BV_IGNORED, (lev))

#define FORCE_FREE_TRIANGLE(gr, pBase, pIn1, pIn2, pOut, gIn1, gIn2, lev)  \
    ForceInfo(gr, (pBase), (pIn1), (pIn2), (pOut), (gIn1), (gIn2), (lev))
//---------------------------------------------------------------------------
void ForceInfo(TGraphInfo &vGraph, WORD vBase, WORD vPtForceIn1, WORD vPtForceIn2,
    WORD vPtForceOut, WORD vGraphIn1, WORD vGraphIn2, WORD vLever )
{
    vGraph.pBase = vBase;
	vGraph.pForceIn1 = vPtForceIn1; vGraph.pForceIn2 = vPtForceIn2;
    vGraph.pForceOut = vPtForceOut; vGraph.lever = vLever;
    vGraph.graphIn1 = vGraphIn1; vGraph.graphIn2 = vGraphIn2;
}
//---------------------------------------------------------------------------
#endif

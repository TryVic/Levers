//---------------------------------------------------------------------------
// Вариант: Гладковская
// Звенья:
//	0 - ОА
//	1 - OB
//	2 - CD
//	3 - AC, a
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include <math.h>
#include <math.hpp>

#include "unVariant.h"
//---------------------------------------------------------------------------
void GetParameters (TLeverParam &vParam)
{
    vParam.doWork = -1;

	// Вершины
	vParam.countVertexs = 6;
    vParam.Vertexs = new TVertexInfo[vParam.countVertexs];
    vParam.Vertexs[0] = VertexInfo("O", jkRotational, akFree);
    vParam.Vertexs[1] = VertexInfo("A", jkRotational, akFree, 2);
    vParam.Vertexs[2] = VertexInfo("D", jkRotational, akFree);
    vParam.Vertexs[3] = VertexInfo("O1", jkRotational, akFree);
    vParam.Vertexs[4] = VertexInfo("C", jkRotational, akVertical);
    vParam.Vertexs[5] = VertexInfo("B", jkTranslational, akVertical);

    // Длины
    vParam.countDistances = 5;
    vParam.Distances = new TDistanceInfo[vParam.countDistances];
    vParam.Distances[0] = DistanceInfo("OA, AB", 10, 1000, 150);
    vParam.Distances[1] = DistanceInfo("OO1", 10, 1000, 45);
    vParam.Distances[2] = DistanceInfo("DO1", 10, 1000, 90);
    vParam.Distances[3] = DistanceInfo("CO1", 10, 1000, 120);
    vParam.Distances[4] = DistanceInfo("CB", 10, 1000, 350);

    // Звенья
    vParam.countLevers = 5;
    vParam.Levers = new TLeverInfo[vParam.countLevers];
    vParam.Levers[0] = LeverInfo(lkEdge, 0, 1);
    vParam.Levers[1] = LeverInfo(lkEdge, 1, 2);
    vParam.Levers[2] = LeverInfo(lkRocker, 2, 3, 4);
    vParam.Levers[3] = LeverInfo(lkEdge, 4, 5);
    vParam.Levers[4] = LeverInfo(lkStone, 5, 0);

    // Элементы соединений

    vParam.countElements = 3;
    vParam.Elements = new TSchemeInfo[vParam.countElements];
    vParam.Elements[0] = ElementInfo(ekMount, 0, M_PI);
    vParam.Elements[1] = ElementInfo(ekMount, 3, -M_PI_2);
    vParam.Elements[2] = ElementInfo(ekGuide, 0, M_PI_2);

    /*
    // Размеры
    vParam.countSizes = 6;
    vParam.Sizes = new TSchemeSize[vParam.countSizes];
    vParam.Sizes[0] = SizeInfo(akVertical, 0, 0, 3, 5, 75);
    vParam.Sizes[1] = SizeInfo(akHorizontal, 1, 3, 0, 4);
    vParam.Sizes[2] = SizeInfo(akFree, 1, 1, 2, 0);
    vParam.Sizes[3] = SizeInfo(akFree, 0, 4, 5, 3);
    vParam.Sizes[4] = SizeInfo(akFree, 1, 2, 3, 1);
    vParam.Sizes[5] = SizeInfo(akFree, 0, 3, 4, 2);

    // Углы
    vParam.countAngles = 3;
	vParam.Angles = new TSchemeAngle[vParam.countAngles];
    vParam.Angles[0] = AngleInfo(BV_OX, 0, 1);
    vParam.Angles[1] = AngleInfo(BV_OY, 5, 4);
    vParam.Angles[2] = AngleInfo(BV_OX, 3, 4);
    */

    // Графики
    vParam.countGraphics = 5;
    vParam.Graphics = new TGraphInfo[vParam.countGraphics];

    vParam.Graphics[0] =  GraphInfo("Звено OA", gkUnit, 1, 0);
    FORCE_ROCKER(vParam.Graphics[0], 0, 1, 1, 1, 0);

    vParam.Graphics[1] =  GraphInfo("Звено AD", gkUnit, 2, 1);
    FORCE_FREE_EDGE(vParam.Graphics[1], 2, 1);

    vParam.Graphics[2] =  GraphInfo("Звено DO1C", gkUnit, 4, 3);
    FORCE_ROCKER(vParam.Graphics[2], 3, 4, 2, 3, 2);

    vParam.Graphics[3] =  GraphInfo("Звено CB", gkUnit, 5, 4);
    FORCE_FREE_EDGE(vParam.Graphics[3], 4, 3);

    vParam.Graphics[4] =  GraphInfo("Точка B", gkPointY, 5);
    FORCE_STONE(vParam.Graphics[4], BV_OUT, 4);
}
//---------------------------------------------------------------------------
void ReleaseParameters (TLeverParam &vParam)
{
	vParam.countVertexs = 0; if (vParam.Vertexs) delete []vParam.Vertexs;
    vParam.countDistances = 0; if (vParam.Distances) delete []vParam.Distances;
	vParam.countLevers = 0; if (vParam.Levers) delete []vParam.Levers;
    vParam.countElements = 0; if (vParam.Elements) delete []vParam.Elements;
    vParam.countSizes = 0; if (vParam.Sizes) delete []vParam.Sizes;
    vParam.countAngles = 0; if (vParam.Angles) delete []vParam.Angles;
    vParam.countGraphics = 0; if (vParam.Graphics) delete []vParam.Graphics;
}
//---------------------------------------------------------------------------
void CalcMechanic (double vAngle, TVertexInfo *vVertex, TDistanceInfo *vDistance)
{
	TVertexInfo	*lp;
    double		len, lOA, lAD, lOO1, lCO1, lDO1, lCB;
    double		x, y, d, a;
    TPointd		pA, pO1, pC, pD, pB;

    lOA = lAD = vDistance[0].current;
    lOO1 = vDistance[1].current;
    lDO1 = vDistance[2].current;
    lCO1 = vDistance[3].current;
    lCB = vDistance[4].current;
    // 0 - очивидно
    lp = &vVertex[0]; lp->point = Pointd(0,0); lp->angle = 0;
    // 1 - точка A
	lp = &vVertex[1]; len = lOA;
    x = len*cos(vAngle); y = len*sin(vAngle);
    lp->point = pA = Pointd(x,y);
    lp->angle = GetAngle(x,y);
    // 3 - Точка O1
    lp = &vVertex[3];
    x = 0; y = -lOO1;
    lp->point = pO1 = Pointd(x, y);
    lp->angle = a = GetAngle(pA.x-pO1.x, pA.y-pO1.y);
    // 2 - Точка D
    lp = &vVertex[2];
    len = GetDistance(pA.x-pO1.x, pA.y-pO1.y);
    a = a - ArcCos((lAD*lAD-len*len-lDO1*lDO1)/(-2.*lDO1*len));
    x = pO1.x+lDO1*cos(a); y = pO1.y+lDO1*sin(a);
    lp->point = pD = Pointd(x, y);
    lp->angle = GetAngle(x-pO1.x, y-pO1.y);
    // 4 - Точка С
    lp = &vVertex[4];
    a += M_PI_2;
    x = pO1.x+lCO1*cos(a); y = pO1.y+lCO1*sin(a);
    lp->point = pC = Pointd(x,y);
    lp->angle = a;
    // 5 - Точка B
    lp = &vVertex[5];
    a = ArcSin(pC.x/lCB);
    x = 0; y = pC.y-lCB*cos(a);
    lp->point = pB = Pointd(x,y);
    lp->angle = GetAngle(pB.x-pC.x, pB.y-pC.y);
}
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "unGeneral.h"

#include "unGraphics.h"
#include "unTotal.h"

#include "scheme.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tfm *fm;
#define BV_ADD_WEIGHT       125000
//---------------------------------------------------------------------------
TLeverParam		gParam;
double			gAngle, gAngleSpeed = M_PI/60., gScale, gLastAngle;
bool			bSize, bText, bAngle, bAuto, bTrace, bForce;
TFieldControl	*gField;
int             graphSelect     = 0;
//
DWORD			colBackground	= RGB(255, 255, 255);
DWORD			colText			= 0;
DWORD			colSize			= 0;
DWORD			colElement		= 0;
DWORD			colJoint		= 0;
DWORD			colUnit			= 0;
//---------------------------------------------------------------------------
__fastcall Tfm::Tfm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void Tfm::InitModel (void)
{
	int	i;
    TLabel	*lb;
    TEdit	*le;
    TUpDown	*lud;

	// Получение исходных данных
    GetParameters (gParam);
    // Подготовка интерфейса
    // Редакторы длин
    gField = new TFieldControl[gParam.countDistances];
    for (i=0; i<gParam.countDistances; i++)
    {
    	lb = gField[i].Label = new TLabel(gbParam);
		lb->Parent = gbParam;
        lb->Caption = gParam.Distances[i].Caption;
        lb->Top = 23 + i*22; lb->Left = 20;

        le = gField[i].Edit = new TEdit(gbParam);
        le->Parent = gbParam; le->ReadOnly = true;
        le->Top = 20 + i*22; le->Left = 72; le->Width = 40;

        lud = gField[i].UD = new TUpDown(gbParam);
        lud->Parent = gbParam;
        lud->Associate = le;
        lud->Min = gParam.Distances[i].min;
        lud->Max = gParam.Distances[i].max;
        lud->Position = gParam.Distances[i].current;
        lud->Tag = i; lud->Increment = 5;
        lud->OnClick = udChangeParam;
    }
    gbParam->Height = 25 + gParam.countDistances*22;


    // Установка начальных значений
    bAuto = true;
    cbScale->ItemIndex = 3;
    // Обновление надписей
    tbAngleSpeedChange(NULL);
    cbScaleChange(NULL);
}
//---------------------------------------------------------------------------
void Tfm::InitGraphics (void)
{
    TSpeedButton *lBtn;
    int     i;

    // Подготовка интерфейса
    // Кнопки графиков
    for (i=0; i<gParam.countGraphics; i++)
    {
        lBtn = new TSpeedButton(fmGraphics->pnButton);
        lBtn->GroupIndex = 1; lBtn->Tag = i;
        lBtn->Parent = fmGraphics->pnButton;
        lBtn->Left = 3; lBtn->Top = 6 + 22*i;
        lBtn->Width = 70; lBtn->Height = 22;
        lBtn->OnClick = fmGraphics->btGraphClick;
        lBtn->Caption = gParam.Graphics[i].Caption;
        if (i==0) lBtn->Down = true;
    }
    // Построение графиков
    UpdateAxis(gParam);
    RebuildGraphics();
}
//---------------------------------------------------------------------------
void Tfm::DrawBackground (void)
{
	int	i;
    TSchemeInfo	*lp;
    TPointd	p;

    for (i=0; i<gParam.countElements; i++)
    {
    	lp = &gParam.Elements[i];
		switch (lp->kind)
        {
        case ekMount:
			p = gParam.Vertexs[lp->point].point;
            DrawMount(image->Canvas, p, lp->angle, 50);
        break;
        case ekGuide:
        	p = gParam.Vertexs[lp->point].point;
            DrawGuide(image->Canvas, p, lp->angle, 4000);
        break;
        }
    }
}
//---------------------------------------------------------------------------
void Tfm::DrawLinks (void)
{
	int	i;
    TCanvas		*can = image->Canvas;
    TLeverInfo	lp;
    TPointd		p1, p2, p3;

    for (i=0; i<gParam.countLevers; i++)
    {
    	lp = gParam.Levers[i];

        switch (lp.kind)
        {
        case lkEdge:
        	p1 = gParam.Vertexs[lp.p1].point;
            p2 = gParam.Vertexs[lp.p2].point;
            can->MoveTo(p1.x, p1.y);
            can->LineTo(p2.x, p2.y);
        break;
        case lkRocker:
        	p1 = gParam.Vertexs[lp.p1].point;
            p2 = gParam.Vertexs[lp.p2].point;
            p3 = gParam.Vertexs[lp.p3].point;
            DrawRocker(can, p2, p1, p3, 10);
        break;
        case lkTriangle:
        	p1 = gParam.Vertexs[lp.p1].point;
            p2 = gParam.Vertexs[lp.p2].point;
            p3 = gParam.Vertexs[lp.p3].point;
            can->MoveTo(p1.x, p1.y);
            can->LineTo(p2.x, p2.y);
            can->LineTo(p3.x, p3.y);
            can->LineTo(p1.x, p1.y);
        break;
        case lkAxis:
        	p1 = gParam.Vertexs[lp.p1].point;
            p2 = gParam.Vertexs[lp.p2].point;
            DrawAxis(can, p1, p2, lp.value);
        break;
        }
    }
}
//---------------------------------------------------------------------------
void Tfm::DrawJoints (void)
{
	int	i;
    TCanvas		*can = image->Canvas;
    TVertexInfo	*lp;

    for (i=0; i<gParam.countVertexs; i++)
    {
		lp = &gParam.Vertexs[i];
        switch (lp->kind)
        {
        case jkTranslational:
            switch (lp->angleKind)
            {
            case akVertical:
                DrawTranslationalJoint(can, lp->point, 50, 30, M_PI_2);
            break;
            case akHorizontal:
            	DrawTranslationalJoint(can, lp->point, 50, 30, 0);
            break;
            case akFree:
            	DrawTranslationalJoint(can, lp->point, 50, 30,
                	gParam.Vertexs[lp->index].angle);
            break;
            }

        case jkRotational:
            DrawRotationalJoint(can, lp->point, 3);
        break;
        }
    }
}
//---------------------------------------------------------------------------
void Tfm::DrawSizes (void)
{
	int		i, len;
    TSchemeSize	lp;
    TCanvas		*can = image->Canvas;
    TPointd		p1, p2;

    for (i=0; i<gParam.countSizes; i++)
    {
    	lp = gParam.Sizes[i];
        p1 = gParam.Vertexs[lp.p1].point;
        p2 = gParam.Vertexs[lp.p2].point;
        len = gParam.Distances[lp.distance].current;

        switch (lp.kind)
        {
        case akHorizontal:
            DrawSize(can, p1, p2, len, 0, lp.displace, lp.up, IntToStr(len));
        break;
        case akVertical:
        	DrawSize(can, p1, p2, len, M_PI_2, lp.displace, lp.up, IntToStr(len));
        break;
        case akFree:
        	DrawSize(can, p1, p2, len,
            	GetAngle(p2.x-p1.x, p2.y-p1.y),
            	lp.displace, lp.up, IntToStr(len));
        break;
        }
    }
}
//---------------------------------------------------------------------------
void Tfm::DrawAngles (void)
{
	int		i;
    TSchemeAngle	lp;
    TCanvas	*can = image->Canvas;
    TPointd	p1, p2, p3;

    for (i=0; i<gParam.countAngles; i++)
    {
    	lp = gParam.Angles[i];

        if (lp.p2 == BV_OX)	p2 = Pointd(100,0);
			else if (lp.p2 == BV_OY) p2 = Pointd(0,100);
            else p2 = gParam.Vertexs[lp.p2].point;
        if (lp.p1 == BV_OX)	p1 = Pointd(100,0);
			else if (lp.p1 == BV_OY) p1 = Pointd(0,100);
            else
            {
            	p1 = gParam.Vertexs[lp.p1].point;
                p1.x -= p2.x; p1.y -= p2.y;
            }
        if (lp.p3 == BV_OX)	p3 = Pointd(100,0);
			else if (lp.p3 == BV_OY) p3 = Pointd(0,100);
            else
            {
            	p3 = gParam.Vertexs[lp.p3].point;
                p3.x -= p2.x; p3.y -= p2.y;
            }
        DrawAngle(can, p2, p1, p3, 30, FloatToStrF(
        	GetAngle(p1.x, p1.y, p3.x, p3.y)*180/M_PI,ffFixed, 3, 0));
    }
}
//---------------------------------------------------------------------------
void Tfm::DrawText (void)
{
	int		i;
    TVertexInfo	*lp;
    TCanvas		*can = image->Canvas;

    for (i=0; i<gParam.countVertexs; i++)
    {
    	lp = &gParam.Vertexs[i];
    	can->TextOutA(Round(lp->point.x+5), Round(lp->point.y+5), lp->Name);
    }
}
//---------------------------------------------------------------------------
void Tfm::DrawForce(void)
{
    TCanvas		*can = image->Canvas;
    int         j, i = (int) (gAngle*180/M_PI)%360, x, y, lX, lY;
    TGraphInfo  *lpInfo;
    TLeverInfo  *lpLever;
    TPointd     p, p0, p1, p2;
    double      a, fX, fY;

    if (i<0) i = 360+i;
    can->Pen->Width = 1;
    for (j=0; j<gParam.countGraphics; j++)
    {
        lpInfo = &gParam.Graphics[j];
        lpLever = &gParam.Levers[lpInfo->lever];

        switch (lpLever->kind)
        {
        case lkStone:
            //continue;
            p = gParam.Vertexs[lpInfo->point].point;
        break;

        case lkAxis:
        case lkEdge:
            if (lpInfo->pForceOut!=BV_IGNORED)
                p = gParam.Vertexs[lpInfo->pForceOut].point;
            else
                p = gParam.Vertexs[lpInfo->center].point;
        break;

        case lkTriangle:
            continue;
            //p = gParam.Vertexs[lpInfo->pForceOut].point;
        break;

        case lkRocker:
            p = gParam.Vertexs[lpInfo->pForceOut].point;
        break;
        }

        x = Round(p.x); y = Round(p.y);
        fX = lpInfo->data[i].forceX/5000.;
        fY = lpInfo->data[i].forceY/5000.;
        a = GetAngle(fX,fY);

        lX = Round(p.x+fX); lY = Round(p.y+fY);
        DrawArrow(can, Pointd(x,y), a, 25.);
        can->MoveTo(x, y); can->LineTo(lX, y);
        can->MoveTo(x, y); can->LineTo(x, lY);

        if (fX>0)
        {
            can->MoveTo(lX,y);
            can->LineTo(lX-5, y-2);
            can->LineTo(lX-5, y+4);
            can->LineTo(lX, y);
        }
        else
        {
            can->MoveTo(lX,y);
            can->LineTo(lX+5, y-2);
            can->LineTo(lX+5, y+4);
            can->LineTo(lX, y);
        }

        if (fY>0)
        {
            can->MoveTo(x, lY);
            can->LineTo(x-2, lY-5);
            can->LineTo(x+2, lY-5);
            can->LineTo(x, lY);
        }
        else
        {
            can->MoveTo(x,lY);
            can->LineTo(x-2, lY+5);
            can->LineTo(x+2, lY+5);
            can->LineTo(x, lY);
        }
    }
}
//---------------------------------------------------------------------------
void BuildGraphic (TGraphInfo &vInfo)
{
	int	i, j, n, angle, l, add, beginJ;
    TGraphKind	gk;
    TPointd		p[5], pCen, p0, p2, p3;
    TLeverInfo  *lpLever;
    TGraphData  *lpData;
    double		a[5], accel, aX, aY, fX, fY, speed, value, w2, w1, x, y,
                tmp, dX, dY, force, weight, aUnitX, aUnitY, sX, sY, aTmp;
    double      angleDir, angleEdge1, shoulder1, shoulder2, shoulder3, fTmp,
                force1, force2, fTempX, fTempY, aEdge, energy, kEnergy;

    vInfo.minValue = vInfo.minSpeed = vInfo.minAccel = vInfo.minForce = 1e+100;
    vInfo.maxValue = vInfo.maxSpeed = vInfo.maxAccel = vInfo.maxForce = -1e+100;
    if (gParam.dirRotation)
    {
        add = -1; beginJ = 4;
    }
    else
    {
        add = 1; beginJ = 0;
    }

    for (i=0; i<360; i++)
    {
    	for (l=0, j = beginJ; l<5; l++, j += add)
        {
        	CalcMechanic((i-2+j)*M_PI/180, gParam.Vertexs, gParam.Distances);
            p[l] = gParam.Vertexs[vInfo.point].point;
			switch (vInfo.kind)
            {
            case gkUnit:
				pCen = gParam.Vertexs[vInfo.center].point;
                p2 = gParam.Vertexs[vInfo.point].point;
            	a[l] = GetAngle(p2.x-pCen.x, p2.y-pCen.y);
            break;
            case gkPointY: a[l] = p[l].y / 1000.; break;
            case gkPointX: a[l] = p[l].x / 1000.; break;
            }
        }

        value = a[2];
        switch (vInfo.kind)
        {
        case gkUnit:
        	p3 = p[3];
            p2 = p[1];
            speed = (GetDifference(p2.x-pCen.x,p2.y-pCen.y,
            	p3.x-pCen.x,p3.y-pCen.y))/(2.*BV_RAD);

            p3 = p[4];
            p2 = p[3];
            w2 = (GetDifference(p2.x-pCen.x,p2.y-pCen.y,
            	p3.x-pCen.x,p3.y-pCen.y))/(2.*BV_RAD);
            dX = p3.x-p2.x; dY = p3.y-p2.y;
            sX = (p3.x+pCen.x)/2.-(p2.x+pCen.x)/2.;
            sY = (p3.y+pCen.y)/2.-(p2.y+pCen.y)/2.;

            p3 = p[1];
            p2 = p[0];
            w1 = (GetDifference(p2.x-pCen.x,p2.y-pCen.y,
            	p3.x-pCen.x,p3.y-pCen.y))/(2.*BV_RAD);
            dX = dX - (p3.x-p2.x); dY = dY - (p3.y-p2.y);
            aUnitX = sX - ((p3.x+pCen.x)/2.-(p2.x+pCen.x)/2.);
            aUnitY = sY - ((p3.y+pCen.y)/2.-(p2.y+pCen.y)/2.);

            accel = (fabs(w2)-fabs(w1))/(2.*BV_RAD);
            tmp = GetAngle(dX,dY); aX = accel*cos(tmp); aY = accel*sin(tmp);
        break;

        case gkPointX:
        	p2 = p[3]; p3 = p[1];
        	speed = (p2.x - p3.x)/(2000.*BV_RAD);

            p2 = p[4]; p3 = p[3];
        	w2 = (p2.x - p3.x)/(2.*BV_RAD);

            p2 = p[1]; p3 = p[0];
        	w1 = (p2.x - p3.x)/(2.*BV_RAD);

            aX = accel = (fabs(w2)-fabs(w1))/(2000.*BV_RAD);
            aY = 0;
        break;

        case gkPointY:
        	p2 = p[3]; p3 = p[1];
        	speed = (p2.y - p3.y)/(2000.*BV_RAD);

            p2 = p[4]; p3 = p[3];
        	w2 = (p2.y - p3.y)/(2.*BV_RAD);

            p2 = p[1]; p3 = p[0];
        	w1 = (p2.y - p3.y)/(2.*BV_RAD);

            aY = accel = (fabs(w2)-fabs(w1))/(2000.*BV_RAD);
            aX = 0;
        break;
        }

        if (vInfo.minSpeed>speed) vInfo.minSpeed = speed;
        if (vInfo.maxSpeed<speed) vInfo.maxSpeed = speed;

        if (vInfo.minAccel>accel) vInfo.minAccel = accel;
        if (vInfo.maxAccel<accel) vInfo.maxAccel = accel;

        {   // Вычисление силы
            fX = 0; fY = 0;
            CalcMechanic(i*M_PI/180, gParam.Vertexs, gParam.Distances);
            if (vInfo.graphIn1==BV_OUT)    // Выходное звено
            {
                switch (gParam.Levers[vInfo.lever].kind)
                {
                case lkStone:   // если выходное звено - камень
                    weight = BV_STONE_WEIGHT;
                    if (vInfo.kind == gkPointX)
                    {
                        if (gParam.doWork*speed>0)  // работа совершается
                        {
                            if (speed>=0) fX = BV_FORCE_WORK;
                                else fX = -BV_FORCE_WORK;
                        }
                        fX = fX-aX*BV_SPEED_2*weight;
                    }
                    else
                    {
                        if (gParam.doWork*speed>0)  // работа совершается
                        {
                            if (speed>=0) fY = BV_FORCE_WORK;
                                else fY = -BV_FORCE_WORK;
                        }
                        fY = fY-(aY*BV_SPEED_2-BV_G)*weight;
                    }
                    kEnergy = weight;
                break;

                case lkEdge:
                case lkRocker:
                    // Вес ребра
                    pCen = gParam.Vertexs[vInfo.center].point;
                    lpLever = &gParam.Levers[vInfo.lever];
                    p0 = gParam.Vertexs[lpLever->p1].point;
                    p2 = gParam.Vertexs[lpLever->p2].point;
                    p3 = gParam.Vertexs[lpLever->p3].point;
                    shoulder1 = GetDistance(p0.x-p2.x, p0.y-p2.y);
                    shoulder2 = GetDistance(p2.x-p3.x, p2.y-p3.y);
                    tmp = shoulder1 + shoulder2;
                    weight = BV_LEVER_WEIGHT*tmp/1000.;
                    kEnergy = BV_LEVER_WEIGHT*shoulder1*shoulder1/4000.
                        +BV_LEVER_WEIGHT*shoulder2*shoulder2/4000.;
                    // Внутрение силы
                    fX = -(aX+aUnitX)*weight*BV_SPEED_2;
                    fY = -((aY+aUnitY)*BV_SPEED_2-BV_G)*weight;

                    if (vInfo.pBase!=BV_IGNORED)
                    {   // если звено имеет точкy опоры
                        // входное воздействие
                        pCen = gParam.Vertexs[vInfo.pBase].point;
                        p2 = gParam.Vertexs[vInfo.point].point;
                        angleDir = GetAngle(p2.x-pCen.x, p2.y-pCen.y);
                        if (gParam.doWork*speed>0)
                        {
                            if (speed>=0) aTmp = angleDir-M_PI;
                                else aTmp = angleDir+M_PI;
                            fTmp = 500000*tmp/2.;
                        }
                        else
                        {
                            fTmp = 0;
                        }
                        // длина перпендикуляра
                        aEdge = GetDifference(angleDir-M_PI_2, aTmp);
                        // выходное воздействие
                        p2 = gParam.Vertexs[vInfo.pForceOut].point;
                        angleDir = GetAngle(p2.x-pCen.x, p2.y-pCen.y)-M_PI_2;
                        tmp = GetDistance(pCen.x-p2.x, pCen.y-p2.y);

                        fTempX = fTmp*cos(angleDir)/tmp;
                        fTempY = fTmp*sin(angleDir)/tmp;
                    }

                    fX = fTempX+fX/2.; fY = fTempY+fY/2.;
                break;

                case lkAxis:
                    // Вес ребра
                    pCen = gParam.Vertexs[vInfo.center].point;
                    tmp = gParam.Levers[vInfo.lever].value;
                    weight = BV_LEVER_WEIGHT*tmp/1000.;
                    // Внутрение силы
                    fX = -(aX+aUnitX)*weight*BV_SPEED_2;
                    fY = -((aY+aUnitY)*BV_SPEED_2-BV_G)*weight;

                    if (vInfo.pBase!=BV_IGNORED)
                    {   // если звено имеет точкy опоры
                        // входное воздействие
                        pCen = gParam.Vertexs[vInfo.pBase].point;
                        p2 = gParam.Vertexs[vInfo.point].point;
                        angleDir = GetAngle(p2.x-pCen.x, p2.y-pCen.y);
                        if (gParam.doWork*speed>0)
                        {
                            if (speed>=0) aTmp = angleDir-M_PI;
                                else aTmp = angleDir+M_PI;
                            fTmp = 500000*tmp/2.;
                        }
                        else
                        {
                            fTmp = 0;
                        }
                        // длина перпендикуляра
                        aEdge = GetDifference(angleDir-M_PI_2, aTmp);
                        // выходное воздействие
                        p2 = gParam.Vertexs[vInfo.pForceOut].point;
                        angleDir = GetAngle(p2.x-pCen.x, p2.y-pCen.y)-M_PI_2;
                        tmp = GetDistance(pCen.x-p2.x, pCen.y-p2.y);

                        fTempX = fTmp*cos(angleDir)/tmp;
                        fTempY = fTmp*sin(angleDir)/tmp;
                    }

                    fX = fTempX+fX/2.; fY = fTempY+fY/2.;
                    kEnergy = weight*tmp/12.;
                break;

                default:    // если выходное звено - другой тип
                    fX = 1; fY = 1;
                break;
                }
            }
            else    // промежуточное звено
            {
                switch (gParam.Levers[vInfo.lever].kind)
                {
                case lkStone:
                    kEnergy = weight = BV_STONE_WEIGHT;
                    lpData = &gParam.Graphics[vInfo.graphIn1].data[i];

                    fX = -(aX+aUnitX)*weight*BV_SPEED_2;
                    fY = -((aY+aUnitY)*BV_SPEED_2-BV_G)*weight;

                    switch (gParam.Vertexs[vInfo.point].angleKind)
                    {
                    case akFree:
                        fX = lpData->forceX+fX;
                        fY = lpData->forceY+fY;
                    break;

                    case akHorizontal:
                        fX = lpData->forceX+fX;
                        fY = 0;
                    break;

                    case akVertical:
                        fX = 0;
                        fY = lpData->forceY+fY;
                    break;
                    }
                break;

                case lkEdge:
                    // Вес ребра
                    pCen = gParam.Vertexs[vInfo.center].point;
                    p2 = gParam.Vertexs[vInfo.point].point;
                    tmp = GetDistance(pCen.x-p2.x, pCen.y-p2.y);
                    weight = BV_LEVER_WEIGHT*tmp/1000.;
                    kEnergy = weight*tmp/12.;
                    // Внутрение силы
                    fX = -(aX+aUnitX)*weight*BV_SPEED_2;
                    fY = -((aY+aUnitY)*BV_SPEED_2-BV_G)*weight;

                    if (vInfo.graphIn1!=BV_IGNORED)
                    {
                        // Внешняя сила
                        lpData = &gParam.Graphics[vInfo.graphIn1].data[i];
                        fTempX = lpData->forceX+fX/2.; fTempY = lpData->forceY+fY/2.;
                    }
                    else
                    {
                        // Только внутренняя
                        fTempX = fX/2.; fTempY = fY/2.;
                    }

                    if (vInfo.pBase!=BV_IGNORED)
                    {   // если звено имеет точкy опоры
                        // входное воздействие
                        pCen = gParam.Vertexs[vInfo.pBase].point;
                        angleDir = GetAngle(p2.x-pCen.x, p2.y-pCen.y);
                        tmp = GetDistance(pCen.x-p2.x, pCen.y-p2.y);
                        aTmp = GetAngle(fTempX, fTempY);
                        // длина перпендикуляра
                        aEdge = GetDifference(angleDir-M_PI_2, aTmp);
                        fTmp = GetDistance(fTempX, fTempY)*cos(aEdge)*tmp;
                        // выходное воздействие
                        p2 = gParam.Vertexs[vInfo.pForceOut].point;
                        angleDir = GetAngle(p2.x-pCen.x, p2.y-pCen.y)-M_PI_2;
                        tmp = GetDistance(pCen.x-p2.x, pCen.y-p2.y);

                        fTempX = fTmp*cos(angleDir)/tmp;
                        fTempY = fTmp*sin(angleDir)/tmp;
                    }

                    fX = fTempX; fY = fTempY;
                break;

                case lkRocker:
                    // Вес ребра
                    pCen = gParam.Vertexs[vInfo.pBase].point;
                    p2 = gParam.Vertexs[vInfo.pForceIn1].point;
                    p3 = gParam.Vertexs[vInfo.pForceOut].point;
                    shoulder1 = GetDistance(pCen.x-p2.x, pCen.y-p2.y);
                    shoulder2 = GetDistance(pCen.x-p3.x, pCen.y-p3.y);
                    weight = BV_LEVER_WEIGHT*(shoulder1+shoulder2)/1000.;
                    kEnergy = BV_LEVER_WEIGHT*shoulder1/4000.+
                        BV_LEVER_WEIGHT*shoulder2/4000.;
                    // Внутрение силы
                    fX = -(aX+aUnitX)*weight*BV_SPEED_2;
                    fY = -((aY+aUnitY)*BV_SPEED_2-BV_G)*weight;
                    // Внешняя сила
                    lpData = &gParam.Graphics[vInfo.graphIn1].data[i];
                    fTempX = lpData->forceX; fTempY = lpData->forceY;
                    // входное воздействие
                    angleDir = GetAngle(p2.x-pCen.x, p2.y-pCen.y);
                    aTmp = GetAngle(fTempX, fTempY);
                    // длина перпендикуляра
                    aEdge = GetDifference(angleDir-M_PI_2, aTmp);
                    fTmp = GetDistance(fTempX, fTempY)*cos(aEdge)*shoulder1;
                    // выходное воздействие
                    angleDir = GetAngle(p3.x-pCen.x, p3.y-pCen.y)-M_PI_2;

                    fX = ((fX/2.+fTmp)*cos(angleDir))/shoulder2;
                    fY = ((fY/2.+fTmp)*sin(angleDir))/shoulder2;
                break;

                case lkTriangle:
                    // Вес ребра
                    lpLever = &gParam.Levers[vInfo.lever];
                    p0 = gParam.Vertexs[lpLever->p1].point;
                    p2 = gParam.Vertexs[lpLever->p2].point;
                    p3 = gParam.Vertexs[lpLever->p3].point;
                    shoulder1 = GetDistance(p0.x-p2.x, p0.y-p2.y);
                    shoulder2 = GetDistance(p2.x-p3.x, p2.y-p3.y);
                    shoulder3 = GetDistance(p0.x-p3.x, p0.y-p3.y);
                    tmp = shoulder1 + shoulder2 + shoulder3;
                    weight = BV_LEVER_WEIGHT*tmp/1000.;
                    kEnergy = weight*tmp/36.;
                    // центр
                    x = (p0.x+p2.x+p3.x)/3.; y = (p0.y+p2.y+p3.y)/3.;
                    // внутрение силы
                    fX = -(aX+aUnitX)*weight*BV_SPEED_2;
                    fY = -((aY+aUnitY)*BV_SPEED_2-BV_G)*weight;

                    if (vInfo.pForceIn2!=BV_IGNORED)
                    {
                        // треугольник подвешен
                        // направляющая
                        pCen = Pointd(x,y);
                        p0 = gParam.Vertexs[vInfo.pForceIn2].point;
                        angleDir = GetAngle(x-p0.x, y-p0.y);

                        // внешняя сила 2
                        lpData = &gParam.Graphics[vInfo.graphIn2].data[i];
                        fTempX = lpData->forceX; fTempY = lpData->forceY;
                        // плечо силы 2
                        p2 = gParam.Vertexs[vInfo.pForceIn2].point;
                        aEdge = GetAngle(p2.x-pCen.x, p2.y-pCen.y);
                        aTmp = fabs(GetDifference(angleDir, aEdge));
                        shoulder1 = GetDistance(p2.x-pCen.x, p2.y-pCen.y)*cos(aTmp);
                        // входное воздействие 2
                        aTmp = GetAngle(fTempX, fTempY);
                        // длина перпендикуляра 2
                        aEdge = GetDifference(angleDir-M_PI_2, aTmp);
                        fTmp = GetDistance(fTempX, fTempY)*cos(aEdge)*shoulder1;

                        fX = fTempX+fX/3.; fY = fTempY+fY/3.;
                    }
                    else
                    {   // направляющая
                        pCen = gParam.Vertexs[vInfo.pBase].point; //Pointd(x,y);
                        angleDir = GetAngle(x-pCen.x, y-pCen.y);
                        fX = fX/2.; fY = fY/2.; fTmp = 0;
                    }

                    // внешняя сила 1
                    lpData = &gParam.Graphics[vInfo.graphIn1].data[i];
                    fTempX = lpData->forceX; fTempY = lpData->forceY;
                    // плечо силы 1
                    p2 = gParam.Vertexs[vInfo.pForceIn1].point;
                    aEdge = GetAngle(p2.x-pCen.x, p2.y-pCen.y);
                    aTmp = fabs(GetDifference(angleDir, aEdge));
                    shoulder1 = GetDistance(p2.x-pCen.x, p2.y-pCen.y)*cos(aTmp);
                    // входное воздействие 1
                    aTmp = GetAngle(fTempX, fTempY);
                    // длина перпендикуляра 1
                    aEdge = GetDifference(angleDir-M_PI_2, aTmp);
                    fTmp += GetDistance(fTempX, fTempY)*cos(aEdge)*shoulder1;

                    // плечо выходной силы
                    p2 = gParam.Vertexs[vInfo.pForceOut].point;
                    aEdge = GetAngle(p2.x-pCen.x, p2.y-pCen.y);
                    aTmp = fabs(GetDifference(angleDir, aEdge));
                    shoulder2 = GetDistance(p2.x-pCen.x, p2.y-pCen.y)*cos(aTmp);
                    fX = (fX+fTmp*cos(angleDir-M_PI_2))/shoulder2;
                    fY = (fY+fTmp*sin(angleDir-M_PI_2))/shoulder2;
                break;
                } // case lkTriangle
            } // else Lever == OUT
        } // code block
        energy = kEnergy*speed*speed*BV_SPEED_2/2.;

        // Temp
        force = sqrt(fX*fX+fY*fY)/1000.;
        if (vInfo.minForce>force) vInfo.minForce = force;
        if (vInfo.maxForce<force) vInfo.maxForce = force;

        if (vInfo.minValue>value) vInfo.minValue = value;
        if (vInfo.maxValue<value) vInfo.maxValue = value;

        vInfo.data[i].value = value;
        vInfo.data[i].speed = speed;
        vInfo.data[i].accel = accel;
		vInfo.data[i].accelX = aX;
        vInfo.data[i].accelY = aY;
        vInfo.data[i].forceX = fX;
        vInfo.data[i].forceY = fY;
        vInfo.data[i].energy = energy;
    }
}
//---------------------------------------------------------------------------
void Tfm::RebuildGraphics (void)
{
    int     i, j;
    double  moment = 0, work = 0, tmp, tmp2, fX, fY, energy = 0,
        transitive, transitive2;
    TGraphInfo  *lpInfo;
    TGraphData  *lpData;

    for (i=gParam.countGraphics-1; i>=0; i--)
	    BuildGraphic(gParam.Graphics[i]);


    gParam.TotalInfo.minMoment = gParam.TotalInfo.minInertia =
        gParam.TotalInfo.minWork = gParam.TotalInfo.minTransitive = gParam.TotalInfo.minTransitiveOpt = 1.0e+20;
    gParam.TotalInfo.maxMoment = gParam.TotalInfo.maxInertia =
        gParam.TotalInfo.maxWork = gParam.TotalInfo.maxTransitive = gParam.TotalInfo.maxTransitiveOpt = -1.0e+20;

    // Момент сопротивления
    lpInfo = &gParam.Graphics[0];
    for (i=0; i<360; i++)
    {
        lpData = &lpInfo->data[i];
        fX = lpData->forceX; fY = lpData->forceY;
        tmp = sqrt(fX*fX+fY*fY);
        tmp2 = BV_ANGLE_SPEED*tmp*BV_RAD;
        gParam.TotalInfo.data[i].Moment = tmp2;
        if (tmp2>gParam.TotalInfo.maxMoment)
            gParam.TotalInfo.maxMoment = tmp2;
        else if (tmp2<gParam.TotalInfo.minMoment)
            gParam.TotalInfo.minMoment = tmp2;
    }

    // Работа
    tmp = BV_RAD/2.;
    for (i=0; i<359; i++)
    {
        work += (gParam.TotalInfo.data[i].Moment
            +gParam.TotalInfo.data[i+1].Moment)*tmp;
        gParam.TotalInfo.data[i].Work = work;
    }
    work += (gParam.TotalInfo.data[359].Moment
        +gParam.TotalInfo.data[0].Moment)*tmp;
    gParam.TotalInfo.data[359].Work = work;

    gParam.TotalInfo.maxWork = work;
    gParam.TotalInfo.minWork = (gParam.TotalInfo.data[0].Moment
            +gParam.TotalInfo.data[1].Moment)*tmp;;

    // Средний момент двигателя
    gParam.TotalInfo.averageMoment = work/M_2PI;

    // Приведенный момент инерции
    tmp = 2/BV_SPEED_2;
    for (i=0; i<360; i++)
    {
        for (j=0, energy=0; j<gParam.countGraphics; j++)
            energy += gParam.Graphics[j].data[i].energy;
        tmp2 = tmp*energy;
        gParam.TotalInfo.data[i].Inertia = tmp2;

        if (tmp2>gParam.TotalInfo.maxInertia)
            gParam.TotalInfo.maxInertia = tmp2;
        else if (tmp2<gParam.TotalInfo.minInertia)
            gParam.TotalInfo.minInertia = tmp2;
    }

    // Момент инерции входного угла
    for (i=0; i<360; i++)
    {
        tmp2 = (gParam.TotalInfo.averageMoment-gParam.TotalInfo.data[i].Moment)
            /gParam.TotalInfo.data[i].Inertia;
        gParam.TotalInfo.data[i].Epsilon = tmp2;

        if (tmp2>gParam.TotalInfo.maxEpsilon)
            gParam.TotalInfo.maxEpsilon = tmp2;
        else if (tmp2<gParam.TotalInfo.minEpsilon)
            gParam.TotalInfo.minEpsilon = tmp2;
    }

    // Переходные процессы
    transitive = transitive2 = 225*M_2PI/30.;
    for (i=0; i<359; i++)
    {
        tmp = (gParam.TotalInfo.averageMoment-gParam.TotalInfo.data[i].Moment)
            /(gParam.TotalInfo.data[i].Inertia);
        tmp2 = (gParam.TotalInfo.averageMoment-gParam.TotalInfo.data[i+1].Moment)
            /(gParam.TotalInfo.data[i+1].Inertia);
        transitive += (tmp+tmp2)*BV_RAD/2.;
        gParam.TotalInfo.data[i].Transitive = transitive;

        if (transitive>gParam.TotalInfo.maxTransitive)
            gParam.TotalInfo.maxTransitive = transitive;
        if (transitive<gParam.TotalInfo.minTransitive)
            gParam.TotalInfo.minTransitive = transitive;

        tmp = (gParam.TotalInfo.averageMoment-gParam.TotalInfo.data[i].Moment)
            /(gParam.TotalInfo.data[i].Inertia+BV_ADD_WEIGHT);
        tmp2 = (gParam.TotalInfo.averageMoment-gParam.TotalInfo.data[i+1].Moment)
            /(gParam.TotalInfo.data[i+1].Inertia+BV_ADD_WEIGHT);
        transitive2 += (tmp+tmp2)*BV_RAD/2.;
        gParam.TotalInfo.data[i].TransitiveOpt = transitive2;

        if (transitive2>gParam.TotalInfo.maxTransitiveOpt)
            gParam.TotalInfo.maxTransitiveOpt = transitive2;
        if (transitive2<gParam.TotalInfo.minTransitiveOpt)
            gParam.TotalInfo.minTransitiveOpt = transitive2;
    }

    tmp = (gParam.TotalInfo.averageMoment-gParam.TotalInfo.data[359].Moment)
        /gParam.TotalInfo.data[359].Inertia;
    tmp2 = (gParam.TotalInfo.averageMoment-gParam.TotalInfo.data[0].Moment)
        /gParam.TotalInfo.data[0].Inertia;
    transitive += (tmp+tmp2)*BV_RAD/2.;
    gParam.TotalInfo.data[359].Transitive = transitive;

    if (transitive>gParam.TotalInfo.maxTransitive)
        gParam.TotalInfo.maxTransitive = transitive;
    if (transitive<gParam.TotalInfo.minTransitive)
        gParam.TotalInfo.minTransitive = transitive;

    tmp = (gParam.TotalInfo.averageMoment-gParam.TotalInfo.data[359].Moment)
        /(gParam.TotalInfo.data[359].Inertia+BV_ADD_WEIGHT);
    tmp2 = (gParam.TotalInfo.averageMoment-gParam.TotalInfo.data[0].Moment)
        /(gParam.TotalInfo.data[0].Inertia+BV_ADD_WEIGHT);
    transitive2 += (tmp+tmp2)*BV_RAD/2.;
    gParam.TotalInfo.data[359].TransitiveOpt = transitive2;

    if (transitive2>gParam.TotalInfo.maxTransitiveOpt)
        gParam.TotalInfo.maxTransitiveOpt = transitive2;
    if (transitive2<gParam.TotalInfo.minTransitiveOpt)
        gParam.TotalInfo.minTransitiveOpt = transitive2;

    if (fmGraphics->Visible || fm3!=NULL) RedrawGraph();
}
//---------------------------------------------------------------------------
void Tfm::RedrawGraph (void)
{
    TGraphInfo  *lpGraph;
    int         width = fmGraphics->Image->ClientWidth,
                height = fmGraphics->Image->ClientHeight;
	bTrace = false; // Удалять трассирующий луч не надо

    if (gParam.countGraphics==0) return;
	lpGraph = &gParam.Graphics[fmGraphics->Tag];

    DrawOneGraphic(Rect(0, 0, width/2, height/2), *lpGraph, gvValue);
    DrawOneGraphic(Rect(0, height/2, width/2, height), *lpGraph, gvForce);
    DrawOneGraphic(Rect(width/2, 0, width, height/2), *lpGraph, gvSpeed);
    DrawOneGraphic(Rect(width/2, height/2, width, height), *lpGraph, gvAccel);

    width = fm3->img->ClientWidth,
    height = fm3->img->ClientHeight;

    DrawTotal(Rect(0, 0, width/2, height/3), gvMoment);
    DrawTotal(Rect(0, height/3, width/2, 2*height/3), gvWork);
    DrawTotal(Rect(0, 2*height/3, width/2, height), gvInertia);

    DrawTotal(Rect(width/2, 0, width, height/3), gvEpsilon);
    DrawTotal(Rect(width/2, height/3, width, 2*height/3), gvTransitive);
    DrawTotal(Rect(width/2, 2*height/3, width, height), gvTransitiveOpt);
}
//---------------------------------------------------------------------------
void Tfm::UpdateGraph (void)
{
	TGraphInfo	*lpGraph;
    int         width = fmGraphics->Image->ClientWidth,
                height = fmGraphics->Image->ClientHeight;

	if (fmGraphics->Visible)
    {
        if (gParam.countGraphics==0) return;
    	lpGraph = &gParam.Graphics[fmGraphics->Tag];

    	if (bTrace)
        {
    		DrawGraphTrace(Rect(0, 0, width/2, height/2), *lpGraph, gvValue, gLastAngle);
            DrawGraphTrace(Rect(0, height/2, width/2, height), *lpGraph, gvForce, gLastAngle);
	    	DrawGraphTrace(Rect(width/2, 0, width, height/2), *lpGraph, gvSpeed, gLastAngle);
	    	DrawGraphTrace(Rect(width/2, height/2, width, height), *lpGraph, gvAccel, gLastAngle);
        }
    	DrawGraphTrace(Rect(0, 0, width/2, height/2), *lpGraph, gvValue, gAngle);
        DrawGraphTrace(Rect(0, height/2, width/2, height), *lpGraph, gvForce, gAngle);
    	DrawGraphTrace(Rect(width/2, 0, width, height/2), *lpGraph, gvSpeed, gAngle);
    	DrawGraphTrace(Rect(width/2, height/2, width, height), *lpGraph, gvAccel, gAngle);
        bTrace = true;	// Удалять трассирующий луч
        gLastAngle = gAngle;
    }
}
//---------------------------------------------------------------------------
void __fastcall Tfm::udChangeParam (TObject *Sender, TUDBtnType Button)
{
	int		i;
    TUpDown	*lp = (TUpDown*) Sender;

	i = lp->Tag;
    gParam.Distances[i].current = lp->Position;
    // Проверка корректности данных
    if (!image->Visible)
    {
    	try
        {
            UpdateAxis(gParam);
			RebuildGraphics();
            image->Visible = true;
        }
        catch (...)
        {
			return;
        }
    }
    // Попытка перестроить графики
    else if (bAuto)
    {
    	try
        {
            UpdateAxis(gParam);
            RebuildGraphics();
        }
        catch (...)
        {
            Timer->Enabled = false;
            cbAnimation->Checked = false;
            image->Visible = false;
            return;
        }
    }
    // Обновление сцены
    Refresh();
}
//---------------------------------------------------------------------------
void Tfm::DrawTotal (TRect vRect, TGraphView vView)
{
    TCanvas	*can = fm3->img->Canvas;
	int	//width = vRect.Width(), height = vRect.Height(),
        gTextHeight = can->TextHeight("0"), h2 = gTextHeight/2,
        gTextWidth = can->TextWidth("000.00"), w2 = gTextWidth/2,
        i, j;
    TRect g;
    double fX, fY, x, y, min, max;
    AnsiString  s;

    // Область графика
    g.Left = vRect.Left+gTextWidth+4;
    g.Right = vRect.Right - w2;
    g.Top = vRect.Top+gTextHeight+4;
    g.Bottom = vRect.Bottom - gTextHeight - 4;
	// Коэффициенты масштаба
    fX = g.Width()/359.;
    switch (vView)
    {
    case gvMoment: min = gParam.TotalInfo.minMoment; max = gParam.TotalInfo.maxMoment; break;
    case gvInertia: min = gParam.TotalInfo.minInertia; max = gParam.TotalInfo.maxInertia; break;
    case gvWork: min = gParam.TotalInfo.minWork; max = gParam.TotalInfo.maxWork; break;
    case gvTransitive: min = gParam.TotalInfo.minTransitive; max = gParam.TotalInfo.maxTransitive; break;
    case gvTransitiveOpt: min = gParam.TotalInfo.minTransitiveOpt-1; max = gParam.TotalInfo.maxTransitiveOpt+1; break;
    case gvEpsilon: min = gParam.TotalInfo.minEpsilon; max = gParam.TotalInfo.maxEpsilon; break;
    }
    if (max-min<1e-5) { max += 0.1; min -= 0.1; }
    fY = g.Height() / (double) (max-min);
    // Отчистка
    can->Brush->Color = TColor (colBackground);
    can->Brush->Style = bsSolid;
    can->FillRect(vRect);
    // Сетка
    can->Pen->Width = 1;
    for (i=0; i<11; i++)
    {
    	// Вертикальная
        x = g.Left+g.Width()*i/10;
        can->MoveTo(x, g.Top); can->LineTo(x, g.Bottom);
        can->TextOutA(x-w2, g.Bottom+2,
            FloatToStrF(M_2PI*i/10, ffFixed, 2, BV_DIGIT));

        if (vView==gvMoment || vView==gvWork)
        {
            // Горизонтальная
            y = g.Bottom-g.Height()*i/10;
            can->MoveTo(g.Left, y); can->LineTo(g.Right, y);
            can->TextOutA(vRect.Left+2, y+2-h2,
                FloatToStrF((max-min)*i/10+min, ffExponent, 2, 1));
        }
        else
        {
            // Горизонтальная
            y = g.Bottom-g.Height()*i/10;
            can->MoveTo(g.Left, y); can->LineTo(g.Right, y);
            can->TextOutA(vRect.Left+2, y+2-h2,
                FloatToStrF((max-min)*i/10+min, ffFixed, 5, 2));
        }
    }

    // Нуль
    can->Pen->Width = 2;
    // График
	switch (vView)
    {
    case gvMoment:	// значение
    	can->TextOutA(g.Left+2, vRect.Top+2, "График момента сопротивления");
	    can->MoveTo(g.Left, g.Bottom-(gParam.TotalInfo.data[0].Moment-min)*fY);
    	for (i=1; i<360; i++)
	    {
        	can->LineTo(g.Left+i*fX, g.Bottom-(gParam.TotalInfo.data[i].Moment-min)*fY);
        }
        y = g.Bottom-(gParam.TotalInfo.averageMoment-min)*fY;
        can->MoveTo(g.Left, y); can->LineTo(g.Right, y);
    break;
    case gvInertia: // скорость
    	can->TextOutA(g.Left+2, vRect.Top+2, "График приведенного момента инерции, кг*м^2");
	    can->MoveTo(g.Left, g.Bottom-(gParam.TotalInfo.data[0].Inertia-min)*fY);
    	for (i=1; i<360; i++)
	    {
        	can->LineTo(g.Left+i*fX, g.Bottom-(gParam.TotalInfo.data[i].Inertia-min)*fY);
        }
    break;
    case gvWork: // ускорение
    	can->TextOutA(g.Left+2, vRect.Top+2, "График A(fi), Дж");
	    can->MoveTo(g.Left, g.Bottom-(gParam.TotalInfo.data[0].Work-min)*fY);
    	for (i=1; i<360; i++)
	    {
        	can->LineTo(g.Left+i*fX, g.Bottom-(gParam.TotalInfo.data[i].Work-min)*fY);
        }
        can->MoveTo(g.Left, g.Bottom+min*fY); can->LineTo(g.Right, g.Bottom-max*fY);
    break;
    case gvEpsilon: // ускорение
    	can->TextOutA(g.Left+2, vRect.Top+2, "График ускорения, 1/с^2");
	    can->MoveTo(g.Left, g.Bottom-(gParam.TotalInfo.data[0].Epsilon-min)*fY);
    	for (i=1; i<360; i++)
	    {
        	can->LineTo(g.Left+i*fX, g.Bottom-(gParam.TotalInfo.data[i].Epsilon-min)*fY);
        }
    break;
    case gvTransitive: // силы
        can->TextOutA(g.Left+2, vRect.Top+2, "График W(fi), 1/с");
	    can->MoveTo(g.Left, g.Bottom-(gParam.TotalInfo.data[0].Transitive-min)*fY);
    	for (i=1; i<360; i++)
	    {
        	can->LineTo(g.Left+i*fX, g.Bottom-(gParam.TotalInfo.data[i].Transitive-min)*fY);
        }
    break;
    case gvTransitiveOpt: // силы
        double  ave = (gParam.TotalInfo.maxTransitiveOpt+gParam.TotalInfo.minTransitiveOpt)/2.,
            n = (gParam.TotalInfo.maxTransitiveOpt-gParam.TotalInfo.minTransitiveOpt)/ave;
        can->TextOutA(g.Left+2, vRect.Top+2, "График W(fi) с махавико, 1/с");
        s.printf("Показатель: %f    Доп. J: %i", n, BV_ADD_WEIGHT);
        can->TextOutA(g.Left+2, vRect.Top+20, s);
        can->MoveTo(g.Left, g.Bottom-(gParam.TotalInfo.data[0].TransitiveOpt-min)*fY);
    	for (i=1; i<360; i++)
	    {
        	can->LineTo(g.Left+i*fX, g.Bottom-(gParam.TotalInfo.data[i].TransitiveOpt-min)*fY);
        }
    break;
    }
}
//---------------------------------------------------------------------------
void Tfm::DrawOneGraphic (TRect vRect, TGraphInfo &vInfo, TGraphView vView)
{
	TCanvas	*can = fmGraphics->Image->Canvas;
	int	//width = vRect.Width(), height = vRect.Height(),
        gTextHeight = can->TextHeight("0"), h2 = gTextHeight/2,
        gTextWidth = can->TextWidth("000.00"), w2 = gTextWidth/2,
        i, j;
    TRect g;
    double fX, fY, x, y, min, max;

    // Область графика
    g.Left = vRect.Left+gTextWidth+4;
    g.Right = vRect.Right - w2;
    g.Top = vRect.Top+gTextHeight+4;
    g.Bottom = vRect.Bottom - gTextHeight - 4;
	// Коэффициенты масштаба
    fX = g.Width()/359.;
    switch (vView)
    {
    case gvValue: min = vInfo.minValue; max = vInfo.maxValue; break;
    case gvSpeed: min = vInfo.minSpeed; max = vInfo.maxSpeed; break;
    case gvAccel: min = vInfo.minAccel; max = vInfo.maxAccel; break;
    case gvForce: min = vInfo.minForce; max = vInfo.maxForce; break;
    }
    if (max-min<1e-5) { max += 0.1; min -= 0.1; }
    fY = g.Height() / (double) (max-min);
    // Отчистка
    can->Brush->Color = TColor (colBackground);
    can->Brush->Style = bsSolid;
    can->FillRect(vRect);
    // Сетка
    can->Pen->Width = 1;
    for (i=0; i<11; i++)
    {
    	// Вертикальная
        x = g.Left+g.Width()*i/10;
        can->MoveTo(x, g.Top); can->LineTo(x, g.Bottom);
        can->TextOutA(x-w2, g.Bottom+2, FloatToStrF(M_2PI*i/10,ffFixed,5,BV_DIGIT));
        // Горизонтальная
        y = g.Bottom-g.Height()*i/10;
        can->MoveTo(g.Left, y); can->LineTo(g.Right, y);
        can->TextOutA(vRect.Left+2, y+2-h2,
            FloatToStrF((max-min)*i/10+min,ffFixed,5,BV_DIGIT));
    }

    // Нуль
    can->Pen->Width = 2; y = g.Bottom+min*fY;
    if (min<=0 && 0<=max) { can->MoveTo(g.Left, y); can->LineTo(g.Right, y); }
    // График
	switch (vView)
    {
    case gvValue:	// значение
    	switch (vInfo.kind)
        {
        case gkUnit:
        	can->TextOutA(g.Left+2, vRect.Top+2, "График изменения угла"); break;
        case gkPointY:
        	can->TextOutA(g.Left+2, vRect.Top+2, "График изменения координаты Y (м)"); break;
        case gkPointX:
        	can->TextOutA(g.Left+2, vRect.Top+2, "График изменения координаты X (м)"); break;
        };

	    can->MoveTo(g.Left, g.Bottom-(vInfo.data[0].value-min)*fY);
    	for (i=1; i<360; i++)
	    {
        	can->LineTo(g.Left+i*fX, g.Bottom-(vInfo.data[i].value-min)*fY);
        }
    break;
    case gvSpeed: // скорость
    	can->TextOutA(g.Left+2, vRect.Top+2, "График изменения: приведенной скорости");
	    can->MoveTo(g.Left, g.Bottom-(vInfo.data[0].speed-min)*fY);
    	for (i=1; i<360; i++)
	    {
        	can->LineTo(g.Left+i*fX, g.Bottom-(vInfo.data[i].speed-min)*fY);
        }
    break;
    case gvAccel: // ускорение
    	can->TextOutA(g.Left+2, vRect.Top+2, "График изменения: приведенного ускорения");
	    can->MoveTo(g.Left, g.Bottom-(vInfo.data[0].accel-min)*fY);
    	for (i=1; i<360; i++)
	    {
        	can->LineTo(g.Left+i*fX, g.Bottom-(vInfo.data[i].accel-min)*fY);
        }
    break;
    case gvForce: // силы
        double  tmp;

    	can->TextOutA(g.Left+2, vRect.Top+2, "График изменения: компенсирующей силы (KН)");
        x = vInfo.data[0].forceX; y = vInfo.data[0].forceY;
        tmp = sqrt(x*x+y*y)/1000.;
	    can->MoveTo(g.Left, g.Bottom-(tmp-min)*fY);
    	for (i=1; i<360; i++)
	    {
            x = vInfo.data[i].forceX; y = vInfo.data[i].forceY;
            tmp = sqrt(x*x+y*y)/1000.;
        	can->LineTo(g.Left+i*fX, g.Bottom-(tmp-min)*fY);
        }
    break;
    }
}
//---------------------------------------------------------------------------
void Tfm::SetAutoCalc (bool vValue)
{
	bAuto = vValue;
}
//---------------------------------------------------------------------------
void Tfm::SetGraphSelect (int vGraph)
{
    graphSelect = vGraph;
}
//---------------------------------------------------------------------------
void Tfm::DrawGraphTrace(TRect vRect, TGraphInfo &vInfo, TGraphView vView,
    double vAngle)
{
	TCanvas	*can = fmGraphics->Image->Canvas;
	int	//width = vRect.Width(), // height = vRect.Height(),
        gTextHeight = can->TextHeight("0"),
        gTextWidth = can->TextWidth("000.00"), w2 = gTextWidth/2;
	int		i, j;
    TRect 	g;
    double 	fX, fY, x, y, max, min, tmp;

    // Область графика
    g.Left = vRect.Left+gTextWidth+4;
    g.Right = vRect.Right - w2;
    g.Top = vRect.Top+gTextHeight+4;
    g.Bottom = vRect.Bottom - gTextHeight - 4;
	// Коэффициенты масштаба
    fX = g.Width()/359.;
    switch (vView)
    {
    case gvValue: min = vInfo.minValue; max = vInfo.maxValue; break;
    case gvSpeed: min = vInfo.minSpeed; max = vInfo.maxSpeed; break;
    case gvAccel: min = vInfo.minAccel; max = vInfo.maxAccel; break;
    case gvForce: min = vInfo.minForce; max = vInfo.maxForce; break;
    }
    if (max-min<1e-5) { max += 0.1; min -= 0.1; }
    fY = g.Height() / (double) (max-min);
	// Трассирующий луч
    can->Pen->Width = 1; can->Pen->Style = psDash;
    can->Pen->Mode = pmNotXor;
    i = Round(vAngle / BV_RAD)%360; if (i<0) i=360+i;
	switch (vView)
    {
    case gvValue:	// значение
	    can->MoveTo(g.Left, g.Bottom-(vInfo.data[i].value-min)*fY);
        can->LineTo(g.Right, g.Bottom-(vInfo.data[i].value-min)*fY);
    break;

    case gvSpeed: // скорость
	    can->MoveTo(g.Left, g.Bottom-(vInfo.data[i].speed-min)*fY);
        can->LineTo(g.Right, g.Bottom-(vInfo.data[i].speed-min)*fY);
    break;

    case gvAccel: // ускорение
	    can->MoveTo(g.Left, g.Bottom-(vInfo.data[i].accel-min)*fY);
        can->LineTo(g.Right, g.Bottom-(vInfo.data[i].accel-min)*fY);
    break;

    case gvForce: // сила
        x = vInfo.data[i].forceX; y = vInfo.data[i].forceY;
        tmp = sqrt(x*x+y*y)/1000.;

	    can->MoveTo(g.Left, g.Bottom-(tmp-min)*fY);
        can->LineTo(g.Right, g.Bottom-(tmp-min)*fY);
    break;
    }
    can->MoveTo(g.Left+i*fX, g.Bottom); can->LineTo(g.Left+i*fX, g.Top);
    can->Pen->Mode = pmCopy; can->Pen->Style = psSolid;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall Tfm::FormCreate(TObject *Sender)
{
	InitModel();
}
//---------------------------------------------------------------------------
void __fastcall Tfm::FormDestroy(TObject *Sender)
{
	ReleaseParameters(gParam);
}
//---------------------------------------------------------------------------
void __fastcall Tfm::FormPaint(TObject *Sender)
{
	TCanvas	*lp = image->Canvas;
    int		halfHeight = lp->ClipRect.Height() / 2,
    		halfWidth = lp->ClipRect.Width() / 2,
            dX = sbX->Position,
            dY = sbY->Position,
            dF = (double) 1000.*gScale;
    // Проверка
    //if (!image->Visible) return;
	// Очистка
	lp->Brush->Color = (TColor) colBackground;
    lp->Brush->Style = bsSolid;
    lp->FillRect(image->ClientRect);

    // Подготовка
	SetMapMode(lp->Handle, MM_ANISOTROPIC);
	SetWindowExtEx(lp->Handle, 1000, 1000, NULL);
	SetViewportExtEx(lp->Handle, dF, -dF, NULL);
    SetViewportOrgEx(lp->Handle, (halfWidth-dX),
    	(halfHeight-dY), NULL);

    // Вычисление
    __try
    {
	    CalcMechanic(gAngle, gParam.Vertexs, gParam.Distances);

        // Прорисовка фоновых элементов
        lp->Pen->Color = (TColor) colElement;
        lp->Brush->Color = (TColor) colElement;
        DrawBackground();
        // Прорисовка звеньев
        lp->Pen->Color = (TColor) colUnit;
        lp->Brush->Color = (TColor) colBackground;
        DrawLinks();
        // Прорисовка соединений
        lp->Pen->Color = (TColor) colJoint;
        DrawJoints();
        // Прорисовка размеров
        lp->Pen->Color = (TColor) colSize;
        if (bSize) DrawSizes();
        // Прорисовка углов
        if (bAngle) DrawAngles();
        // Прорисовка сил
        if (bForce) DrawForce();
        // Прорисовка подписей
        lp->Pen->Color = (TColor) colText;
        if (bText) DrawText();

        // Обновление графиков
        UpdateGraph();
    }
    catch (...)
    {
    	cbAnimation->Checked = false;
    	Timer->Enabled = false;
        image->Visible = false;
        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall Tfm::TimerTimer(TObject *Sender)
{
    int     i;
	gAngle += gAngleSpeed;
    i = Round(gAngle*180./M_PI)%360;
    if (i<0) i = 360 + i;
    udAngle->Position = i;
	Refresh();
}
//---------------------------------------------------------------------------
void __fastcall Tfm::cbAnimationClick(TObject *Sender)
{
	Timer->Enabled = cbAnimation->Checked;
}
//---------------------------------------------------------------------------
void __fastcall Tfm::tbAngleSpeedChange(TObject *Sender)
{
	gAngleSpeed = tbAngleSpeed->Position*M_PI/180.;
    if (gParam.dirRotation) gAngleSpeed *= -1;
	lbAngleSpeed->Caption = IntToStr(tbAngleSpeed->Position);
}
//---------------------------------------------------------------------------
void __fastcall Tfm::udAngleClick(TObject *Sender, TUDBtnType Button)
{
	if (udAngle->Position==-1) udAngle->Position = 359;
    	else if (udAngle->Position==360) udAngle->Position = 0;
    gAngle = udAngle->Position*M_PI/180.;
    Refresh();
}
//---------------------------------------------------------------------------
void __fastcall Tfm::cbSizeClick(TObject *Sender)
{
	bSize = cbSize->Checked;
    Refresh();
}
//---------------------------------------------------------------------------
void __fastcall Tfm::cbAngleClick(TObject *Sender)
{
	bAngle = cbAngle->Checked;
    Refresh();
}
//---------------------------------------------------------------------------
void __fastcall Tfm::cbTextClick(TObject *Sender)
{
	bText = cbText->Checked;
    Refresh();
}
//---------------------------------------------------------------------------
void __fastcall Tfm::sbXChange(TObject *Sender)
{
	Refresh();
}
//---------------------------------------------------------------------------
void __fastcall Tfm::btGraphicsClick(TObject *Sender)
{
	fmGraphics->Show();
}
//---------------------------------------------------------------------------
void __fastcall Tfm::FormResize(TObject *Sender)
{
    image->Picture->Bitmap->Width = image->ClientWidth;
    image->Picture->Bitmap->Height = image->ClientHeight;
    Refresh();
}
//---------------------------------------------------------------------------
void __fastcall Tfm::cbScaleChange(TObject *Sender)
{
	gScale = StrToInt(cbScale->Text)/100.;
    Refresh();
}
//---------------------------------------------------------------------------

void __fastcall Tfm::cbForceClick(TObject *Sender)
{
    bForce = cbForce->Checked;
    Refresh();    
}
//---------------------------------------------------------------------------

void __fastcall Tfm::Button1Click(TObject *Sender)
{
    fm3->Show();    
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#ifndef kinematicsH
#define kinematicsH

#include <math.h>
#include <math.hpp>
//---------------------------------------------------------------------------
const double M_2PI = 2*M_PI;
const double BV_RAD = M_PI/180.;
//---------------------------------------------------------------------------
struct TPointd
{
	double x, y;
};
//---------------------------------------------------------------------------
TPointd __inline Pointd(double vX, double vY)
	{ TPointd p; p.x = vX; p.y = vY; return p; }
//---------------------------------------------------------------------------
double __inline GetAngle(double vX, double vY)
{
	double angle = Math::ArcTan2(vY,vX);
    if (angle<0) angle = M_2PI+angle;
    return angle;
}
//---------------------------------------------------------------------------
double __inline GetAngle(double vX1, double vY1, double vX2, double vY2)
{
	double angle = Math::ArcTan2(vY2,vX2)-Math::ArcTan2(vY1,vX1)+M_2PI;
    if (angle>=M_2PI) angle -= M_2PI;
    return angle;
}
//---------------------------------------------------------------------------
double __inline GetDifference(double vAngle1, double vAngle2)
{
	double	a1 = vAngle1, a2 = vAngle2, res;

	if (a1>=M_PI_2 && a2<=-M_PI_2) res = M_2PI+a2-a1;
    	else if (a1<=-M_PI_2 && a2>=M_PI_2) res = a2-M_2PI-a1;
    	else res = a2-a1;
    return res;
}
//---------------------------------------------------------------------------
double __inline GetDifference(double vX1, double vY1, double vX2, double vY2)
{
	double	a1 = Math::ArcTan2(vY1,vX1), a2 = Math::ArcTan2(vY2,vX2), res;
    
	if (a1>=M_PI_2 && a2<=-M_PI_2) res = M_2PI+a2-a1;
    	else if (a1<=-M_PI_2 && a2>=M_PI_2) res = a2-M_2PI-a1;
    	else res = a2-a1;
    return res;
}
//---------------------------------------------------------------------------
double __inline GetDistance(double vX, double vY)
{
	double distance = Power(vX*vX+vY*vY, 0.5);
    return distance;
}
//---------------------------------------------------------------------------
int __inline Round(double vX)
{
	double n, y = modf(vX, &n);
    if (y>=0.5) return int (vX+1);
    else if (y<=-0.5) return int (vX-1);
    else return int (vX);
}
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------

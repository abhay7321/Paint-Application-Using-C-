#include "polygon.h"

//For Class Polygon

Polyg::Polyg()
{
    flag=0;
    edges=3;
    length=0;
}



Polyg::Polyg(int s, int m)
{
        flag=0;
        edges=s;
	    length=m;
}



void Polyg::perimeter()
{
        peri=edges*length;
}

void Polyg::int_ext()
{
       interior=180*(edges-2)/edges;
       exterior=180-interior;
}


//For Class Rectang


Rectang::Rectang()
{

}

void Rectang::setp(int x1,int y1,int x2,int y2)
{
	dx1=x1;
	dy1=y1;
	dx2=x2;
	dy2=y2;
}

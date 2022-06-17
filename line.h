#include"point.h"
#ifndef LINE_H_INCLUDED
#define LINE_H_INCLUDED

class line:public Point
{
	protected:
		float x1,y1,x2,y2;
	public:
		line();
		void set_point(int sx,int sy,int ex,int ey);
		void drawLine(int x1,int y1,int x2,int y2);
};



#endif

#ifndef CURVE__H
#define CURVE__H
#include"shapes.h"
class circle:public shape
{
	protected:
		float cx,cy,px,py;  //cx,cy represents coordinates of center and px,py represents arbitrary points on that circle.
	public:
		circle();
		void setpt(float x,float y,float x1,float y1);
		void drawCircle(int x1, int y1, int x2, int y2);
};
#endif

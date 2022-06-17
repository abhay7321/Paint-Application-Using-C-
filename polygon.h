#include"shapes.h"
#include"line.h"
#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED

class Polyg : public shape
{
        int edges,length;
        float peri, interior, exterior;
    public:
        Polyg();
        Polyg(int s, int m);
        void perimeter();
        void int_ext();
        void draw_polygon(int x1,int y1,int x2,int y2);
};


class Rectang : public Polyg, public line
{
    protected:
        int dx1,dy1,dx2,dy2;
    public:
        Rectang();
        void setp(int x1,int y1,int x2,int y2);
        void drawRectangle(int x1,int y1,int x2,int y2);
};
#endif

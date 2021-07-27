#ifndef POINT__H
#define POINT__H

#include <iostream>
#include <string>
using namespace std;


extern float window_h;
extern bool isEraser;
extern float red , green , blue ;


class Point
{
private:
  int newX;
  int newY;
  float newR;
  float newG;
  float newB;
  int pt_size;


public:
  Point();
  Point(int x, int y, float r, float g, float b);
  ~Point();

  int getX();
  int getY();
  float getR();
  float getG();
  float getB();
  int getSize();
  void setPosition(int x, int y);
  void setColour(float r, float g, float b);


};

#endif

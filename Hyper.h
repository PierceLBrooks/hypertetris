// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// Hyper.h
// Last modified: April 17, 1996
//
// (C) 1996 Board of Trustees University of Illinois

#ifndef _HYPER_H_
#define _HYPER_H_

#include <GL/glut.h>
#include "FourD.h"
#include "Asserts.h"
#include "Board.h"

class Hyper
{
public:
  Hyper();
  Hyper(float low, float high, int *newpos);
  Hyper(float *low, float *high, int *newpos);
  ~Hyper();

  void Translate(int dir, int sign);

  int *GetPos();

  void Output();

  void SetColor(int newcolor);
  void DimColor();

  void Drawit(int axes);
  void Drawit2D(int axes);
  void Plot(int index, int axes);

  void Rotate(int *newpos, FourD *center, int dir, int dude, int sign);

private:
  FourD **pts;   // the 16 points that make up the hypercube
  int *boardpos; // the position of the cube in term of
                 // the board's "coordinates"
  int color;
};
#endif

// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// FourD.h
// Last modified: April 17, 1996
//
// (C) 1996 Board of Trustees University of Illinois

#ifndef _FOURD_H_
#define _FOURD_H_

#include "Asserts.h"
#include "Shared.h"
#include <GL/glut.h>

class FourD
{
  friend class Hyper;
public:
  FourD();
  FourD(float x, float y, float z, float w);
  FourD(float *newpts);
  ~FourD();

  void Output();

  void Translate(float xx, float yy, float zz, float ww);
  void Rotate(FourD *center, int dir, int dude, int sign);

  void Plot(int *axis, float *dist);

  float& operator[](int i);

private:
  float *pt;
};

#endif

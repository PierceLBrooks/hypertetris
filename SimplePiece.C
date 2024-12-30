// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// SimplePiece.C
// Last modified: April 22, 1996
//
// (C) 1996 Board of Trustees University of Illinois

#include "Pieces.h"

extern int *DIM;
extern float *LEN;
extern float *botcorner;

SimplePiece::SimplePiece() : GenPiece(1)
{
  centerstat = 0;

  int *dudeman = new int[4];
  for (int i = 0; i < 3; i++)
    dudeman[i] = (DIM[i] /2);
  dudeman[W] = DIM[W];

  float *high = new float[4];
  float *low = new float[4];

  for (int i = 0; i < 4; i++) {
    low[i] = botcorner[i] + dudeman[i] * LEN[i];
    high[i] = low[i] + LEN[i];
  }

  float *temp = new float[4];
  for (int i = 0; i < 4; i++)
    temp[i] = (low[i] + high[i]) / 2;

  center = new FourD(temp);
  delete temp;

  cubes[0] = new Hyper(low, high, dudeman);
  cubes[0]->SetColor(0x0066ff);

  delete dudeman;
  delete low;
  delete high;
}

// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// ThreeLinePiece.C
// Last modified: April 22, 1996

#include "Pieces.h"

extern int *DIM;
extern float *LEN;
extern float *botcorner;

ThreeLinePiece::ThreeLinePiece() : GenPiece(3)
{
  centerstat = 1;
  
  int *dudeman = new int[4];
  for (int i = 0; i < 3; i++)
    dudeman[i] = (DIM[i] / 2);
  dudeman[VW] = (DIM[VW] + 1);

  int **newpos = new int*[3];
  for (int i = 0; i < 3; i++)
    newpos[i] = new int[4];

  for (int i = 0; i < 4; i++) // cube 0
    newpos[0][i] = dudeman[i] + ((i == VW) ? 1 : 0);

  for (int i = 0; i < 4; i++) // cube 1
    newpos[1][i] = dudeman[i];

  for (int i = 0; i < 4; i++) // cube 2
    newpos[2][i] = dudeman[i] + ((i == VW) ? -1 : 0);

  float **low = new float*[3];
  float **high = new float*[3];
  for (int i = 0; i < 3; i++) {
    low[i] = new float[4];
    high[i] = new float[4];
  }

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 4; j++) {
      low[i][j] = botcorner[j] + newpos[i][j] * LEN[j];
      high[i][j] = low[i][j] + LEN[j];
    }

  float *temp = new float[4];
  for (int i = 0; i < 4; i++)
    temp[i] = (low[centerstat][i] + high[centerstat][i]) / 2;

  center = new FourD(temp);
  delete temp;

  for (int i = 0; i < 3; i++) {
    cubes[i] = new Hyper(low[i], high[i], newpos[i]);
    cubes[i]->SetColor(0xff6699);
  }

  delete dudeman;

  for (int i = 0; i < 3; i++) {
    delete newpos[i];
    delete low[i];
    delete high[i];
  }

  delete newpos;
  delete low;
  delete high;
}

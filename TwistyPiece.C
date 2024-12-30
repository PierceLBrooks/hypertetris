// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// TwistyPiece.C
// Last modified: April 17, 1996
//
// (C) 1996 Board of Trustees University of Illinois

#include "Pieces.h"

extern int *DIM;
extern float *LEN;
extern float *botcorner;

TwistyPiece::TwistyPiece() : GenPiece(4)
{
  centerstat = -1;
  
  int *dudeman = new int[4];
  for (int i = 0; i < 3; i++)
    dudeman[i] = (DIM[i] / 2);
  dudeman[W] = (DIM[W] + 1);

  int **newpos = new int*[4];
  for (i = 0; i < 4; i++)
    newpos[i] = new int[4];

  for (i = 0; i < 4; i++) // cube 0
    newpos[0][i] = dudeman[i] + ((i == Z) ? -1 : 0);
  
  for (i = 0; i < 4; i++) // cube 1
    newpos[1][i] = dudeman[i];

  for (i = 0; i < 4; i++) // cube 2
    newpos[2][i] = dudeman[i] + ((i == X) ? 1 : 0);

  for (i = 0; i < 4; i++) // cube 3
    newpos[3][i] = dudeman[i] + (((i == X) || (i == Y)) ? 1 : 0);
 
  /*
  for (i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      cout << "newpos["<<i<<"]["<<j<<"] = " << newpos[i][j] << endl;
*/

  float **low = new float*[4];
  float **high = new float*[4];
  for (i = 0; i < 4; i++) {
    low[i] = new float[4];
    high[i] = new float[4];
  }

  for (i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
      low[i][j] = botcorner[j] + newpos[i][j] * LEN[j];
      high[i][j] = low[i][j] + LEN[j];
    }
 
  float *temp = new float[4];
  for (i = 0; i < 4; i++)
    temp[i] = botcorner[i] + dudeman[i]*LEN[i]+(((i == Y)||(i == X))?LEN[i]:0);

  center = new FourD(temp);
  delete temp;

  for (i = 0; i < 4; i++) {
    cubes[i] = new Hyper(low[i], high[i], newpos[i]);
    cubes[i]->SetColor(0x666666);
  }

  delete dudeman;

  for (i = 0; i < 4; i++) {
    delete newpos[i];
    delete low[i];
    delete high[i];
  }

  delete newpos;
  delete low;
  delete high;
}

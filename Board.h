// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// Board.h
// Last modified: April 17, 1996
//
// (C) 1996 Board of Trustees University of Illinois

#ifndef _BOARD_H_
#define _BOARD_H_

#include "Shared.h"
#include "Hyper.h"
#include "DList.h"
#include "FourD.h"

#define FILLED (char)(1)
#define EMPTY  (char)(0)

class Board {

public:
  Board();  
  ~Board();

  int GoodSpotShort(int *pos);
  int GoodSpot(int *pos);
  void EmptySpot(int *pos);
  void FillSpot(int *pos);

  int DonePiece(Hyper **newcubes, int numcubes);
  void Drawit(int axes);

  int CheckBoard(); // checks for filled cubes
  void RemoveCube(int wcoord); // deletes the cube with the wcoord

private:
  char ****boardarray;
  DList<Hyper> *fallen;
  FourD **bottom;
  FourD **top;
};

#endif

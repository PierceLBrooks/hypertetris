// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// GenPiece.h
// Last modified: April 17, 1996
//
// (C) 1996 Board of Trustees University of Illinois

#ifndef _GENPIECE_H_
#define _GENPIECE_H_

#include "Board.h"
#include "Hyper.h"
#include "Shared.h"

class GenPiece
{
public:
  GenPiece(int cubenum);
  ~GenPiece();

  int **CanRotate(int dir, int dude, int sign);
  void Rotate(int dir, int dude, int sign);
  
  void Drop();
  int Translate(int dir, int sign);
  int DonePiece();

  void Drawit(int axes);

protected:
  Hyper **cubes;  // an array of the cubes in the piece
  FourD *center;  // center for rotation
  int centerstat; // status of center 
                  // <  0 => not in center of a cube
                  // >= 0 => in the center of cube[centerstat]

  // what about center of a cube that's not in the piece??

private:
  int numcubes;   // number of cubes in piece (is four for typical piece)
};

#endif

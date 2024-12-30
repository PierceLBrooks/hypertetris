// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// Pieces.h
// Last modified: April 22, 1996
//
// (C) 1996 Board of Trustees University of Illinois

#ifndef _PIECES_H_
#define _PIECES_H_

#include "GenPiece.h"

class CubePiece : public GenPiece {
public:
  CubePiece();
};

class LPiece : public GenPiece {
public:
  LPiece();
};

class SPiece : public GenPiece {
public:
  SPiece();
};

class StraightPiece : public GenPiece {
public:
  StraightPiece();
};

class TPiece : public GenPiece {
public:
  TPiece();
};

class CornerPiece : public GenPiece {
public:
  CornerPiece();
};

class TwistyPiece : public GenPiece {
public:
  TwistyPiece();
};

class ZPiece : public GenPiece {
public:
  ZPiece();
};

class ThreeLinePiece : public GenPiece {
public:
  ThreeLinePiece();
};

class TriPiece : public GenPiece {
public:
  TriPiece();
};

class TwoPiece : public GenPiece {
public:
  TwoPiece();
};

class SimplePiece : public GenPiece {
public:
  SimplePiece();
};

#endif

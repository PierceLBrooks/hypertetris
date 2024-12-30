// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// GenPiece.C
// Last modified: April 25, 1996
//
// (C) 1996 Board of Trustees University of Illinois

#include "GenPiece.h"
#include <math.h>
#include <iostream>

//#define DEBUG_GENPIECE

extern Board *board;
extern int *DIM;
extern float *LEN;
extern float *botcorner;

// Given:   The number of hypercubes in this piece
// Task:    Allocate the appropriate number of Hypers for this piece
// Return:  Nothing, but the Hypers have been allocated
GenPiece::GenPiece(int cubenum)
{
  numcubes = cubenum;
  cubes = new Hyper*[numcubes];
}


// Given:   Nothing
// Task:    Delete the memory used by this piece, but preserving the
//          parts that were passed on to the board
// Return:  Nothing, but the memory is cleared
GenPiece::~GenPiece()
{
  // don't delete the cubes, as they will become part of the board
  // after this is done
  delete center;
}


// Given:   The two axes for the plane of rotation, and the sign of rotation
// Task:    Determine if this piece can be rotated in the given plane and
//          and the given direction
// Return:  A pointer to the new board positions of each of the cubes if
//          the given rotation is possible, otherwise, a pointer to NULL
int **GenPiece::CanRotate(int dir, int dude, int sign)
{
#ifdef DEBUG_GENPIECE
  cout << "Begin GenPiece::CanRotate(" << dir << ", " << sign << ")" << endl;
#endif

  int **newpos = new int*[numcubes];
  for (int i = 0; i < numcubes; i++)
    newpos[i] = new int[4];
  
  int i = 0;
  int boolean = 1;
  int dircenter, dudecenter;
  
  if (centerstat < 0) { // center is messed up
    dircenter = (int)(floor((((*center)[dir] - botcorner[dir])/LEN[dir])+.5));
    dudecenter = (int)(floor((((*center)[dude] - botcorner[dude])/LEN[dude]) 
			      + .5));

  } else { // center is (0,0)
    dircenter = cubes[centerstat]->GetPos()[dir];
    dudecenter = cubes[centerstat]->GetPos()[dude];
  }

  while (boolean && (i < numcubes)) {
    if (centerstat < 0) {
      int dircoord = cubes[i]->GetPos()[dir];
      int dudecoord = cubes[i]->GetPos()[dude];

      int dirchange = ((dircoord >= dircenter) ? 1 : 0);
      int dudechange = ((dudecoord >= dudecenter) ? 1 : 0);

      dircoord += (-dircenter) + dirchange;
      dudecoord += (-dudecenter) + dudechange;

      int tempcoord = dircoord;
      dircoord  = dudecoord * sign;
      dudecoord = tempcoord * (-sign);

      dirchange = ((dircoord >= 0) ? 1 : 0);
      dudechange = ((dudecoord >= 0) ? 1 : 0);

      dircoord  += dircenter  - dirchange;
      dudecoord += dudecenter - dudechange;

      for (int j = 0; j < 4; j++)
	newpos[i][j] = ((j == dir) ? dircoord : ((j == dude) ? dudecoord :
						 cubes[i]->GetPos()[j]));

    } else {
      int dircoord = cubes[i]->GetPos()[dir] - dircenter;
      int dudecoord = cubes[i]->GetPos()[dude] - dudecenter;

      int tempcoord = dircoord;
      dircoord = dudecoord * sign;
      dudecoord = tempcoord * (-sign);

      dircoord += dircenter;
      dudecoord += dudecenter;

      for (int j = 0; j < 4; j++)
	newpos[i][j] = ((j == dir) ? dircoord : ((j == dude) ? dudecoord :
						  cubes[i]->GetPos()[j]));
    }
    
    boolean = board->GoodSpot(newpos[i]);
    
    i++;
  }

#ifdef DEBUG_GENPIECE
  cout << "End GenPiece::CanRotate(...)" << endl;
#endif

  if (boolean)
    return newpos;
  return NULL;
}


// Given:   Two axes of the plane of rotation, and a sign for the direction
// Task:    If possible, perform the given rotation
// Return:  Nothing, but if the piece could be rotated, then it's been rotated
void GenPiece::Rotate(int dir, int dude, int sign)
{
#ifdef DEBUG_GENPIECE
  cout << "Begin GenPiece::Rotate(" << dir << ", " << sign << ")" << endl;
#endif

  int **newpos = CanRotate(dir, dude, sign);

  if (newpos != NULL) {
    for (int i = 0; i < numcubes; i++)
      cubes[i]->Rotate(newpos[i], center, dir, dude, sign);

    for (int i = 0; i < numcubes; i++)
      delete newpos[i];
    delete newpos;
  }

#ifdef DEBUG_GENPIECE
  cout << "End GenPiece::Rotate(...)" << endl;
#endif
}


// Given:   A direction and sign for translation
// Task:    If possible, perform the given translation on this piece
// Return:  1 if the translation was performed, 0 if it was impossible
int GenPiece::Translate(int dir, int sign)
{
#ifdef DEBUG_GENPIECE
  cout << "Begin GenPiece::Translate(" << dir << "," << sign << ")" << endl;
#endif

  int edge = ((sign == 1) ? DIM[dir] : -1);
  
  int **newpos = new int*[numcubes];
  for (int i = 0; i < numcubes; i++)
    newpos[i] = new int[4];

  int i = 0;
  int boolean = 1;
  while (boolean && (i < numcubes))
    if ((cubes[i]->GetPos()[dir] + sign) == edge) 
      boolean = 0;
    else {
      for (int j = 0; j < 4; j++)
	newpos[i][j] = cubes[i]->GetPos()[j] + ((j == dir) ? sign : 0);

      boolean = board->GoodSpotShort(newpos[i]);
     
      i++;
    }

  if (boolean) {
    for (int i = 0; i < numcubes; i++)
      cubes[i]->Translate(dir, sign);

/*
    cout << endl;
    for (int i = 0; i < numcubes; i++)
      for (int j = 0; j < 4; j++)
	cout << "boardpos["<<i<<"]["<<j<<"] = "<<cubes[i]->GetPos()[j] << endl;
*/

    float xx = ((dir == X) ? (float)(sign)*LEN[X] : 0.0);
    float yy = ((dir == Y) ? (float)(sign)*LEN[Y] : 0.0);
    float zz = ((dir == Z) ? (float)(sign)*LEN[Z] : 0.0);
    float ww = ((dir == W) ? (float)(sign)*LEN[W] : 0.0);
    center->Translate(xx, yy, zz, ww);
    }
  
  for (int i = 0; i < numcubes; i++)
    delete newpos[i];
  delete newpos;

#ifdef DEBUG_GENPIECE
  cout << "End GenPiece::Translate(" << dir << "," << sign << ")" << endl;
#endif  

  return boolean;
}


// Given:   Nothing
// Task:    Drop this piece as far as possible in the negative W direction
// Return:  Nothing, but the piece has been dropped
void GenPiece::Drop() 
{
#ifdef DEBUG_GENPIECE
  cout << "Begin GenPiece::Drop()" << endl;
#endif

  while (Translate(W, -1)); 

#ifdef DEBUG_GENPIECE
  cout << "End GenPiece::Drop()" << endl;
#endif
}


// Given:   Nothing
// Task:    Let the board know that we're done with this piece, basically by
//          sending this pieces information to the board, so it can become
//          part of the board (resistance is futile.  you will be assimilated)
// Return:  Nothing, but the piece has been assimilated to the board
int GenPiece::DonePiece() 
{
#ifdef DEBUG_GENPIECE
  cout << "Begin GenPiece::DonePiece()" << endl;
#endif

  return (board->DonePiece(cubes, numcubes));

#ifdef DEBUG_GENPIECE
  cout << "End GenPiece::DonePiece()" << endl;
#endif
}


// Given:   Three axes for the projection
// Task:    Draw this piece in the given projection, plus draw its shadow on
//          the floor, if w is in the projection
// Return:  Nothing, but the piee has been drawn
void GenPiece::Drawit(int axes)
{
#ifdef DEBUG_GENPIECE
//  cout << "Begin GenPiece::Drawit()" << endl;
#endif

  for (int i = 0; i < numcubes; i++)
    cubes[i]->Drawit(axes);

  if (axes != 7)
    for (int i = 0; i < numcubes; i++)
      cubes[i]->Drawit2D(axes);

#ifdef DEBUG_GENPIECE
//  cout << "End GenPiece::Drawit()" << endl;
#endif
}




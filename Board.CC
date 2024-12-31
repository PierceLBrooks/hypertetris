// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// Board.C
// Last modified: May 8th, 2001 - Benjamin Bernard
//
// (C) 1996 Board of Trustees University of Illinois

#include "Board.h"

extern float *LEN;
extern int *DIM;
extern float *botcorner;

// Given:   Nothing
// Task:    Create a new board
// Return:  Nothing, but the new board has been created
Board::Board()
{
  boardarray = new char***[DIM[VX]];

  for (int i = 0; i < DIM[VX]; i++) {
    boardarray[i] = new char**[DIM[VY]];
    for (int j = 0; j < DIM[VY]; j++) {
      boardarray[i][j] = new char*[DIM[VZ]];
      for (int k = 0; k < DIM[VZ]; k++) {
	boardarray[i][j][k] = new char[DIM[VW]];
	for (int l = 0; l < DIM[VW]; l++)
	  boardarray[i][j][k][l] = EMPTY;
      }
    }
  }

  float *topcorner = new float[4];
  for (int i = 0; i < 4; i++)
    topcorner[i] = botcorner[i] + .01 + DIM[i] * LEN[i];

  float temp[16][4];
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 4; j++)
      temp[i][j] = ((i & (1<<(3-j))) ? (botcorner[j] - .01) : topcorner[j]);
  }

  delete topcorner;

  bottom = new FourD*[8];
  for (int i = 0; i < 8; i++)
    bottom[i] = new FourD(temp[(15 - 2 * i)]);


  top = new FourD*[8];
  for (int i = 0; i < 8; i++)
    top[i] = new FourD(temp[14 - 2*i]);

  fallen = new DList<Hyper>();
}


// Given:   Nothing
// Task:    Free all of the memory used by this board object
// Return:  Nothing, but the memory has been freed
Board::~Board()
{
  for (int i = 0; i < DIM[VX]; i++) {
    for (int j = 0; j < DIM[VY]; j++) {
      for (int k = 0; k < DIM[VZ]; k++)
	delete boardarray[i][j][k];
      delete boardarray[i][j];
    }
    delete boardarray[i];
  }
  delete boardarray;

  delete fallen;

  for (int i = 0; i < 8; i++) {
    delete bottom[i];
    delete top[i];
  }

  delete bottom;
  delete top;
}


// Given:   An integer pointer with a position
// Task:    Assume the position is within the board, and simply check
//          to see if it is FILLED
// Return:  0 if it is FILLED, 1 if it is not
int Board::GoodSpotShort(int *pos)
{
#ifdef DEBUG_BOARD
  cout << "Begin Board::GoodSpotShort(...)" << endl;
#endif

  if (pos[VW] >= DIM[VW])
    return 1;

  return ((boardarray[pos[0]][pos[1]][pos[2]][pos[3]] == FILLED) ? 0 : 1);
}


// Given:   An integer pointer with a position
// Task:    Deterime if the position is within the board, and if it
//          is, whether or not it is FILLED
// Return:  0 if off of board or FILLED, 1 if the position is EMPTY
int Board::GoodSpot(int *pos)
{
  if ((pos[VX] >= DIM[VX]) || (pos[VX] < 0) || (pos[VY] >= DIM[VY]) ||
      (pos[VY] < 0) || (pos[VZ] >= DIM[VZ]) || (pos[VZ] < 0) || (pos[VW] < 0))
    return 0;

  if (pos[VW] >= DIM[VW])
    return 1;
  
  return (boardarray[pos[0]][pos[1]][pos[2]][pos[3]] == EMPTY);
}


// Given:   An integer pointer with a position
// Task:    Make the given board position EMPTY
// Return:  Nothing (but the position is now EMPTY)
void Board::EmptySpot(int *pos)
{
  boardarray[pos[0]][pos[1]][pos[2]][pos[3]] = EMPTY;
}


// Given:   An integer pointer with a position
// Task:    Make the given board position FILLED
// Return:  Nothing (but the position in now FILLED)
void Board::FillSpot(int *pos)
{
  boardarray[pos[0]][pos[1]][pos[2]][pos[3]] = FILLED;
}


// Given:   A list of hypercubes, with the number that are in the list
// Task:    Add each hypercube to the fallen list, and dim its color
// Return:  0 if any of these hypercubes are off of the board, 1 if
//          all of them are on the board
int Board::DonePiece(Hyper **newcubes, int numcubes)
{
#ifdef DEBUG_BOARD
  cout << "Begin Board::DonePiece(..., "<<numcubes<<")" << endl;
#endif

  for (int i = 0; i < numcubes; i++) {
    if (newcubes[i]->GetPos()[VW] >= DIM[VW]) 
      return 0;
    FillSpot(newcubes[i]->GetPos());
    newcubes[i]->DimColor();
    fallen->InsertAfter(newcubes[i]);
  }
  
  return 1;

#ifdef DEBUG_BOARD
  cout << "End Board::DonePiece(..., "<<numcubes<<")" << endl;
#endif
}


// Given:   Nothing
// Task:    Check each of the cubes in the board to determine whether or
//          not the cube has been filled.  If filled, remove the hypercubes
//          within that cube.
// Return:  The number of cubes that were removed
int Board::CheckBoard()
{
  int numremoved = 0;

  for (int ww = DIM[VW] - 1; ww >= 0; ww--) {
    int boolean = 0;
    int xx = 0;
    while (boolean && (xx < DIM[VX])) {
      int yy = 0;
      while (boolean && (yy < DIM[VY])) {
	int zz = 0;
	while (boolean && (zz < DIM[VZ])) {
	  boolean = boardarray[xx][yy][zz][ww] == FILLED;
	  zz++;
	}
	yy++;
      }
      xx++;
    }

    if (boolean) {
      numremoved++;
      RemoveCube(ww);
    }
  }

  return numremoved;
}


// Given:   An integer axes which tell of the projection for the drawing
// Task:    Draw the outline of the board, and any fallen hypercubes
// Return:  Nothing, but the board has been drawn
void Board::Drawit(int axes)
{
  // draw the cubes that have fallen
  if (fallen->NotEmpty()) {
    fallen->Head();
    fallen->Get()->Drawit(axes);
    while (!fallen->EndOfList()) {
      (*fallen)++;
      fallen->Get()->Drawit(axes);
    }
  }

  // now draw the "background" which has the bottom, a line, and two sides
  if (axes != 7) {
    //int *axis = new int[3];
    int axis[3];

    switch (axes) {
    case 11:
      axis[0] = VW;
      axis[1] = VX;
      axis[2] = VY;
      break;
      
    case 13:
      axis[0] = VZ;
      axis[1] = VW;
      axis[2] = VX;
      break;
      
    case 14:
      axis[0] = VY;
      axis[1] = VZ;
      axis[2] = VW;
      break;
    }

    //float *dist = new float[3];
    float dist[3];

    for (int i = 0; i < 3; i++)
      dist[i] = 0.0;

    switch (axes) {
    case 11:
      cpack(0xff990000); // needs alpha set to ff to be shown, I guess
      bgnline();
      bottom[6]->Plot(axis, dist);
      top[6]->Plot(axis, dist);
      endline();
      break;

    case 13:
      cpack(0xff009900);
      bgnline();
      bottom[5]->Plot(axis, dist);
      top[5]->Plot(axis, dist);
      endline();
      break;

    case 14:
      cpack(0xff000099);
      bgnline();
      bottom[3]->Plot(axis, dist);
      top[3]->Plot(axis, dist);
      endline();
      break;
    }
    glEnable(GL_BLEND);
    blendfunction(BF_SA, BF_MSA);
    //    cpack(0x66ffffff);
    glColor4f(.6, .6, .6, (float)66/255);
    // cout << "alpha is: " << (float)66/255 << endl;
    // cout << "Number is: " << 0x66ffffff << endl;

    bgntmesh();
    for (int i = 4; i < 4; i++)
      bottom[i]->Plot(axis, dist);
    endtmesh();
    
    bgntmesh();
    for (int i = 4; i < 8; i++)
      bottom[i]->Plot(axis, dist);
    endtmesh();

    bgntmesh();
    for (int i = 0; i < 2; i++)
      bottom[i]->Plot(axis, dist);
    for (int i = 4; i < 6; i++)
      bottom[i]->Plot(axis, dist);
    endtmesh();
    
    bgntmesh();
    for (int i = 2; i < 4; i++)
      bottom[i]->Plot(axis, dist);
    for (int i = 6; i < 8; i++)
      bottom[i]->Plot(axis, dist);
    endtmesh();
    
    bgntmesh();
    for (int i = 0; i < 8; i += 2)
      bottom[i]->Plot(axis, dist);
    endtmesh();
    
    bgntmesh();
    for (int i = 1; i < 8; i += 2)
      bottom[i]->Plot(axis, dist);
    endtmesh();

    // negative == 99, positive == ff
    // x is red
    // y is green
    // z is blue

    switch (axes) {
    case 11: //wxy
      //      cpack(0x33000099);
      glColor4f((float)99/255, 0,0, (float)33/255);
      bgntmesh();
      bottom[0]->Plot(axis, dist);
      top[0]->Plot(axis, dist);
      bottom[2]->Plot(axis, dist);
      top[2]->Plot(axis, dist);
      endtmesh();

      //      cpack(0x33009900);
      glColor4f(0,(float)99/255,0, (float)33/255);
      bgntmesh();
      bottom[0]->Plot(axis, dist);
      top[0]->Plot(axis, dist);
      bottom[4]->Plot(axis, dist);
      top[4]->Plot(axis, dist);
      endtmesh();
      break;

    case 13: //zwx      
      //      cpack(0x33000099);
      glColor4f((float)99/255, 0,0, (float)33/255);
      bgntmesh();
      bottom[0]->Plot(axis, dist);
      top[0]->Plot(axis, dist);
      bottom[1]->Plot(axis, dist);
      top[1]->Plot(axis, dist);
      endtmesh();
      
      //      cpack(0x33990000);
      glColor4f(0,0,(float)99/255, (float)33/255);
      bgntmesh();
      bottom[0]->Plot(axis, dist);
      top[0]->Plot(axis, dist);
      bottom[4]->Plot(axis, dist);
      top[4]->Plot(axis, dist);
      endtmesh();
      break;

    case 14: //yzw
      //      cpack(0x33009900);
      glColor4f(0, (float)99/255,0, (float)33/255);
      bgntmesh();
      bottom[0]->Plot(axis, dist);
      top[0]->Plot(axis, dist);
      bottom[1]->Plot(axis, dist);
      top[1]->Plot(axis, dist);
      endtmesh();

      //      cpack(0x33990000);   
      glColor4f(0, 0,(float)99/255, (float)33/255);
      bgntmesh();
      bottom[0]->Plot(axis, dist);
      top[0]->Plot(axis, dist);
      bottom[2]->Plot(axis, dist);
      top[2]->Plot(axis, dist);
      endtmesh();
      break;
    }
    
    //    glDisable(GL_BLEND);
    //delete dist;
    //delete axis;
  } // if (axes != 7)
}


// Given:   A coordinate, in terms of board coordinates, in VW
// Task:    Remove all of the hypercubes which have this VW coordinate, and
//          crop all of the hypcubes above this VW coordinate down one
// Return:  Nothing, but the board has been taken care of
void Board::RemoveCube(int wcoord)
{
#ifdef DEBUG_BOARD
  cout << "Begin RemoveCube("<<wcoord<<")" << endl;
#endif
  
  fallen->Head();
  while (FindAtW(fallen, wcoord)) {
    Hyper *temp = fallen->Get();
    fallen->Remove();
    EmptySpot(temp->GetPos());
    delete temp;
  }

  int boolean = 1;
  fallen->Head();
  while (boolean && FindAboveW(fallen, wcoord)) {
    EmptySpot(fallen->Get()->GetPos());
    fallen->Get()->Translate(VW, -1);
    FillSpot(fallen->Get()->GetPos());
    if (fallen->EndOfList())
      boolean = 0;
    else
      (*fallen)++;
  }

#ifdef DEBUG_BOARD
  cout << "End RemoveCube("<<wcoord<<")" << endl;
#endif
}


// Given:   A List of hypercubes and a VW coordinate
// Task:    Find the next cube in the list above this VW coordinate, and
//          point the current pointer at it
// Return:  0 if there are no such cubes in the list, 1 if there is such
//          a cube in the list
int FindAboveW(DList<Hyper> *List, int wcoord)
{
  Node<Hyper> *Search = List->current;
  while (Search != NULL)
    if (Search->data->GetPos()[VW] > wcoord) {
      List->current = Search;
      return 1;
    } else
      Search = Search->next;

  return 0;
}


// Given:   A list of hypercubes and a VW coordinate
// Task:    Find the next cube in the list with this VW coordinate, and
//          point the current pointer at it
// Return:  0 if there are no such cubes in the list, 1 if there is such
//          a cube in the list
int FindAtW(DList<Hyper> *List, int wcoord)
{
  Node<Hyper> *Search = List->current;
  while (Search != NULL)
    if (Search->data->GetPos()[VW] == wcoord) {
      List->current = Search;
      return 1;
    } else
      Search = Search->next;

  return 0;
}

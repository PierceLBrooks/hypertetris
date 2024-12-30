// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// Hyper.C
// Last modified: April 24, 1996
//
// (C) 1996 Board of Trustees University of Illinois

#include <iostream> // for debugging only

//#define DEBUG_HYPER

#include "Hyper.h"
#include "DList.h"

DList<Hyper> junk;	// is this how to instantiate this template??

extern Board *board;
extern float *LEN;
extern int *DIM;
extern float *botcorner;
extern int SHADEUNDER;


// Given:   Nothing
// Task:    Just the default constructor
// Return:  Nothing
Hyper::Hyper()
{
  pts = new FourD*[16];
}

// Given:   A low, high, and new board position
// Task:    Construct a new hypercube whose real coordinates are based
//          off of high and low, and whose board position is newpos
// Return:  Nothing, but the hypercube has been constructed
Hyper::Hyper(float low, float high, int *newpos)
{
  pts = new FourD*[16];

  float temp[4];

  for (int ii = 0; ii < 16; ii++) {
    for (int jj = 0; jj < 4; jj++)
      temp[jj] = ((ii & (1<<(3-jj))) ? low : high);
    pts[ii] = new FourD(temp);
  }

  boardpos = new int[4];
  for (int ii = 0; ii < 4; ii++)  {
    if (ii != W)
      Assert(newpos[ii] < DIM[ii], "Piece doesn't fit on board");
    boardpos[ii] = newpos[ii];
  }
}


// Given:   A list of lows for each coordinate, a list of highs for each
//          coordinate, and a new board position
// Task:    Construct a new hypercube whose real coordinates are based
//          off of the highs and lows, and whose board position is newpos
// Return:  Nothing, but the hypercube has been constructed
Hyper::Hyper(float *low, float *high, int *newpos)
{
  pts = new FourD*[16];

  float temp[4];

  for (int ii = 0; ii < 16; ii++) {
    for (int jj = 0; jj < 4; jj++)
      temp[jj] = ((ii & (1<<(3-jj))) ? low[jj] : high[jj]);
    pts[ii] = new FourD(temp);
  }

  boardpos = new int[4];
  for (int ii = 0; ii < 4; ii++) {
    if (ii != W)
      Assert((newpos[ii] < DIM[ii]) && (newpos[ii] >= 0), 
	     "Piece doesn't fit on board");
    boardpos[ii] = newpos[ii];
  }
}


// Given:   Nothing
// Task:    Delete the memory used by this hypercube
// Return:  Nothing, but the memory has been cleared
Hyper::~Hyper()
{
  for (int i = 0; i < 16; i++)
    delete pts[i];
  
  delete pts;
  delete boardpos;
}


// Given:   A color
// Task:    Make the given color this color for this cube
// Return:  Nothing, but the color of the cube has been set
void Hyper::SetColor(int newcolor)
{
  color = newcolor;
}


// Given:   Nothing
// Task:    Dim the color of the cube, by halving each of the red, green
//          and blue components
// Return:  Nothing, but the color of the cube has been dimmed
void Hyper::DimColor()
{
  int temp = color;
  int rr = (temp & 0x000000ff) / 2;
  temp  = temp >> 8;
  int gg = (temp & 0x000000ff) / 2;
  temp  = temp >> 8;
  int bb = (temp & 0x000000ff) / 2;

  color = 0xff000000 + (bb << 16) + (gg << 8) + rr;
}


// Given:   Nothing
// Task:    Nothing
// Return:  The board position of this hypercube
int *Hyper::GetPos()
{
  return boardpos;
}


// This is for debugging only
void Hyper::Output()
{
#ifdef DEBUG_HYPER
  cout << "Begin Hyper Output" << endl;
#endif

  for (int i = 0; i < 16; i++)
    pts[i]->Output();

#ifdef DEBUG_HYPER
  cout << "End Hyper Output" << endl;
#endif
}


// Given:   A direction, and a sign
// Task:    Translate this hypercube one unit (in terms of board coordinate) in
//          the direction and sign designated
// Return:  Nothing, but the hypercube has been translated
void Hyper::Translate(int dir, int sign)
{
  float xx = ((dir == X) ? (float)(sign)*LEN[X] : 0.0);
  float yy = ((dir == Y) ? (float)(sign)*LEN[Y] : 0.0);
  float zz = ((dir == Z) ? (float)(sign)*LEN[Z] : 0.0);
  float ww = ((dir == W) ? (float)(sign)*LEN[W] : 0.0);

  for (int i = 0; i < 16; i++)
    pts[i]->Translate(xx, yy, zz, ww);

  boardpos[dir] += sign;
}


// Given:   The three axes to draw, in a bitmask
// Task:    Draw this hypercube in the three axes projection specified
// Return:  Nothing, but the cube has been drawn
void Hyper::Drawit(int axes)
{
#ifdef DEBUG_HYPER
//  cout << "Begin Hyper::Drawit()" << endl;
#endif

  if ((boardpos[W] < DIM[W]) || (axes == 7)) {
    // face 1, +x, +y
    bgntmesh(); Plot(0, axes); Plot(1, axes); 
    Plot(2, axes); Plot(3, axes); endtmesh();
    
    // face 2, +x, -y
    bgntmesh(); Plot(4, axes); Plot(5, axes); 
    Plot(6, axes); Plot(7, axes); endtmesh();
    
    // face 3, -x, +y
    bgntmesh(); Plot(8, axes); Plot(9, axes);
    Plot(10, axes); Plot(11, axes); endtmesh();
    
    // face 4, -x, -y
    bgntmesh(); Plot(12, axes); Plot(13, axes);
    Plot(14, axes); Plot(15, axes); endtmesh();
    
    // face 5, +x, +z
    bgntmesh(); Plot(0, axes); Plot(1, axes);
    Plot(4, axes); Plot(5, axes); endtmesh();
    
    // face 6, +x, -z
    bgntmesh(); Plot(2, axes); Plot(3, axes);
    Plot(6, axes); Plot(7, axes); endtmesh();
    
    // face 7, -x, +z
    bgntmesh(); Plot(8, axes); Plot(9, axes);
    Plot(12, axes); Plot(13, axes); endtmesh();
    
    // face 8, -x, -z
    bgntmesh(); Plot(10, axes); Plot(11, axes);
    Plot(14, axes); Plot(15, axes); endtmesh();
    
    // face 9, +x, +w
    bgntmesh(); Plot(0, axes); Plot(2, axes);
    Plot(4, axes); Plot(6, axes); endtmesh();
    
    // face 10, +x, -w
    bgntmesh(); Plot(1, axes); Plot(3, axes);
    Plot(5, axes); Plot(7, axes); endtmesh();
               
    // face 11, -x, +w
    bgntmesh(); Plot(8, axes); Plot(10, axes);
    Plot(12, axes); Plot(14, axes); endtmesh();
    
    // face 12, -x, -w
    bgntmesh(); Plot(9, axes); Plot(11, axes);
    Plot(13, axes); Plot(15, axes); endtmesh();

    // face 13, +y, +z
    bgntmesh(); Plot(0, axes); Plot(1, axes);
    Plot(8, axes); Plot(9, axes); endtmesh();
    
    // face 14, +y, -z
    bgntmesh(); Plot(2, axes); Plot(3, axes);
    Plot(10, axes); Plot(11, axes); endtmesh();
    
    // face 15, -y, +z
    bgntmesh(); Plot(4, axes); Plot(5, axes);
    Plot(12, axes); Plot(13, axes); endtmesh();
    
    // face 16, -y, -z
    bgntmesh(); Plot(6, axes); Plot(7, axes);
    Plot(14, axes); Plot(15, axes); endtmesh();
    
    // face 17, +y, +w
    bgntmesh(); Plot(0, axes); Plot(2, axes);
    Plot(8, axes); Plot(10, axes); endtmesh();
    
    // face 18, +y, -w
    bgntmesh(); Plot(1, axes); Plot(3, axes);
    Plot(9, axes); Plot(11, axes); endtmesh();
    
    // face 19, -y, +w
    bgntmesh(); Plot(4, axes); Plot(6, axes); 
    Plot(12, axes); Plot(14, axes); endtmesh();
    
    // face 20, -y, -w
    bgntmesh(); Plot(5, axes); Plot(7, axes);
    Plot(13, axes); Plot(15, axes); endtmesh();
    
    // face 21, +z, +w
    bgntmesh(); Plot(0, axes); Plot(4, axes); 
    Plot(8, axes); Plot(12, axes); endtmesh();
    
    // face 22, +z, -w
    bgntmesh(); Plot(1, axes); Plot(5, axes); 
    Plot(9, axes); Plot(13, axes); endtmesh();
    
    // face 23, -z, +w
    bgntmesh(); Plot(2, axes); Plot(6, axes); 
    Plot(10, axes); Plot(14, axes); endtmesh();
    
    // face 24, -z, -w
    bgntmesh(); Plot(3, axes); Plot(7, axes); 
    Plot(11, axes); Plot(15, axes); endtmesh();
  }
#ifdef DEBUG_HYPER
//  cout << "End Hyper::Drawit()" << endl;
#endif
}


// Given:   The index number of a point, and the three axes for projection
// Task:    Plot pt[index] in the given projection, using my cheesy shading
//          scheme for the point
// Return:  Nothing, but the point has been plotted
void Hyper::Plot(int index, int axes)
{
  int temp = color;
  
  int rr = temp & 0x000000ff;
  temp  = temp >> 8;
  int gg = temp & 0x000000ff;
  temp  = temp >> 8;
  int bb = temp & 0x000000ff;

  int ans = index%2;
  ans += ((index+1)%4) ? 0 : 1;
  ans += ((index+1)%8) ? 0 : 1;
  ans += ((index+1)%16) ? 0 : 1;
  
  rr -= ans*rr/5;
  gg -= ans*gg/5;
  bb -= ans*bb/5;

  cpack((0xff000000 + (bb << 16) + (gg << 8) + rr));

  //int *axis, notused;
  //axis = new int[3];
  int axis[3], notused;

  switch (axes) {
  case 7:
    axis[0] = X;
    axis[1] = Y;
    axis[2] = Z;
    notused = W;
    break;

  case 11:
    axis[0] = W;
    axis[1] = X;
    axis[2] = Y;
    notused = Z;
    break;
    
  case 13:
    axis[0] = Z;
    axis[1] = W;
    axis[2] = X;
    notused = Y;
    break;

  case 14:
    axis[0] = Y;
    axis[1] = Z;
    axis[2] = W;
    notused = X;
    break;
  }

  //float *dist = new float[3];
  float dist[3];

  for (int i = 0; i < 3; i++) {
    dist[i] = 1.0;
    if ((*pts[0])[axis[i]] > (*pts[15])[axis[i]]) {
      if ((*pts[index])[axis[i]] != (*pts[0])[axis[i]])
	dist[i] = -1.0;
    } else
      if ((*pts[index])[axis[i]] != (*pts[15])[axis[i]])
	dist[i] = -1.0;
    dist[i] *= (float)(DIM[notused] - boardpos[notused]);
  }

  pts[index]->Plot(axis, dist);

  //delete axis;
  //delete dist;
}


// Given:   The three axes for projection
// Task:    Draw the shadow of this cube on the floor of the playing surface
// Return:  Nothing, but the shadow has been drawn
void Hyper::Drawit2D(int axes)
{
  float wcoord = botcorner[W] - .005;
  float wcoord2 = botcorner[W] - .02;
  float mypoints[8][3];
  int i;

  switch (axes) {
  case 11: // wxy
    for (int i = 0; i < 4; i++)
      mypoints[i][0] = wcoord;
    for (int i = 4; i < 8; i++)
      mypoints[i][0] = wcoord2;

    for (int i = 0; i < 4; i++)
      mypoints[2*i][1] = (*pts[0])[X];
    for (int i = 0; i < 4; i++)
      mypoints[2*i+1][1] = (*pts[15])[X];

    //mypoints[0][1] = mypoints[2][1] = (*pts[0])[X];
    //mypoints[1][1] = mypoints[3][1] = (*pts[15])[X];
    mypoints[0][2] = mypoints[1][2] = mypoints[4][2] = mypoints[5][2] = 
      (*pts[0])[Y];
    mypoints[2][2] = mypoints[3][2] = mypoints[6][2] = mypoints[7][2] = 
      (*pts[15])[Y];
    break;
  
  case 13: // zwx
    for (int i = 0; i < 4; i++)
      mypoints[i][1] = wcoord;
    for (int i = 4; i < 8; i++)
      mypoints[i][1] = wcoord2;

    for (int i = 0; i < 4; i++)
      mypoints[2*i][0] = (*pts[0])[Z];
    for (int i = 0; i < 4; i++)
      mypoints[2*i+1][0] = (*pts[15])[Z];
    
    //mypoints[0][0] = mypoints[2][0] = (*pts[0])[Z];
    //mypoints[1][0] = mypoints[3][0] = (*pts[15])[Z];
    mypoints[0][2] = mypoints[1][2] = mypoints[4][2] = mypoints[5][2] =
      (*pts[0])[X];
    mypoints[2][2] = mypoints[3][2] = mypoints[6][2] = mypoints[7][2] =
      (*pts[15])[X];
    break;
  
  case 14: // yzw
    for (int i = 0; i < 4; i++)
      mypoints[i][2] = wcoord;
    for (int i = 4; i < 8; i++)
      mypoints[i][2] = wcoord2;

    for (int i = 0; i < 4; i++)
      mypoints[2*i][0] = (*pts[0])[Y];
    for (int i = 0; i < 4; i++)
      mypoints[2*i+1][0] = (*pts[15])[Y];

    //mypoints[0][0] = mypoints[2][0] = (*pts[0])[Y];
    //mypoints[1][0] = mypoints[3][0] = (*pts[15])[Y];
    mypoints[0][1] = mypoints[1][1] = mypoints[4][1] = mypoints[5][1] = 
      (*pts[0])[Z];
    mypoints[2][1] = mypoints[3][1] = mypoints[6][1] = mypoints[7][1] = 
      (*pts[15])[Z];
    break;
  }

  //cpack(0xff000000);
  // glEnable(GL_BLEND);
  //  blendfunction(BF_SA, BF_MSA);
  //  glColor4f(.2,.2,.2,.1);
  glColor4f(1,1,1,.1);
  bgntmesh();
  for (int i = 0; i < 4; i++)
    v3f(mypoints[i]);
  endtmesh();

  if (SHADEUNDER) {
    bgntmesh();
    for (int i = 4; i < 8; i++)
      v3f(mypoints[i]);
    endtmesh();
  }

  //  glDisable(GL_BLEND);

}


// Given:   A new board position, a center for rotation, and the two
//          axes and the sign for the rotation
// Task:    Set the board position to the one which was passed in, and
//          change the points of the cube by rotating them about the
//          given center in the given axes in the given direction
// Return:  Nothing, but the rotation has been performed
void Hyper::Rotate(int *newpos, FourD *center, int dir, int dude, int sign)
{
#ifdef DEBUG_HYPER
  cout << "Begin Hyper::Rotate(newpos, center, "<<dir<<", "<<sign<<")"<<endl;
#endif

  for (int i = 0; i < 16; i++)
    pts[i]->Rotate(center, dir, dude, sign);

  for (int i = 0; i < 4; i++)
    boardpos[i] = newpos[i];

#ifdef DEBUG_HYPER
  cout << "End Hyper::Rotate(...)" << endl;
#endif
}

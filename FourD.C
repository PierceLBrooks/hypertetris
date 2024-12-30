// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// FourD.C
// Last modified: April 24, 1996
//
// (C) 1996 Board of Trustees University of Illinois

#include <iostream> // for debugging purposes only

#include "FourD.h"


// Given:   Nothing
// Task:    Default constructor
// Return:  Nothing
FourD::FourD()
{
  pt = new float[4];
}
 

// Given:   A new x, y, z, and w coordinate
// Task:    Create a point with these coordinates
// Return:  Nothing, but the new point has been created
FourD::FourD(float x, float y, float z, float w)
{
  pt = new float[4]; pt[X] = x; pt[Y] = y; pt[Z] = z; pt[W] = w;
}


// Given:   A pointer to new x, y, z, and w coordinates
// Task:    Create a point with these coordinates
// Return:  Nothing, but the new point has been created
FourD::FourD(float *newpt)
{
  pt = new float[4];
  for (int i = 0; i < 4; i++)
    pt[i] = newpt[i];

  Assert(pt != NULL, "Ran out of memory!!!");
}


// Given:   Nothing
// Task:    Delete the memory used by this point
// Return:  Nothing, but the memory has been freed
FourD::~FourD()
{
  delete pt;
}


// for debugging purposes only
void FourD::Output()
{
#ifdef DEBUG_FOURD
  cout << "Begin FourD::Output()" << endl;
#endif

  for (int i =0; i < 4; i++)
    std::cout << pt[i] << " ";
  std::cout << std::endl;

#ifdef DEBUG_FOURD
  std::cout << "End FourD::Output()" << endl;
#endif
}


// Given:   Three axes for projection and a distancing factor
// Task:    Plot this point in the given projection using the given factor
// Return:  Nothing, but the point has been plotted
void FourD::Plot(int *axis, float *dist)
{
#ifdef DEBUG_FOURD
  cout << "Begin FourD::Plot(..., ...)" << endl;
#endif

  //float *temp = new float[3];
  float temp[3];

  for (int i = 0; i < 3; i++)
    temp[i] = pt[axis[i]] - .001 * dist[i];

  v3f(temp);
  //delete temp;
}


// Given:   A change for each of the coordinates for this point
// Task:    Increment each of the corrdinates by the given increments
// Return:  Nothing, but the point has been translated
void FourD::Translate(float xx, float yy, float zz, float ww)
{
  pt[X] += xx;  pt[Y] += yy;  pt[Z] += zz;  pt[W] += ww;
}


// Given:   A center for rotation, the two axes for the plane of rotation,
//          a the sign of the rotation
// Task:    Rotate this point around the given center, in the given plane,
//          in the given direction
// Return:  Nothing, but the point has been rotated
void FourD::Rotate(FourD *center, int dir, int dude, int sign)
{
  float temp = pt[dir];

  pt[dir] = ((pt[dude] - (*center)[dude]) * (float)(sign)) + (*center)[dir];
  pt[dude] = ((temp - (*center)[dir]) * -(float)(sign)) + (*center)[dude];
}


// Given:   An integer i
// Task:    This is the subscript operator for the FourD point class
// Return:  The value of the i coordinate
float& FourD::operator[](int i)
{
  Assert(((i < 4) && (i > -1)), "FourD array index out of range");

  return pt[i];
}

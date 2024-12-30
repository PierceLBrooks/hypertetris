// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// Rotor.h
// Last modified: April 25, 1996
//
// (C) 1996 Board of Trustees University of Illinois

#include <device.h>
#include <GL/glut.h>
#include "Shared.h"

class Rotor {
public:
  Rotor();
  ~Rotor();
  void CurrentMousePosition(int x, int y);
  void Rotate();
  void Activate();
  void Deactivate();
  void MouseLeftDown(); // returns state of other mouse button
  int MouseLeftUp();
  void MouseRightDown();
  int MouseRightUp(); //returns state of other mouse button
  void ShiftDown();
  void ShiftUp();

private:
  int isMouseLeftDown, isMouseRightDown, isShiftDown;
  int active;
  int mouseX, mouseY;

  float *aff;
};

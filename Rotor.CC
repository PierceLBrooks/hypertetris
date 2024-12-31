// Pelsmajer 13jan04
// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// Rotor.C
// Last modified: April 25, 1996
//
// (C) 1996 Board of Trustees University of Illinois

#define DEADZONE 40

#include <iostream>
#include "Rotor.h"
#define ABS(x) ((x>0)?x:-x)
#define IFSHIFT  if(getbutton(LEFTSHIFTKEY)||getbutton(RIGHTSHIFTKEY))
#define PRESS(K,A,b)   if(getbutton(K)){IFSHIFT{A;}else{b;}}

/* static float **id  = {{1.,0.,0.,0.},
		     {0.,1.,0.,0.},
		     {0.,0.,1.,0.},
		     {0.,0.,0.,1.}};
 Matrix aff = {{1.,0.,0.,0.},
		      {0.,1.,0.,0.},
		      {0.,0.,1.,0.},
		      {0.,0.,0.,1.}};
*/

Rotor::Rotor()
{
  active = 1;
  mouseX = mouseY = 0;
  isMouseLeftDown = isMouseRightDown =  isShiftDown =0;

  aff = new float[16];

  for(int i=0; i<16;i++)
    {
      if((i%5)==0)
	aff[i]=1;
      else
	aff[i]=0;
    }
}

Rotor::~Rotor()
{
  delete aff;
}


void Rotor::Rotate()
{
  float dx = (active ? mouseX : 0.0);
  float dy = (active ? mouseY : 0.0);
  if (active) {
#ifndef _WIN32
    int spd = 19;
#else
    int spd = 19;
#endif
    int xt, yt;

    //get the window size
    xt = glutGet(GLUT_WINDOW_WIDTH);
    yt = glutGet(GLUT_WINDOW_HEIGHT);
    //    getsize(&xt,&yt);

    //determine where the mose is in relation to the center or "origin" of the window
    if(dx != 0.0)
      dx -= xt/2;
    if(dy!= 0.0)
      dy -= yt/2;

    //    dx = ((-20 < dx) &&(20 > dx) ? 0 : dx);    
    //    dy = ((-20 < dy) &&(20 > dy) ? 0 : dy);
    if( -DEADZONE >= dx)dx+=spd;
    else if(DEADZONE<=dx)dx-=spd;
    else dx=0;
    if( -DEADZONE >= dy)dy+=spd;
    else if(DEADZONE<=dy)dy-=spd;
    else dy=0;
    //
    dy = -dy;
#ifdef _WIN32
    dx /= spd/4;
    dy /= spd/4;
#endif
  }  

  pushmatrix();
  glLoadIdentity();

  //  loadmatrix(id); //replaces what's there by the identity
  translate(aff[12],aff[13],aff[14]);
  
  if (active) {
    float torq = .005;
    rot(dx*torq,'y'); rot(-dy*torq, 'x');
    if(isMouseRightDown)
      {
	if(isShiftDown)
	  rot(-10, 'z');
	else
	  rot(-1, 'z');
      }
    
    if(isMouseLeftDown)
      {
	if(isShiftDown)
	  rot(10, 'z');
	else
	  rot(1,'z');
      }
  }

  translate(-aff[12],-aff[13],-aff[14]);
  multmatrix(aff); getmatrix(aff); //x = x+dx  aff<- daff*aff
  popmatrix();
  multmatrix(aff); //replaces rotate turn tilt
}

void Rotor::CurrentMousePosition(int x, int y)
{
  mouseX = x;
  mouseY = y;
}

void Rotor::Activate()
{
  active = 1;
}

void Rotor::Deactivate()
{
  active = 0;
}

void Rotor::MouseLeftDown()
{
  isMouseLeftDown =1;
}

int Rotor::MouseLeftUp()
{
  isMouseLeftDown =0;
  return isMouseRightDown;
}

void Rotor::MouseRightDown()
{
  isMouseRightDown =1;
}

int Rotor::MouseRightUp()
{
  isMouseRightDown =0;
  return isMouseLeftDown;
}

void Rotor::ShiftDown()
{
  isShiftDown = 1;
}

void Rotor::ShiftUp()
{
  isShiftDown =0;
}


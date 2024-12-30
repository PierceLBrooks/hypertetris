// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// Shared.h
// Last modified: May 8th, 2001 - Benjamin Bernard
//
// (C) 1996 Board of Trustees University of Illinois

#ifndef _SHARED_H_
#define _SHARED_H_

#define X 0
#define Y 1
#define Z 2
#define W 3


//attempt to convert to OpenGL
#define BF_SA GL_SRC_ALPHA
#define BF_MSA GL_ONE_MINUS_SRC_ALPHA

#define pushmatrix()   {glMatrixMode(GL_MODELVIEW); glPushMatrix();}
#define loadmatrix   glLoadMatrixf
#define multmatrix   glMultMatrixf
#define popmatrix    glPopMatrix
#define bgntmesh()   glBegin(GL_TRIANGLE_STRIP) 
#define endtmesh     glEnd
#define bgnpolygon() glBegin(GL_POLYGON)
#define endpolygon() glEnd
#define bgnline()    glBegin(GL_LINE_STRIP);
#define endline      glEnd
#define linewidth   
#define depthcue    
#define v3f          glVertex3fv
#define v2f          glVertex2fv
#define translate    glTranslatef
#define viewport     glViewport
#define window       glFrustum
#define ortho        glOrtho
#define ortho2(a,b,c,d)   glOrtho(a,b,c,d,10.0,-10) /* ogl has no ortho2 ?? */
#define swapbuffers  glutSwapBuffers
#define rectzoom     glPixelZoom
#define blendfunction glBlendFunc
#define zbuffer(a) glEnable(GL_DEPTH_TEST)
#define doublebuffer ChoosePixelFormat
//#define clear() glClear(GL_COLOR_BUFFER_BIT) // has problems with clear in the iostream library
#define zclear() glCear(GL_DEPTH_BUFFER_BIT)

#include <GL/gl.h>


void getmatrix(float* mat);

void rectcopy(int x1, int y1, int x2, int y2, int newx, int newy);

void cpack(long hexad);

void rot(int degrees, char axis);


void lrectwrite(int x1, int y1, int x2, int y2, unsigned long* offbase);

#endif












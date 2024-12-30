#ifndef _SHARED_FUNCTIONS_H
#define _SHARED_FUNCTIONS_H

#include <GL/gl.h>
#include "Shared.h"

/**********************************************************************/
//void rotate(int angle, int axis){return;}
void getmatrix(float* mat){glGetFloatv(GL_MODELVIEW_MATRIX, mat);}
/**********************************************************************/
//int getbutton(unsigned char key){ /* returns that keybit was set */
//  if(CLAW & (1<<key)){ CLAW &= (-1 ^ (1 << key)); return 1;}
//  else return 0; /* and if it was, unsets it */  /* gkf 23jul2K */
//}

void rectcopy(int x1, int y1, int x2, int y2, int newx, int newy){
    glCopyPixels( x1, y1, x2-x1, y2-y1,  GL_COLOR); 
}

void cpack(long hexad){
   float red, green, blue;
   red   = (hexad & 255)/255.;
   green = ((hexad >> 8) & 255)/255.;
   blue =  ((hexad >> 16)& 255)/255.;
   glColor3f(red,green,blue);
 }

//Pelsmajer changed int to float Jan13-05
void rot(float degrees, char axis)
{
  if(axis == 'x')
    glRotatef(degrees, 1.,0.,0.);
  else if(axis == 'y')
    glRotatef(degrees, 0.,1.,0.);
  else if(axis == 'z')
    glRotatef(degrees, 0.,0.,1.);
}


void lrectwrite(int x1, int y1, int x2, int y2, unsigned long* offbase){
    int pair[2]; 
    pair[0]=x1;
    pair[1]=y1;
    glRasterPos2iv(pair) ;
    glDrawPixels(x2-x1, y2-y1, GL_RGB, GL_UNSIGNED_INT, offbase);

}



#endif

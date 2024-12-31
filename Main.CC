// Greg Kaiser
//
// CS290 with Prof. Francis
// MATH198 with Prof. Francis
// 4D Tetris
//
// Main.C
// modified: April 30, 2001 by Benjamin Bernard
//
// (C) 1996-2005 Board of Trustees University of Illinois
// modified: 13jan04 S Levy, Pelsmajer and Francis

#define DEADZONE 40     //no use, doesn't propagate to Rotor.C
#define SQUARE   0.4

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifndef _WIN32
#include <sys/time.h>
#endif
// #include <device.h> from Irix
//#include <GL/glut.h>
//#include <GL/gl.h>
// #include <gl.h>
#include <glut.h>
#ifndef _WIN32
#include <strings.h>
#endif
#include <math.h>
#include "Board.h"
#include "Shared.h"
#include "GenPiece.h"
#include "Pieces.h"
#include "Rotor.h"
#include "DList.h"	// unnecessary?
#include "Hyper.h"	// unnecessary?

#define SOAK(K)  while(getbutton(K))

// How do we instantiate a template?  Just name one?
#include "DList.CC"  // ugh
template class DList<Hyper>;

int *DIM;
float *LEN;
Board *board;
float *botcorner;
int SHADEUNDER;
int gameon;
Rotor **quadrant;
int AUTOFALL;
int *dims;
int ONE_TURN_AT_A_TIME;
int fallsec;
int fallusec;
int numpieces;
int *mypieces;
int numremoved;
GenPiece *dude;

void InitGlobals()
{
  gameon = 1;
  quadrant = new Rotor*[4];
  for (int i = 0; i < 4; i++)
    quadrant[i] = new Rotor();

  AUTOFALL = 1;
  dims = new int[4];
  dims[VX] = dims[VY] = dims[VZ] = 4;
  dims[VW] = 10;
  ONE_TURN_AT_A_TIME = 1;
  fallsec =  4;
  fallusec = 0;
  mypieces = NULL;
  SHADEUNDER = 0;
  numremoved =0;

  DIM = new int[4];
  for (int i = 0; i < 4; i++)
    DIM[i] = dims[i];
  LEN = new float[4];
  LEN[VX] = LEN[VY] = LEN[VZ] = LEN[VW] = 4.5 / (float)(DIM[VW]);
  botcorner = new float[4];
  for (int i = 0; i < 4; i++)
    botcorner[i] = -2.0 + ((4.0 - LEN[i] * DIM[i]) / 2);
  board = new Board();
}
  
GenPiece *NewPiece(int numpieces, int *mypieces)
{
#ifndef _WIN32
  int piecenum = mypieces[(random() % numpieces)];
#else
	srand(time(NULL));
  int piecenum = mypieces[(rand() % numpieces)];
#endif
  //cout << piecenum << endl;

  GenPiece *ans;
  switch (piecenum) {
  case 1:
    ans = new CubePiece();
    break;

  case 2:
    ans = new LPiece();
    break;

  case 3:
    ans = new SPiece();
    break;

  case 4:
    ans = new StraightPiece();
    break;

  case 5:
    ans = new TPiece();
    break;

  case 6:
    ans = new CornerPiece();
    break;

  case 7:
    ans = new TwistyPiece();
    break;

  case 8:
    ans = new ZPiece();
    break;

  case 9:
    ans =  new ThreeLinePiece();
    break;

  case 10:
    ans = new TriPiece();
    break;

  case 11:
    ans = new TwoPiece();
    break;

  case 12:
    ans = new SimplePiece();
    break;
  }

  return ans;
}
void EndGame(int endState)
{
  if (gameon == 0) 
    std::cout << "Game Over, Man!" << std::endl;
  else
    std::cout << "Thanks for playing" << std::endl;


  //delete DIM; //program crashes when this line is run, instead of ending

  delete LEN;
  delete board;
  delete botcorner;
  
  
  for(int i=0; i<4; i++)
    {
      delete quadrant[i];
    }
  
  delete quadrant;
  
  delete dims;
  delete mypieces;
  
  delete dude;

  exit(endState);
}


void Timer(int callValue)
{//Timer called if fallsec in seconds and fallusec in microseconds has passed
  if (AUTOFALL && (gameon == 1)) {
    if (!(dude->Translate(VW, -1))) { //if a new piece is necessary
      gameon = dude->DonePiece();
      delete dude;
      dude = NewPiece(numpieces, mypieces);
      if(gameon !=1)
	{
	  EndGame(0);
	}
    }
    //reset the callback
    glutTimerFunc(((fallsec*1000) + (fallusec/1000)), Timer, 1);
  }
}



void CleanUpGlobals()
{
  delete DIM;
  delete LEN;
  delete board;
  delete botcorner;
}

int MyQuad(int getx, int gety) {
  int sx, sy;

  sx = glutGet(GLUT_WINDOW_WIDTH);
  sy = glutGet(GLUT_WINDOW_HEIGHT);


  if (getx <= (int)(sx / 2))
    {
      if(gety <= (int)(sy /2))
	return 0;
      else
	return 2;
    }
  else
    {
      if(gety <= (sy / 2))
	return 1;
      else
	return 3;
    }
}
 
int StrToInt(char *changeme) {
  int ans = (int)(changeme[0]) - 48;
  for (int i = 1; changeme[i] != '\0'; i++)
    ans = 10*ans + (int)(changeme[i]) - 48;
  return ans;
}


void Keyboard(unsigned char key, int x, int y)
{
  if(gameon)
    {
      if(key == 'p')
	{
	  if(AUTOFALL)
	    AUTOFALL=0;
	  else
	    {
	      AUTOFALL=1;
	      if(gameon)
		glutTimerFunc(((fallsec*1000) + (fallusec/1000)), Timer, 1);
	      
	    }	      
	  
	}
      else if(glutGetModifiers() == GLUT_ACTIVE_ALT)
	{ //test for alt true, so its a rotation, find out the axies
	  
	  if(key == ('6' & '8'))
	    {
	      dude->Rotate(VX, VY, 1);
	    }
	  else if(key == ('6' & '2'))
	    {
	      dude->Rotate(VX, VY, -1);	  
	    }
	  else if(key == ('6' & '9'))
	    {
	      dude->Rotate(VX, VZ, 1);
	    }
	  else if(key == ('6' & '1'))
	    {
	      dude->Rotate(VX, VZ, -1);
	    }
	  else if(key == ('6' & '7'))
	    {
	      dude->Rotate(VX, VW, 1);	 
	    }
	  else if(key == ('6' & '3'))
	    {
	      dude->Rotate(VX, VW, -1);
	    }
	  else if(key == ('4' & '8'))
	    {
	      dude->Rotate(VX, VY, -1);
	    }
	  else if(key == ('4' & '2'))
	    {
	      dude->Rotate(VX, VY, 1);
	    }
	  else if(key == ('4' & '9'))
	    {
	      dude->Rotate(VX, VZ, -1);
	    }
	  else if(key == ('4' & '1'))
	    {
	      dude->Rotate(VX, VZ, 1);	  
	    }
	  else if(key == ('4' & '7'))
	    {	
	      dude->Rotate(VX, VW, -1);
	    }
	  else if(key == ('4' & '3'))
	    {
	      dude->Rotate(VX, VW, 1);	  
	    }
	  else if(key == ('8' & '9'))
	    {
	      dude->Rotate(VY, VZ, 1);
	    }
	  else if(key == ('8' & '1'))
	    {
	      dude->Rotate(VY, VZ, -1);
	    }
	  else if(key == ('8' & '7'))
	    {
	      dude->Rotate(VY, VW, 1);
	    }
	  else if(key == ('8' & '3'))
	    {
	      dude->Rotate(VY, VW, -1);
	    }
	  else if(key == ('2' & '9'))
	    {
	      dude->Rotate(VY, VZ, -1);
	    }
	  else if(key == ('2' & '1'))
	    {
	      dude->Rotate(VY, VZ, 1);
	    }
	  else if(key == ('2' & '7'))
	    {
	      dude->Rotate(VY, VW, -1);
	    }
	  else if(key == ('2' & '3'))
	    {
	      dude->Rotate(VY, VW, 1);
	    }
	  else if(key == ('9' & '7'))
	    {
	      dude->Rotate(VZ, VW, 1);
	    }
	  else if(key == ('9' & '3'))
	    {
	      dude->Rotate(VZ, VW, -1);
	    }
	  else if(key == ('1' & '7'))
	    {
	      dude->Rotate(VZ, VW, -1);
	    }
	  else if(key == ('1' & '3'))
	    {
	      dude->Rotate(VZ, VW, 1);
	    }
	} //test for alt false
      else
	{ // key is not an alt-modified key, 
	  // check for translation, drop or escape
	  switch(key)
	    {//if number, translate along that axis
	      case '6': {dude->Translate(VX, 1); break;}
	      case '4': {dude->Translate(VX, -1); break;}
	      case '8': {dude->Translate(VY, 1); break;}
	      case '2': {dude->Translate(VY, -1); break;}
	      case '9': {dude->Translate(VZ, 1); break;}
	      case '1': {dude->Translate(VZ, -1); break;}
	      case '7': {dude->Translate(VW, 1); break;}
	      case '3': {dude->Translate(VW, -1); break;}
	      case ' ': { //space pressed, so drop the piece
		dude->Drop();
		gameon = dude->DonePiece();
		delete dude;
		dude = NewPiece(numpieces, mypieces); 
		if(gameon != 1)
		  {
		    EndGame(0);
		  }
		break;
	      }  
              case 27: {
	        EndGame(0); 
	        break;
	      }
	    }
	  
	}
    }
  glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
  for(int i=0; i<4; i++)
    {
      quadrant[i]->CurrentMousePosition(x,y);
    }

  if(state == GLUT_UP)
    {
      if(button == GLUT_LEFT_BUTTON)
	{
	  for(int i=0; i<4; i++)
	    {
	      quadrant[i]->ShiftUp();
	      quadrant[i]->MouseLeftUp();
	    }
	}
      else if(button == GLUT_RIGHT_BUTTON)
	{
	  for(int i=0; i<4; i++)
	    {
	      quadrant[i]->ShiftUp();
	      quadrant[i]->MouseRightUp();
	    }	  
	}
    }
  else if(glutGetModifiers() == GLUT_ACTIVE_CTRL)
    {
      if(button == GLUT_LEFT_BUTTON)
	{//if Control-Left click, only rotate the quadrant clicked
	  if (ONE_TURN_AT_A_TIME)
	    for (int i = 0; i < 4; i++)
	      quadrant[i]->Deactivate();
  
	  quadrant[MyQuad(x, y)]->Activate();
	}
      else if(button == GLUT_RIGHT_BUTTON)
	{//if Control-Right click, stop rotating all quadrants
	  for (int i = 0; i < 4; i++)
	    quadrant[i]->Deactivate();
	}
      else if(button == GLUT_MIDDLE_BUTTON)
	{
	  for( int i=0; i<4; i++)
	    {
	      quadrant[i]->Activate();
	    }
	}
    }
  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT && state == GLUT_DOWN)
    {
      if(button == GLUT_LEFT_BUTTON)
	{
	  quadrant[MyQuad(x,y)]->MouseLeftDown();
	  quadrant[MyQuad(x,y)]->ShiftDown();
	}
      else if(button == GLUT_RIGHT_BUTTON)
	{
	  quadrant[MyQuad(x,y)]->MouseRightDown();
	  quadrant[MyQuad(x,y)]->ShiftDown();	  
	}
    }
  else if(button == GLUT_LEFT_BUTTON)
    {
      quadrant[MyQuad(x,y)]->MouseLeftDown();
    }
  else if(button == GLUT_RIGHT_BUTTON)
    {
      quadrant[MyQuad(x,y)]->MouseRightDown();
    }
  else if(button == GLUT_MIDDLE_BUTTON)
    {//"pause" or unpause the game, i.e. no more autofall
      gameon = -gameon;
      if(gameon && AUTOFALL)
	glutTimerFunc(((fallsec*1000) + (fallusec/1000)), Timer, 1);

    }

  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT && state == GLUT_DOWN)
    {
      if(button == GLUT_LEFT_BUTTON)
	{
	  quadrant[MyQuad(x,y)]->MouseLeftDown();
	  quadrant[MyQuad(x,y)]->ShiftDown();
	}
      else if(button == GLUT_RIGHT_BUTTON)
	{
	  quadrant[MyQuad(x,y)]->MouseRightDown();
	  quadrant[MyQuad(x,y)]->ShiftDown();	  
	}
    }
  glutPostRedisplay();
}

void SpecialKeyboard(int key, int x, int y)
{
  if(key == GLUT_KEY_F1)
    {
      dude->Drop();
      gameon = dude->DonePiece();
      delete dude;
      dude = new CubePiece();
    }
  else if(key == GLUT_KEY_F2)
    {
      dude->Drop();
      gameon = dude->DonePiece();
      delete dude;
      dude = new LPiece();
    }
  else if(key == GLUT_KEY_F3)
    {
      dude->Drop();
      gameon = dude->DonePiece();
      delete dude;
      dude = new SPiece();
    }
  else if(key == GLUT_KEY_F4)
    {
      dude->Drop();
      gameon = dude->DonePiece();
      delete dude;
      dude = new StraightPiece();
    }
  else if(key == GLUT_KEY_F5)
    {
      dude->Drop();
      gameon = dude->DonePiece();
      delete dude;
      dude = new TPiece();
    }
  else if(key == GLUT_KEY_F6)
    {
      dude->Drop();
      gameon = dude->DonePiece();
      delete dude;
      dude = new CornerPiece();
    }
  else if(key == GLUT_KEY_F7)
    {
      dude->Drop();
      gameon = dude->DonePiece();
      delete dude;
      dude = new TwistyPiece();
    }
  else if(key == GLUT_KEY_F8)
    {
      dude->Drop();
      gameon = dude->DonePiece();
      delete dude;
      dude = new ZPiece();
    }
  else if(key == GLUT_KEY_F9)
    {
      dude->Drop();
      gameon = dude->DonePiece();
      delete dude;
      dude = new ThreeLinePiece();
    }
  else if(key == GLUT_KEY_F10)
    {
      dude->Drop();
      gameon = dude->DonePiece();
      delete dude;
      dude = new TriPiece();
    }
  else if(key == GLUT_KEY_F11)
    {
      dude->Drop();
      gameon = dude->DonePiece();
      delete dude;
      dude = new TwoPiece();
    }
  else if(key == GLUT_KEY_F12)
    {
      dude->Drop();
      gameon = dude->DonePiece();
      delete dude;
      dude = new SimplePiece();
    } 
  else if(glutGetModifiers() == GLUT_ACTIVE_ALT)
    { //test for alt true, so its a rotation, find out the axies, w/o numlock
	  
      if(key == (GLUT_KEY_RIGHT & GLUT_KEY_RIGHT))
	{
	  dude->Rotate(VX, VY, 1);
	}
      else if(key == (GLUT_KEY_RIGHT & GLUT_KEY_DOWN))
	{
	  dude->Rotate(VX, VY, -1);	  
	}
      else if(key == (GLUT_KEY_RIGHT & GLUT_KEY_PAGE_UP))
	{
	  dude->Rotate(VX, VZ, 1);
	}
      else if(key == (GLUT_KEY_RIGHT & GLUT_KEY_END))
	{
	  dude->Rotate(VX, VZ, -1);
	}
      else if(key == (GLUT_KEY_RIGHT & GLUT_KEY_HOME))
	{
	  dude->Rotate(VX, VW, 1);	 
	}
      else if(key == (GLUT_KEY_RIGHT & GLUT_KEY_PAGE_DOWN))
	{
	  dude->Rotate(VX, VW, -1);
	}
      else if(key == (GLUT_KEY_LEFT & GLUT_KEY_UP))
	{
	  dude->Rotate(VX, VY, -1);
	}
      else if(key == (GLUT_KEY_LEFT & GLUT_KEY_DOWN))
	{
	  dude->Rotate(VX, VY, 1);
	}
      else if(key == (GLUT_KEY_LEFT & GLUT_KEY_PAGE_UP))
	{
	  dude->Rotate(VX, VZ, -1);
	}
      else if(key == (GLUT_KEY_LEFT & GLUT_KEY_END))
	{
	  dude->Rotate(VX, VZ, 1);	  
	}
      else if(key == (GLUT_KEY_LEFT & GLUT_KEY_HOME))
	{	
	  dude->Rotate(VX, VW, -1);
	}
      else if(key == (GLUT_KEY_LEFT & GLUT_KEY_PAGE_DOWN))
	{
	  dude->Rotate(VX, VW, 1);	  
	}
      else if(key == (GLUT_KEY_UP & GLUT_KEY_PAGE_UP))
	{
	  dude->Rotate(VY, VZ, 1);
	}
      else if(key == (GLUT_KEY_UP & GLUT_KEY_END))
	{
	  dude->Rotate(VY, VZ, -1);
	}
      else if(key == (GLUT_KEY_UP & GLUT_KEY_HOME))
	{
	  dude->Rotate(VY, VW, 1);
	}
      else if(key == (GLUT_KEY_UP & GLUT_KEY_PAGE_DOWN))
	{
	  dude->Rotate(VY, VW, -1);
	}
      else if(key == (GLUT_KEY_DOWN & GLUT_KEY_PAGE_UP))
	{
	  dude->Rotate(VY, VZ, -1);
	}
      else if(key == (GLUT_KEY_DOWN & GLUT_KEY_END))
	{
	  dude->Rotate(VY, VZ, 1);
	}
      else if(key == (GLUT_KEY_DOWN & GLUT_KEY_HOME))
	{
	  dude->Rotate(VY, VW, -1);
	}
      else if(key == (GLUT_KEY_DOWN & GLUT_KEY_PAGE_DOWN))
	{
	  dude->Rotate(VY, VW, 1);
	}
      else if(key == (GLUT_KEY_PAGE_UP & GLUT_KEY_HOME))
	{
	  dude->Rotate(VZ, VW, 1);
	}
      else if(key == (GLUT_KEY_PAGE_UP & GLUT_KEY_PAGE_DOWN))
	{
	  dude->Rotate(VZ, VW, -1);
	}
      else if(key == (GLUT_KEY_END & GLUT_KEY_HOME))
	{
	  dude->Rotate(VZ, VW, -1);
	}
      else if(key == (GLUT_KEY_END & GLUT_KEY_PAGE_DOWN))
	{
	  dude->Rotate(VZ, VW, 1);
	}
    } //test for alt false
  else
    { // key is not an alt-modified key, 
      // check for translation, drop or escape
      switch(key)
	{//if number, translate along that axis
	  case GLUT_KEY_RIGHT: {dude->Translate(VX, 1); break;}
	  case GLUT_KEY_LEFT: {dude->Translate(VX, -1); break;}
	  case GLUT_KEY_UP: {dude->Translate(VY, 1); break;}
	  case GLUT_KEY_DOWN: {dude->Translate(VY, -1); break;}
	  case GLUT_KEY_PAGE_UP: {dude->Translate(VZ, 1); break;}
	  case GLUT_KEY_END: {dude->Translate(VZ, -1); break;}
	  case GLUT_KEY_HOME: {dude->Translate(VW, 1); break;}
	  case GLUT_KEY_PAGE_DOWN: {dude->Translate(VW, -1); break;}
	}
    }

  if(gameon ==0)
    EndGame(0);


  glutPostRedisplay();
}

void Idle()
{
  if (gameon == 1) {
    numremoved += board->CheckBoard();
    
    if (AUTOFALL && (numremoved >= 2)) {
      //this code will increase the speed at which pieces fall

      int rem = fallsec % 8;
      rem *= 7;
      rem %= 8;
      rem = (int)((float)(rem)*5000000.0/8.0);
      fallsec = (int)((float)(fallsec)*7.0/8.0);
      fallusec = (int)((float)(fallusec)*7.0/8.0) + rem;
      
      numremoved = 0;
    }
  }
  else if(gameon ==0)
    {
      EndGame(0);
    }
  glutPostRedisplay();
}




void Display()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  ortho(-5.01, 5.01, -5.01, 5.01, -5.01, 5.01);
  
  cpack(0); 

  //clear(); zclear(); replaced by the line below
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // put the x, y, z axes in the upper right hand corner (quad 1)

  
  pushmatrix();
  glColor3f(.5,.5,.5);
  glBegin(GL_LINE_STRIP);
  glVertex3f(SQUARE,SQUARE,0);
  glVertex3f(SQUARE,-SQUARE,0);
  glVertex3f(-SQUARE,-SQUARE,0);
  glVertex3f(-SQUARE,SQUARE,0);
  glVertex3f(SQUARE,SQUARE,0);
  glEnd();
  glBegin(GL_LINE_STRIP);
  glVertex3f(-SQUARE,0,0);
  glVertex3f(SQUARE,0,0);
  glEnd();
  glBegin(GL_LINE_STRIP);
  glVertex3f(0,-SQUARE,0);
  glVertex3f(0, SQUARE,0);
  glEnd();
  
  popmatrix();

  pushmatrix();
  translate(2.5, 2.5, 0.0);
  quadrant[1]->Rotate();
  dude->Drawit(7);
  board->Drawit(7);
  popmatrix();
  // put the w, x, y axes in the upper left hand corner (quadrant 0)
  pushmatrix();
  translate(-2.5, 2.5, 0.0);
  quadrant[0]->Rotate();
  dude->Drawit(11);
  board->Drawit(11);
  popmatrix();
  
  // put the z, w, x axes in the lower left corner (quadrant 2)
  pushmatrix();
  translate(-2.5, -2.5, 0.0);
  quadrant[2]->Rotate();
  dude->Drawit(13);
  board->Drawit(13);
  popmatrix();

  // put the y, z, w axes in the lower right corner (quadrant 3)
  pushmatrix();
  translate(2.5, -2.5, 0.0);
  quadrant[3]->Rotate();
  dude->Drawit(14);
  board->Drawit(14);
  popmatrix();
 
  swapbuffers();
}

void MouseMotion(int x, int y)
{
  for(int i=0; i<4; i++)
    {
      quadrant[i]->CurrentMousePosition(x,y);
    }
}

void Reshape(int width, int height)
{
  glutPostRedisplay();
  glViewport(0,0,width,height);
}


int main(int argc, char **argv)
{
  std::cout << "\n\nHyperTetris\n    by\nGreg Kaiser\n" << std::endl;
  std::cout << "(C) 1996 Board of Trustees University of Illinois\n" << std::endl;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
  
  glutInitWindowPosition(300,0);
  glutInitWindowSize(985,985);
  //keepaspect(1,1); Not sure what this translates to

  
  glutCreateWindow("4-D Tetris"); 
    // make sure you open the window BEFORE initializing
    // any data structures.  Otherwise, you'll dump core
  
  //winconstraints();  // should allow window to be resized not necessary?

  InitGlobals();  

  int i = 1;

  while (i < argc) {
    if (!(strcmp(argv[i], "-NOFALL")))
      AUTOFALL = 0;

    else if (!(strcmp(argv[i], "-DIM")))
      for (int j = 0; j < 4; j++) {
	i++;
	dims[j] = StrToInt(argv[i]);
      }

    else if (!(strcmp(argv[i], "-MULTITURN")))
      ONE_TURN_AT_A_TIME = 0;

    else if (!(strcmp(argv[i], "-FALLTIME"))) {
      i++;
      fallsec = StrToInt(argv[i++]);
      fallusec = StrToInt(argv[i]);

    } else if (!(strcmp(argv[i], "-MYPIECES"))) {
      i++;
      numpieces = StrToInt(argv[i]);
      mypieces = new int[numpieces];
      for (int j = 0; j < numpieces; j++) {
	i++;
	mypieces[j] = StrToInt(argv[i]);
      }
      
    } else if (!(strcmp(argv[i], "-SHADEUNDER")))
      SHADEUNDER = 1;

    else {
      std::cout << std::endl;
      std::cout << "The command line argument " << argv[i] << " isn't recognized.";
      std::cout << std::endl << "Please see the README file for acceptable arguments.";
      std::cout << std::endl << "Thank you.  Have a nice day!" << std::endl;
    }

    i++;
  }

  if (mypieces == NULL) {
    numpieces = 11;
    mypieces = new int[numpieces];
    for (int i = 0; i < numpieces; i++) {
      mypieces[i] = i + ((i < 7) ? 1 : 2);
    }
  }
	
  dude = NewPiece(numpieces, mypieces);
  //GenPiece *dude = new CubePiece();
  //GenPiece *dude = new LPiece();
  //GenPiece *dude = new SPiece();
  //GenPiece *dude = new StraightPiece();
  //GenPiece *dude = new TPiece();
  //GenPiece *dude = new CornerPiece();
  //GenPiece *dude = new TwistyPiece();
  //GenPiece *dude = new ZPiece();
  //GenPiece *dude = new ThreeLinePiece();
  //GenPiece *dude = new TriPiece();
  //GenPiece *dude = new TwoPiece();
  //GenPiece *dude = new SimplePiece();

  if(AUTOFALL) 
    {
      glutTimerFunc(((fallsec*1000) + (fallusec/1000)), Timer, 1);
    }
 
  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);
  glutSpecialFunc(SpecialKeyboard);
  glutMouseFunc(Mouse);
  glutPassiveMotionFunc(MouseMotion);
  glutMotionFunc(MouseMotion);
  glutIdleFunc(Idle);
  glutReshapeFunc(Reshape);
  glutMainLoop();
}






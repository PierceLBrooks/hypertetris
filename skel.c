/****************************************************************/
/****    skel.c = OpenSkelGlut.c = Noosh97 with CAVE removed ****/
/****    (C) 1994 Board of Trustees University of Illinois   ****/ 
/****    A Model Real-Time Interactive OpenGL Application    ****/
/****    George Francis, Glenn Chappell, Chris Hartman       ****/
/****    e-mail  gfrancis@math.uiuc.edu                      ****/
/******* revised 16jan2K by gkf**********************************/
/****************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>    /* for the speedometer */
#include <glut.h>
#define  MAX(x,y)         (((x)<(y))?(y):(x))
#define  MIN(x,y)         (((x)<(y))?(x):(y))
#define  ABS(u)           ((u)<0 ? -(u): (u))
#define  FOR(a,b,c)       for((a)=(b);(a)<(c);(a)++)
#define  DOT(p,q)         ((p)[0]*(q)[0]+(p)[1]*(q)[1]+(p)[2]*(q)[2])
#define  NRM(p)           sqrt(DOT((p),(p)))
#define  DG            M_PI/180
#define  S(u)          sin(u*DG)
#define  C(u)          cos(u*DG)
#define  CLAMP(x,u,v) (x<u? u : (x>v ? v: x))
/* global variables */
float gap, gap0=1.; /* kludge so that arguments() can set a default gap0 */
float lux[3]={1.,2.,3.};             /*world light direction vector     */
float luxx[3];                       /*  object space  direction vector*/
float amb, pwr ;             /* ambient fraction, pseudo-specular power */
float mysiz,speed, torq, focal, far; /*console navigation variables         */
int win = 1;                   /* 2 full screen, use 0 for demand sized */
unsigned int BUT, XX, YY,SHIF;     /* used in chaptrack gluttery           */ 
int xt,yt;                       /* once was xt,yt,xm,ym for viewportery */
int mode,morph,msg,binoc;            /* pretty global */
int th0, th1, dth, ta0, ta1, dta;    /* torus parameters */
#define FLYMODE  (0)
#define TURNMODE (1) 
int ii, jj, kk;  float tmp, temp;   /* saves gray hairs later */
float aff[16], starmat[16], mat[16]; 
int binoc;    /* flag                       */
float nose;   /* to eye distance in console */
/**********************************************************************/
void autotymer(int reset){ /* cheap animations */
#define  TYME(cnt,max,act) {static cnt; if(first)cnt=max; else\
                            if(cnt?cnt--:0){ act ; goto Break;}}
  static first = 1;  /* the first time autymer is called */
  if(reset)first=1;  /* or if it is reset to start over  */
  TYME( shrink , 150,th0++;th1--;ta0++;ta1--)
  TYME( pause  , 20,          )
  TYME( grow   , 150,th0--;th1++;ta0--;ta1++)
  TYME( dwell  , 30,            )
  TYME(finish  , 1 , first = 1  )
  first = 0;
  Break:   ;   /* yes Virginia, C has gotos */
}
/**********************************************************************/
void deFault(void){ float tmp; 
th0=5; th1=355;  ta0=5; ta1=355; gap = gap0; 
msg=1; binoc=0; nose=.06; mode=TURNMODE;  
speed=.1; torq=.02; focal = 2.; far =13.; mysiz=.01; morph=0; 
FOR(ii,0,16) starmat[ii]=aff[ii] = (ii/4==ii%4);   /* identities */ 
tmp=NRM(lux); FOR(ii,0,3)lux[ii]/=tmp; amb = .3; pwr = 10. ;
aff[12]=0; aff[13]= 0; aff[14]= -4.2;   /* place where we can see it */
autotymer(1); /* reset autotymer to start at the beginning */
}
/**********************************************************************/
void drawvert(int th, int ta){  
float lmb,spec,nn[3], dog, cat;
nn[0] = C(th)*C(ta); /* unit sphere radius vector */  
nn[1] = S(th)*C(ta);
nn[2] =       S(ta);
lmb = DOT(nn,luxx);lmb =(lmb<0 ? .2 : lmb);  lmb = MAX(amb, lmb); 
spec = CLAMP((1.1 - pwr+pwr*lmb), 0., 1.);  
dog = (ta-ta0)/(float)(ta1-ta0); cat = (th-th0)/(float)(th1-th0);
glColor3f(
  MAX(spec, lmb*dog),                    /* map R^2(dog,cat)->R^3(RGBspace */
  MAX(spec, lmb*(.25 + ABS(cat -.5))),  /* dog cat model of Hartman       */
  MAX(spec, lmb*(1 - cat)));            /* illiLight by Ray Idaszak 1989  */
glVertex3f(
  C(th) + .5*nn[0],
  S(th) + .5*nn[1],
          .5*nn[2]);
} /* end drawvert */
/**********************************************************************/
void drawtor(void){
int th, ta;
dth = (int)((th1-th0)/24);  /* 24  meridian strips */ 
dta = (int)((ta1-ta0)/24);  /* 24  triangle pairs per strip */
for(th=th0; th < th1; th += dth){
  glBegin(GL_TRIANGLE_STRIP);
  for(ta=ta0; ta < ta1; ta += dta){ drawvert(th,ta); drawvert(th+gap*dth,ta); }
  glEnd();
  } /* end for.theta loop */
}   /* end drawtor */
/**********************************************************************/
void drawcube(void){ /* transfer from skel.c as an exercise  */ }

/**********************************************************************/
void drawall(void){ drawtor(); drawcube();}

/**********************************************************************/
void drawstars(void){ 
     static float star[1000][3]; static int virgin=1;
 if(virgin){  /* first time  set up the stars */
     FOR(ii,0,1000)FOR(jj,0,3)star[ii][jj]=random()/(float)0x40000000-1.;
     virgin=0; /* never again */
  }
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();   /* insurance of superstition */
  glMultMatrixf(starmat);
  glColor3f(1.0,1.0,0.0);    
  glBegin(GL_POINTS);
    FOR(ii,0,1000)glVertex3fv(star[ii]);
  glEnd();
  glPopMatrix();
  glClear(GL_DEPTH_BUFFER_BIT); /* so the stars are behind everything */
}
/**********************************************************************/
void arguments(int argc,char **argv){
  extern char *optarg;
  extern int optind; 
  int chi;   
  /* w: needs ONE number after -w, c<nothing> means NO number follows*/ 
  while ((chi = getopt(argc,argv,"w:d:g:")) != -1) 
  switch(chi)  {
                case 'w': win=atoi(optarg); break; 
                case 'g': gap0=atof(optarg); break;
                }
  if (optind!=argc) fprintf(stderr,"%s: Incorrect usage\n",argv[0]);
}
/**********************************************************************/
void keyboard(unsigned char key, int x, int y){
#define  IF(K)            if(key==K)
#define  PRESS(K,A,b)     IF(K){b;}IF((K-32)){A;}  
/*was backwards in previous versions */
#define  TOGGLE(K,flg)    IF(K){(flg) = 1-(flg); }
#define  CYCLE(K,f,m)     PRESS((K), (f)=(((f)+(m)-1)%(m)),(f)=(++(f)%(m)))
#define  SLIDI(K,f,m,M)   PRESS(K,(--f<m?m:f), (++f>M?M:f))
#define  SLIDF(K,f,m,M,d) PRESS(K,((f -= d)<m?m:f), ((f += d)>M?M:f))
/* Only ASCII characters can be processes by this GLUT callback function */ 
   IF(27) { exit(0); }              /* ESC exit            */
   TOGGLE('v',binoc);                            /* cross-eyed STEREO   */
   CYCLE(' ', mode,TURNMODE+1);                  /* fly/turn modes      */
   TOGGLE('h',morph);                            /* autotymer on/off    */
   TOGGLE('w',msg);                              /* writing on/off      */
   PRESS('n', nose -= .001 , nose += .001 );     /* for binoculars      */
   PRESS('s',speed /= 1.02, speed *= 1.02);      /* flying speed        */
   PRESS('q',torq /= 1.02, torq *= 1.02);        /* turning speed       */
   PRESS('o', focal *= 1.1 , focal /= 1.1)       /* telephoto           */
   PRESS('i', mysiz /= 1.1, mysiz *= 1.1)        /* rescale the world   */
   PRESS('p', far *= 1.01 , far   /= 1.01)       /* rear clipping plane */
   PRESS('z', deFault(), deFault());             /* zap changes         */
   PRESS('g',gap /= .9, gap *= .9);             /* gap parameter      */
   PRESS('a',amb /= .9, amb *= .9);             /* ambient fraction   */
   PRESS('r',pwr /= .9, pwr *= .9);             /* pseudo-spec power  */
}
/**********************************************************************/
void special_keybo(int key, int x, int y){
/*non-ASCII keypresses go here, if you're lucky enough to know their names */
 fprintf(stderr," non-ASCII character was pressed.\n");
 fprintf(stderr," use special_keybo() to process it\n");
}
/**********************************************************************/
float speedometer(void){
double dbl; static double rate; static int ii=0;
static struct timezone notused; static struct timeval now, then;
   if(++ii % 8 == 0){  /* 8 times around measure time */
      gettimeofday(&now, &notused); /* elapsed time */
      dbl =  (double)(now.tv_sec - then.tv_sec)
         +(double)(now.tv_usec - then.tv_usec)/1000000;
      then = now;  rate = 8/dbl;
      }
   return((float)rate);
}
/**********************************************************************/
void char2wall(float x,float y,float z, char buf[]){
     char *p; glRasterPos3f(x,y,z);
     for(p = buf;*p;p++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*p);
}
/**********************************************************************/
void messages(void){     
  char buf[256]; /* console messages are done differently from cave */
#define  LABEL2(x,y,W,u) {sprintf(buf,(W),(u));char2wall(x,y,0.,buf);}
  glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
  gluOrtho2D(0,3000,0,3000);
  glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
      /*bull's eye*/
      if(mode==TURNMODE) glColor3f(0x22/255.,0x88/255.,0xdd/255.);
        else      glColor3f(.8,.8,.8);
      LABEL2(1500,1500,"%s","o");
      /* writings */
      if(mode==TURNMODE) glColor3f(1.,0.,1.);
        else      glColor3f(1.,1.,0.);
      LABEL2(80,80,"%4.1f fps",speedometer());
      LABEL2(80,2840,\
      "(ESC)ape (V)Binoc (MAUS2)Fore (BAR)Flymode %s (H)omotopy (W)riting",
             mode?"FLYING":"CONTROL");
      LABEL2(10,10,"illiOpenSkelGLUT \
   by Francis, Bourd, Hartman & Chappell, U Illinois, 1995..1997 %s","");
      LABEL2(80,2770,"(N)ose   %0.3f",nose);
      LABEL2(80,2700,"(S)peed  %0.4f",speed);
      LABEL2(80,2630," tor(Q) %0.4f",torq);
      LABEL2(80,2560,"near clipper %g", mysiz*focal);
      LABEL2(80,2490,"f(O)cal factor %g",focal);
      LABEL2(80,2420,"my s(I)ze %.2g",mysiz);
      LABEL2(80,2350,"far cli(P)er= %.2g",far);
      LABEL2(80,2280,"(Z)ap %s","");
      LABEL2(80,2210,"(G)ap %.2g",gap);
      LABEL2(80,2140,"(A)mb %.2g",amb);
      LABEL2(80,2070,"pw(R) %.2g",pwr);
     
    glPopMatrix();
glMatrixMode(GL_PROJECTION); glPopMatrix();
} 
/**********************************************************************/
void chaptrack(int but,int xx,int yy,int shif){  
   long dx,dy; 
   dx = xx -.5*xt; dx = abs(dx)>5?dx:0;
   dy = yy -.5*yt; dy = abs(dy)>5?dy:0;

   glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

   if(mode==TURNMODE) glTranslatef(aff[12],aff[13],aff[14]);
   glRotatef(dx*torq,0.,1.,0.); glRotatef(dy*torq,1.,0.,0.);
   if(but&(1<<GLUT_RIGHT_BUTTON ))glRotatef(shif?-10:-1,0.,0.,1.);
   if(but&(1<<GLUT_LEFT_BUTTON  ))glRotatef(shif?10:1,0.,0.,1.);
   if(mode==FLYMODE){
      glPushMatrix();
      glMultMatrixf(starmat);
      glGetFloatv(GL_MODELVIEW_MATRIX,starmat);
      glPopMatrix();
   }
   if(but&(1<<GLUT_MIDDLE_BUTTON)) glTranslatef(0.,0.,shif ? -speed : speed);
   if(mode==TURNMODE) glTranslatef(-aff[12],-aff[13],-aff[14]);
   glMultMatrixf(aff); 
   glGetFloatv(GL_MODELVIEW_MATRIX,aff);
   FOR(ii,0,3){luxx[ii]=0; FOR(jj,0,3)luxx[ii] +=aff[ii*4+jj]*lux[jj];}
   glPopMatrix();
}
/**********************************************************************/
void reshaped(int xx, int yy){xt=xx ; yt=yy;}   /* origin of moved window */
/**********************************************************************/
void drawcons(void){  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if(binoc) glViewport(0,yt/4,xt/2,yt/2);
  glMatrixMode(GL_PROJECTION); glLoadIdentity();
  glFrustum(-mysiz*xt/yt,mysiz*xt/yt,-mysiz,mysiz,mysiz*focal,far); 
  glMatrixMode(GL_MODELVIEW); glLoadIdentity();
  drawstars();
  glTranslatef(-binoc*nose,0.0,0.0);
  glMultMatrixf(aff);
  drawall();
  if(binoc){
    glViewport(xt/2,yt/4,xt/2,yt/2); 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawstars();
    glTranslatef(binoc*nose,0.0,0.0);
    glMultMatrixf(aff);
    drawall();
    }
  glViewport(0,0,xt,yt);
  if(msg) messages();
  glutSwapBuffers();
}

/**********************************************************************/
void idle(void){ /*do this when nothing else is happening*/
   if(morph) autotymer(0);  /* advance autotymer */ 
   glutPostRedisplay();  /*redraw the window*/
   chaptrack(BUT,XX,YY,SHIF);
}

/**********************************************************************/
void mousepushed(int but,int stat,int x,int y){
  if(stat==GLUT_DOWN) BUT |= (1<<but);
  else BUT &= (-1 ^ (1<<but));  
  XX=x; YY=y; SHIF=(glutGetModifiers()==GLUT_ACTIVE_SHIFT)?1:0;
}

/**********************************************************************/
void mousemoved(int x,int y){  XX=x; YY=y; }
/**********************************************************************/
int main(int argc, char **argv){  
   arguments(argc,argv); 
   deFault(); 
 
       glutInit(&argc, argv);
       glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
       switch(win){ 
           case 0: break;  /* manage your own window */
           case 1: glutInitWindowSize(640, 480);
                   glutInitWindowPosition(100,100); break;
           case 2: glutInitWindowPosition(0,0); break;
         }
       glutCreateWindow("<* illiSkel in C/OpenGL/GLUT *>");
       if(win==2) glutFullScreen();
       glEnable(GL_DEPTH_TEST);
       glutDisplayFunc(drawcons);       
       glutKeyboardFunc(keyboard);
       glutSpecialFunc(special_keybo);
       glutMouseFunc(mousepushed);
       glutMotionFunc(mousemoved);       
       glutPassiveMotionFunc(mousemoved); 
       glutReshapeFunc(reshaped);
       glutIdleFunc(idle);             
       glutMainLoop();
}
 

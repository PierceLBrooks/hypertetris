/* igl2ogl translation by */
/* Matthew Rakher, Math 198, 2001 */
/* lica.c copyright Chris Hartman 1992-1997*/
/* this is the final version 27feb97 */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>    /* for the speedometer */
#include <glut.h>
#include <malloc.h>
#pragma warning (disable:581)  
/* igl2ogl defines.i20 */
#define LEFTMOUSE  '[' 
#define RIGHTMOUSE  ']' 
#define ENDKEY 'e'
#define ESCKEY 27 
#define AKEY 'A'
#define UKEY 'U'
#define JKEY 'J'
#define TKEY 'T'
#define PKEY 'P'
#define QKEY 'Q'
#define SKEY 'S'
#define RKEY 'R'
#define IKEY 'I'
#define NKEY 'N'
#define HKEY 'H'
#define LKEY 'L'
#define VKEY 'V'
#define SCROLLOCKKEY  
#define MOUSE3
#define MOUSEX
#define MOUSEY
#define CAPSLOCK
#define  fsin        sin
#define  fcos        cos
#define  fsqrt       sqrt
#define pushmatrix   glPushMatrixf
#define loadmatrix   glLoadMatrixf
#define multmatrix   glMultMatrixf
#define popmatrix    glPopMatrixf
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

#define IFCLICK(K,a){static flag=1;if(getbutton(K))flag=1-flag;if(flag){a} }
/* global variables */
float gap, gap0=1.; /* kludge so that arguments() can set a default gap0 */
float lux[3]={1.,2.,3.};             /*world light direction vector     */
float luxx[3];                       /*  object space  direction vector*/
float amb, pwr ;             /* ambient fraction, pseudo-specular power */
float mysiz,speed, torq, focal, far; /*console navigation variables         */
int win = 1;                   /* 2 full screen, use 0 for demand sized */
unsigned int CLAW,PAW,XX,YY,SHIF;     /* used in chaptrack gluttery           */
int xt,yt;                       /* once was xt,yt,xm,ym for viewportery */
int mode,morph,msg,binoc;            /* pretty global */
int th0, th1, dth, ta0, ta1, dta;    /* torus parameters */
#define FLYMODE  (0)
#define TURNMODE (1)
int ii, jj, kk;  float tmp, temp;   /* saves gray hairs later */
float aff[16], starmat[16], mat[16];
int binoc;    /* flag                       */
float nose;   /* to eye distance in console */
/* end define.i2o */
/**********************************************************************/
void autotymer(int reset){ /* cheap animations */
#define  TYME(cnt,max,act) {static cnt; if(first)cnt=max; else\
                            if(cnt?cnt--:0){ act ; goto Break;}}
  static first = 1;  /* the first time autymer is called */
  if(reset)first=1;  /* or if it is reset to start over  */
#if 0
  TYME( shrink , 150,th0++;th1--;ta0++;ta1--)
  TYME( pause  , 20,          )
  TYME( grow   , 150,th0--;th1++;ta0--;ta1++)
#endif
  TYME( dwell  , 30,            )
  TYME(finish  , 1 , first = 1  )
  first = 0;
  Break:   ;   /* yes Virginia, C has gotos */
}
/**********************************************************************/
void deFault(void){
#if 0
th0=5; th1=355;  ta0=5; ta1=355; gap = gap0;
msg=1; binoc=0; nose=.06; mode=TURNMODE;
speed=.1; torq=.02; focal = 2.; far =13.; mysiz=.01; morph=0;
FOR(ii,0,16) starmat[ii]=aff[ii] = (ii/4==ii%4);   /* identities */
FOR(ii,0,3)lux[ii]/=NRM(lux); amb = .3; pwr = 10. ;
aff[12]=0; aff[13]= 0; aff[14]= -4.2;   /* place where we can see it */
autotymer(1); /* reset autotymer to start at the beginning */
#endif
}
/**********************************************************************/
void cpack(long hexad){
   float red, green, blue;
   red   = (hexad & 255)/255.;
   green = ((hexad >> 8) & 255)/255.;
   blue =  ((hexad >> 16)& 255)/255.;
   glColor3f(red,green,blue);
 }
/**********************************************************************/

/**********************************************************************/
void rotate(int angle, int axis){return;}
void getmatrix(float* mat){glGetFloatv(GL_MODELVIEW_MATRIX, mat);}
/**********************************************************************/
int getbutton(unsigned char key){ /* returns that keybit was set */
  if(CLAW & (1<<key)){ CLAW &= (-1 ^ (1 << key)); return 1;}
  else return 0; /* and if it was, unsets it */  /* gkf 23jul2K */
}
/**********************************************************************/
/* moved the keyboard stuff into the keyboard */
/*********************** Command Line Arguments ***********/

#define MAXX            1280
#define MAXCOLORS       8
#define MAXR            3  /*Maximum radius*/
#define RND             (random()/((float)0x80000000-1.0))
#define MAX(x,y)	((x)>(y)?(x):(y))
#define TBLSIZ		(sumrule?(2*radius+1)*(colors-1)+1:pow(colors,2*radius+1))

char *line1, *line2, *table, *swline, *rules, filerules[1000][256],userrule[256];
unsigned long *offbase,masks[MAXR+1],
    pixval[MAXCOLORS+1][MAXCOLORS]= {{0,0,0,0,0,0,0,0}, 
    {0xff880088,0,0,0,0,0,0,0},       /*The following are the seven different color schemes.*/
    {0xff880088,0xff0044aa,0,0,0,0,0,0},
    {0xff0088aa,0xff660066,0xffaa0000,0,0,0,0,0},
    {0xff0000aa,0xff0088aa,0xffaa8800,0xffff0000,0,0,0,0},
    {0xff0000aa,0xff0088aa,0xff00aa00,0xffff4400,0xff880088,0,0},
    {0xff000088,0xff008888,0xff00ff88,0xff00aa22,0xffff8822,0xffff0044,0,0},
    {0xff000088,0xff008888,0xff00ff88,0xff00aa22,0xffff8822,0xffff0044,0xff880066,0},
    {0xff000088,0xff008888,0xff00ff88,0xff00aa22,0xffaaff44,0xffff8822,0xffff0044,0xff880066}};
long xsize=MAXX,ysize,licawin,statuswin;
int rulenum=-1,numrules=0,gen=-1,bpc,sumrule=1,
    phade=0,slowed=1,cellsize=5,scrolling=0,radius=1,colors=4;
FILE *rulefile;

void rectcopy(int x1, int y1, int x2, int y2, int newx, int newy){
    glCopyPixels( x1, y1, x2-x1, y2-y1,  GL_COLOR); 
}


void lrectwrite(int x1, int y1, int x2, int y2, unsigned long* offbase){
    int pair[2]; 
    pair[0]=x1;
    pair[1]=y1;
    glRasterPos2iv(pair) ;
    glDrawPixels(x2-x1, y2-y1, GL_RGB, GL_UNSIGNED_INT, offbase);

}
/*attempt to transfer lrectwrite to ogl*/

arguments(int argc,char **argv)
{
extern char *optarg;
extern int optind;
int c;
   while ((c = getopt(argc,argv,"d:c:su:r:z:"))!= -1)
      switch (c)
         {
         case 'c': colors=atoi(optarg);
	    if((colors>MAXCOLORS)||(colors<2))
	       {printf("You must have 2<=colors<=%d.\n",MAXCOLORS); exit(1);} break;
         case 'd': slowed=atoi(optarg);
	    if((slowed<1)) {printf("You can't have a negative delay.\n"); exit(1);} break;
         case 'z': cellsize=atoi(optarg);
	    if((cellsize>100)||(cellsize<1))
	       {printf("You must have 1<=cellsize<=100.\n"); exit(1);} break;
         case 'r': radius=atoi(optarg); 
	    if((radius>3)||(radius<0)) {printf("Radius must be 0,1,2 or 3.\n"); exit(1);} break;
         case 'u': sumrule=atoi(optarg); 
	    if((sumrule<0)||(sumrule>1)) {printf("Sum"); exit(1);} break;
         }
   if (optind!=argc) { fprintf(stderr,
         "Usage: %s [-u rulebysums?] [-d delay] [-r radius] [-c numcolors] [-z squaresize] [-s].\n",argv[0]);
	 exit(1);
	 }
   printf("Running with %d colors, radius %d, rules are%sby sums.\n",
           colors, radius,(sumrule?" ":" not "));
}

calcnewline(char *from, char *to)
{
unsigned long ii, cframe = 0;

for (ii = 0; ii < 2 * radius; ii++) cframe = (cframe << bpc) | from[ii];
for (ii = 2 * radius; ii < xsize + 2 * radius + 1; ii++) {
    cframe = masks[radius] & ((cframe << bpc) | from[ii % xsize]);
    to[(ii - radius + phade) % xsize] = rules[cframe];
    offbase[(ii - radius) % xsize] = pixval[colors][rules[cframe]];
    }
}

makerules()
{
int ii,jj=0;
unsigned long sum,aa[2*MAXR+2],state;

for(ii=0;ii<2*radius+1;ii++) aa[ii]=0;
while(aa[2*radius]<colors) {
   for(ii=sum=state=0;ii<2*radius+1;ii++){
      sum += aa[ii];
      state=state | (aa[ii]<<bpc*ii);
      }
   rules[state]=table[sumrule?sum:jj];
   aa[0]++;
   for(ii=1;((aa[ii-1]==colors)&&(ii<2*radius+1));ii++)
      {
      aa[ii-1]=0;
      aa[ii]++;
      }
   jj++;
   }
}

makerandtable()
{
int ii;
for(ii=0;ii<TBLSIZ;ii++)
   table[ii]=(RND*(float)colors);
}

randomize()
{
int ii;
for(ii=0;ii<xsize;ii++) 
   {
   line1[ii]=(RND * colors);
   offbase[ii]=pixval[colors][line1[ii]]; /*set up the offscreen color values*/
   }                                     /*for later copying to the screen*/
}

void drawit(void){  /* draw into licawin */
   static int jj = 0;
   glutSetWindow(licawin); /* winset(licawin); */

   if ((jj-=cellsize) < 0) jj = ysize*cellsize;
   if(scrolling) {
       rectzoom(1.0,1.0);
       rectcopy(0,0,xsize*cellsize,(ysize-1)*cellsize,0,cellsize);
       rectzoom((float)cellsize,(float)cellsize);
       lrectwrite(0,0,xsize,0,offbase);
       }
   else {
       rectzoom((float)cellsize,(float)cellsize);
       lrectwrite(0,jj,xsize,jj,offbase);
   }
}

void initlica(void){
   int  ii;
   for(bpc=0;(1L<<bpc)< MAXCOLORS;bpc++) ;
   for(ii=0;ii<=MAXR;ii++) masks[ii] = (1L<<(bpc*(2*ii+1)))-1;
   line1 = (char *) malloc(MAXX);
   line2 = (char *) malloc(MAXX);
   table = (char *) malloc(pow(MAXCOLORS,2*MAXR+1));
   offbase = (unsigned long *) malloc(MAXX * sizeof(unsigned long));
   rules = (char *) malloc(((unsigned long)1<<(MAXR*2+1)*bpc)-1);
   if (rules == NULL) {printf("Couldn't allocate memory for rules.\n"); exit(0);}
   srandom(time(0));  /*Seen random number gen w/ the time*/
   readrules();
   makerandtable();
   makerules();
   randomize();
}

readrules()
{
if(rulefile = fopen("licarules","r"))
   {
   for(numrules=0;fscanf(rulefile,"%s",filerules[numrules])!=EOF;numrules++) ;
   fclose(rulefile);
   }
}

dofullrule(char arule[])
{
sscanf(arule,"%d:%d:%d:",&colors,&radius,&sumrule);
dorule(arule+6);
}

void updatestatus(void);  /* use precedes definition */

dorule(char darule[]){
   int ii;
   for(ii=0;ii<TBLSIZ;ii++){
       table[ii]=darule[ii]-'0';
       if ((table[ii] < 0) ||  (table[ii] >= colors)){
           table[ii]=colors-1;
           fprintf(stderr,"Error in rule, clipping the values.\n");
      }
   }
   makerules();
   randomize();
   updatestatus();
}

saverule()
{
int ii;

if(rulefile = fopen("licarules","a"))
   {
   fprintf(rulefile,"%d:%d:%d:",colors,radius,sumrule);
   for(ii=0;ii<TBLSIZ;ii++)
      fprintf(rulefile,"%d",table[ii]);
   fprintf(rulefile,"\n");
   fclose(rulefile);
   readrules();
   printf("Saved as rule #%d.\n",numrules);
   }
}

newrule()
{
rulenum=-1;
makerandtable();
makerules();
updatestatus();
}


void keyboard(unsigned char key, int x, int y){
int inx,entry,choice;
long xo,yo;

CLAW |= (1<<key);                         /* called-back by key pressed  */
SHIF=(glutGetModifiers()==GLUT_ACTIVE_SHIFT)?1:0; /* shiftkey was down too*/


#define SHIFT (getbutton(LEFTSHIFTKEY)||getbutton(RIGHTSHIFTKEY))
/*
if(getbutton(SCROLLLOCKKEY))  {scrolling=!scrolling; while(getbutton(SCROLLLOCKKEY));}
*/
if(getbutton(SKEY))  {saverule(); while(getbutton(SKEY));}
if(getbutton(UKEY))  {sumrule = 1-sumrule; updatestatus(); while(getbutton(UKEY));}
#if 0
if(getbutton(UPARROWKEY)) {rulenum++; if(rulenum>=numrules) rulenum=-1;
          else dofullrule(filerules[rulenum]);while(getbutton(UPARROWKEY));}
if(getbutton(DOWNARROWKEY)) {rulenum--; if(rulenum<0) rulenum=-1;
          else dofullrule(filerules[rulenum]);while(getbutton(DOWNARROWKEY));}
if(getbutton(NKEY))  {newrule();while(getbutton(NKEY));}
if(getbutton(F1KEY)) {for(inx=0;inx<xsize;inx++) line1[inx] = 0;}
if(getbutton(F2KEY)) {for(inx=0;inx<xsize;inx++)
         offbase[inx] = pixval[colors][line1[inx] = (char) (inx*colors/xsize)];}
if(getbutton(F3KEY)) randomize();
if(getbutton(CKEY)) {
		    if(SHIFT){colors--; if(colors<2)colors=2; newrule();randomize();}
                    else {colors++; updatestatus();if(colors>MAXCOLORS)colors=MAXCOLORS;}
		    while(getbutton(CKEY)) ;
		    }
if(getbutton(RKEY)) {
		    if(SHIFT){radius--;updatestatus();if(radius<0)radius=0;}
                    else {radius++;updatestatus();if(radius>MAXR)radius=MAXR;}
		    while(getbutton(RKEY)) ;
		    }
if(getbutton(IKEY))  {printf("Rule? ");rulenum=-1;scanf("%s",userrule);dorule(userrule);}
if(getbutton(COMMAKEY)) {slowed++;while(getbutton(COMMAKEY));}
if(getbutton(PERIODKEY)) {slowed--;if(slowed<0)slowed=0;while(getbutton(PERIODKEY));}
if(getbutton(PAD4))  {phade--;while(getbutton(PAD4));}
if(getbutton(PAD5))  phade=0;
if(getbutton(PAD6))  {phade++;while(getbutton(PAD6));}
if(getbutton(F12KEY)) {inx=RND*xsize;offbase[inx] = pixval[colors][line1[inx]
         = ((int)(line1[inx]+1+(RND*(colors-1)))%colors)]; while(getbutton(F12KEY));}
if(getbutton(VKEY))  {updatestatus();rulenum=rulenum++;}
while(getbutton(CAPSLOCKKEY)); /*pause*/
if(getbutton(MOUSE3))
   {
glutSetWindow(statuswin); /* winset(statuswin); */
   getorigin(&xo,&yo);
   choice = (getvaluator(MOUSEX)-xo-5)/25;
   entry = TBLSIZ-((getvaluator(MOUSEY)-yo-5)/25)-1;
   if((choice>=0)&&(choice<colors)&&(entry<TBLSIZ)&&(entry>=0))
      {table[entry]=choice; makerules(); updatestatus();}
   }
#endif
}

/**********************************************************************/
void special_keybo(int key, int x, int y){




/*non-ASCII keypresses go here, if you're lucky enough to know their names */
 fprintf(stderr," non-ASCII character was pressed.\n");
 fprintf(stderr," use special_keybo() to process it\n");
}
/**********************************************************************/


/**********************************************************************/
void char2wall(float x,float y,float z, char buf[]){
     char *p; glRasterPos3f(x,y,z);
     for(p = buf;*p;p++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*p);
}
/**********************************************************************/
void square(int sz, int xx, int yy, long cc) {
   float vv[2]; char phrase[10];
   bgnpolygon();
   cpack(pixval[colors][cc]);
   vv[0]=xx; vv[1]=yy;
   v2f(vv); vv[0]+=sz;
   v2f(vv); vv[1]-=sz;
   v2f(vv); vv[0]-=sz;
   v2f(vv);
   endpolygon();
   if (sz==20) {cpack(0xffffffff); 
   sprintf(phrase,"%d",cc); 
   char2wall(xx+8,yy-14,0,phrase);
   }
}

void updatestatus(void){
   char phrase[256],name[20];
   int ii,jj,kk;
   long xt,yt;

   glutSetWindow(statuswin); /* winset(statuswin); */
   /* I don't know what the new name for that is  reshapeviewport(); */ 
   /* getsize(&xt,&yt);  glutPostRedisplay??*/
   if((xt!=MAX(210,colors*25+100)||yt!=75+25*TBLSIZ))
   {
    /*
   prefsize(MAX(210,colors*25+100),75+25*TBLSIZ);
   winconstraints(); don't know gkf 
    */
   }
   viewport(0,MAX(210,colors*25+100),25*TBLSIZ,25*TBLSIZ+75);
   cpack(0);  /* clear(); */
   ortho2(0.,1.,0.,1.); cpack(0xffffffff);
   sprintf(phrase,"Radius=%d, %d colors.",radius,colors);
   char2wall(.05,.8,0,phrase);
   sprintf(phrase,"Rules are%sby sums.",sumrule?" ":" not ");
   char2wall(.05,.5,0,phrase);
   for(ii=0;ii<TBLSIZ;ii++)sprintf(phrase+ii,"%d",table[ii]);
   if(rulenum>=0)  sprintf(phrase+ii," (%d/%d)",rulenum+1,numrules);
   char2wall(.05,.2,0,phrase);

   viewport(0,MAX(210,colors*25+100),0,25*TBLSIZ);
   ortho2(0,MAX(210,colors*25+100),0,25*TBLSIZ);
   cpack(0);  /* clear(); */
   for(ii=0;ii<TBLSIZ;ii++) {
   for(jj=0;jj<colors;jj++) square(20,5+jj*25,25*(TBLSIZ-ii),jj);
   square(20,MAX(210,colors*25+100)-25,25*(TBLSIZ-ii),table[ii]);
   if(sumrule)
      {
      sprintf(phrase,"%2d=",ii);
      char2wall(MAX(210,colors*25+100)-65,25*(TBLSIZ-ii)-14,0,phrase);
      }
   else for(kk=0;kk<2*radius+1;kk++)
         square(10,MAX(210,colors*25+100)-100+(2*radius+1-kk)*12,25*(TBLSIZ-ii)-8,
	 (ii%(int)pow(colors,kk+1))/pow(colors,kk));
   }
  swapbuffers();
  glutSetWindow(licawin); /* winset(licawin); */
}

/* I delete old main */

/**********************************************************************/
void reshaped(int xx, int yy){xt=xx ; yt=yy;}   /* origin of moved window */
/**********************************************************************/
void drawcons(void){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#if 0
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
#endif
}
/**********************************************************************/
void idle(void){ /*do this when nothing else is happening*/
   if(morph) autotymer(0);  /* advance autotymer */ 
   glutPostRedisplay();  /*redraw the window*/
/*
   IFCLICK('+',chaptrack(PAW,XX,YY,SHIF);)
*/
}

/**********************************************************************/
void mousepushed(int but,int stat,int x,int y){
  if(stat==GLUT_DOWN) PAW |= (1<<but);  /* key came down and calledback */
  else PAW &= (-1 ^ (1<<but));          /* on the wayup erase flag */ 
  XX=x; YY=y;  /* position in window coordinates (pos integers) */ 
  SHIF=(glutGetModifiers()==GLUT_ACTIVE_SHIFT)?1:0; /* shiftkey was down too*/
}

/**********************************************************************/
void mousemoved(int x,int y){  XX=x; YY=y; }
/**********************************************************************/
int main(int argc, char **argv){  
   arguments(argc,argv); 
   deFault(); 
   initlica();           /*get memory, set up variables, etc.*/
 
       glutInit(&argc, argv);
       glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);
       switch(win){ 
           case 0: break;  /* manage your own window */
           case 1: glutInitWindowSize(640, 480);
                   glutInitWindowPosition(100,100); break;
           case 2: glutInitWindowPosition(0,0); break;
         }
       glutCreateWindow("<* Lica in C/OpenGL/GLUT *>");
       if(win==2) glutFullScreen();
       glEnable(GL_DEPTH_TEST);
       glutDisplayFunc(drawcons);       
/* the following 5 aren't needed for a picture */
       glutKeyboardFunc(keyboard);
       glutSpecialFunc(special_keybo);
       glutMouseFunc(mousepushed);
       glutMotionFunc(mousemoved);       
       glutPassiveMotionFunc(mousemoved); 
/*  beyond here you do need them */
       glutReshapeFunc(reshaped);
       glutIdleFunc(idle);             
       glutMainLoop();
}
 

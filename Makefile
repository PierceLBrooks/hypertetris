OBJS =  Main.o\
        Shared.o\
        FourD.o\
        Hyper.o\
        Asserts.o\
        Board.o\
        GenPiece.o\
        CubePiece.o\
        DList.o\
        LPiece.o\
        StraightPiece.o\
        TPiece.o\
        SPiece.o\
        CornerPiece.o\
        TwistyPiece.o\
        ZPiece.o\
        ThreeLinePiece.o\
        TriPiece.o\
        TwoPiece.o\
        SimplePiece.o\
        Rotor.o

LOCAL = /afs/ncsa/projects/math/local
LIB   = o32
# LIB   = o32
# LOCAL = /usr/local/
GLINC = -I$(LOCAL)/include
GLUTINC = -I$(LOCAL)/include/GL
CAVEINC = -I/usr/local/CAVE/include 

CAVELIB = -L/usr/local/CAVE/lib -lcave_ogl 
GLUTLIB = -L$(LOCAL)/$(LIB) -lglut
GLLIB   = -L$(LOCAL)/$(LIB) -lGL -lGLU
SYSLIBS = -L/usr/X11R6/lib -lX11 -lXi -lXext -lXmu   -lm


# OPT =  -DCAVE -O2
# OPT =  -DCAVE -n32 -mips3
OPT =  -DCAVE -o32 

#CFLAGS = ${OPT} ${CAVEINC} ${GLUTINC}  ${GLINC}  -woff 1116,1233,3341
#CC doesn't understand -woff
CFLAGS = ${OPT} ${CAVEINC} ${GLUTINC}  ${GLINC}  
LDFLAGS = ${OPT} ${CAVELIB}  ${GLUTLIB} ${GLLIB} ${SYSLIBS}

tetris: $(OBJS) 
	CC -o $@ $(OBJS)  $(CFLAGS)  $(LDFLAGS) 
.C.o:
	CC  $(CFLAGS)   -c $<


	

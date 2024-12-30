// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// Asserts.h
// Last modified: April 17, 1996
//
// (C) 1996 Board of Trustees University of Illinois

//////////////////////////////////////////////////////////////////////////////
//        Note: For descriptions of the functions, look at Asserts.C        //
//////////////////////////////////////////////////////////////////////////////

#ifndef ASSERTS_H
#define ASSERTS_H

#include <stdlib.h>   // for the exit(1)
#include <iostream>

void Assert(int SafeCondition, char* ErrMsg);

void Warn(int SafeCondition, char* ErrMsg);

#endif

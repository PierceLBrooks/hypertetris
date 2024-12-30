// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// Asserts.C
// Last modified: April 17, 1996
//
// (C) 1996 Board of Trustees University of Illinois

#include "Asserts.h"

// Given:   A SafeCondition, in the form of a boolean expression, and a ErrMsg
// Task:    If the SafeCondition isn't true, then the ErrMsg is displayed to
//          to the screen as an error and the program is terminated
// Return:  Nothing
void Assert(int SafeCondition, char* ErrMsg)
{
  if (!SafeCondition) {
    cerr << "***Error: " << ErrMsg << endl;
    exit(1);
  }
}

// Given:   A SafeCondition, in the form of a boolean expression, and a ErrMsg
// Task:    If the SafeCondition isn't true, then the ErrMsg is displayed to
//          to the screen as a warning
// Return:  Nothing
void Warn(int SafeCondition, char* ErrMsg)
{
  if (!SafeCondition)
    cerr << "***Warning: " << ErrMsg << endl;
}

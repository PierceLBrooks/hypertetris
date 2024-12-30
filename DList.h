// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// DList.h
// Last modified: April 17, 1996
//
// (C) 1996 Board of Trustees University of Illinois

#ifndef _DLIST_H_
#define _DLIST_H_

#include <iostream>
#include "Asserts.h"
class Hyper;

template <class T>
struct Node
{
  Node() {next = prev = NULL;};
  ~Node() {delete data;  if (next != NULL) delete next;}
  Node<T> *next,
          *prev;
  T *data;
};

template <class T>
class DList
{
friend int FindAboveW(DList<Hyper> *List, int wcoord);
friend int FindAtW(DList<Hyper> *List, int wcoord);

private:
  Node<T> *head,
          *tail,
          *current;

public:
  //constructors
  DList();
  DList(T *e);
  ~DList();

  void Output();

  //list manipulation
  void InsertAfter(T *e);
  void InsertBefore(T *e);
  void Remove();

  //current pointer adjustment
  inline void Head() {current = head;};
  inline void Tail() {current = tail;};
  DList& operator++ (int);
  DList& operator-- (int); //remember to use (*p)++, etc

  //value retrieval
  inline int NotEmpty() { return (head != NULL); }
  inline T *Get() {return current->data;};
  inline int EndOfList() {return current==tail;};

  Node<T> *ReturnCurrent();
  void ChangeCurrent(Node<T> *NewCurrent);
};

#endif

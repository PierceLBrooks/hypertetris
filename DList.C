// Greg Kaiser
//
// CS290 with Prof. Francis
// 4D Tetris
//
// DList.C
// Last modified: April 17, 1996
//
// (C) 1996 Board of Trustees University of Illinois

#include "DList.h"

//---------------------------DList CONSTRUCTORS----------------------//
//Given:   nothing
//Task:    creates generic constructor, with everything NULL
//Return:  nothing
template<class T>
DList<T>::DList()
{
  head = tail = current = NULL;
  Assert(head==NULL || tail==NULL || current==NULL,
	 "Not enough memory to create list");
}

//Given:   one element of type T
//Task:    creates constructor with that element
//Return:  nothing
template<class T>
DList<T>::DList(T *e)
{
  Node<T> *temp = new Node<T>;
  temp->data = e;
  temp->next = temp->prev = NULL;
  head = tail = current = temp;
}

template<class T>
DList<T>::~DList()
{
  if (head != NULL)
    delete head;
}


//-----------------------------DList MANIPULATORS------------------------//

template <class T>
void DList<T>::Output()
{
  Node<T> *Temp = head;
  while (Temp != NULL) {
    std::cout << Temp->data << std::endl;
    Temp = Temp->next;
  }
}

//Given:   one element of type T
//Task:    inserts a new Node after the current pointer
//Return:  nothing
template<class T>
void DList<T>::InsertAfter(T *e)
{
  Node<T> *temp = new Node<T>;
  temp->data = e;

  if(head==NULL && tail==NULL) {  //empty list
    temp->next = temp->prev = NULL;
    current = head = tail = temp;
  } else {
    temp->next = current->next;
    current->next = temp;
    temp->prev = current;
    if(current == tail)
      tail = temp;
    else
      temp->next->prev = temp;
    current = temp;
  }
}

//Given:   one element of type T
//Task:    inserts a new Node before the current pointer
//Return:  nothing
template<class T>
void DList<T>::InsertBefore(T *e)
{
  Node<T> *temp = new Node<T>;
  temp->data = e;
  
  if(head==NULL && tail==NULL) { //empty list
    temp->prev = temp->next = NULL;
    current = head = tail = temp;
  } else {
    temp->prev = current->prev;
    current->prev = temp;
    temp->next = current;
    if(current = head)
      head = temp;
    else
      temp->prev->next = temp;
    current = temp;
  }
}

//Given:   nothing
//Task:    removes the current Node
//Return:  nothing
template<class T>
void DList<T>::Remove()
{
  if(head==NULL && tail==NULL) //empty list
    return;
  
  if(head==tail) //one Node list
    head = tail = current = NULL;
  else {
    if(current!=head)
      current->prev->next = current->next;
    else
      head = current->next;
    if(current!=tail)
      current->next->prev = current->prev;
    else
      tail = current->prev;
    if (current->next == NULL)
      current = tail;
    else
      current = current->next;
  }
}

template<class T>
Node<T> *DList<T>::ReturnCurrent()
{
  return current;
}

template<class T>
void DList<T>::ChangeCurrent(Node<T> *NewCurrent)
{
  current = NewCurrent;
}


//------------------------DList CURRENT POINTER ADJUSTMENT-------------------//

//Given:   nothing
//Task:    move current to next Node if not the last one
//Return:  the list
template<class T>
DList<T>& DList<T>::operator++ (int)
{
  if(current != tail)
    current = current->next;
  return *this;
}

//Given:   nothing
//Task:    move current to previous Node if not the first one
//Return:  the list
template<class T>
DList<T>& DList<T>::operator-- (int)
{
  if(current != head)
    current = current->prev;
  return *this;
}

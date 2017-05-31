//*****************************************************************
// FILE:      Queue.h
// AUTHORS:   Steven Brice
// DUE DATE:  3/11/2014
//
// PURPOSE:   Contains the declaration for the Queue class.
//****************************************************************/
#ifndef QUEUE_H
#define QUEUE_H

#include "Arduino.h"


//A structure to hold the Nodes of the Queue LinkedList
template <class T>
struct Node {
  T* classInstance;
  int methodNum;
  int val;
  Node();
  Node(T* classInstance, int methodNum, int val);
};

/********************************************************************
Method:     Node

Use:        This default constructor for the struct Node sets
      the next to -1

Arguments:  None.

Returns:    Nothing.
********************************************************************/
template <class T>
Node<T>::Node(T* newClassInstance, int newMethodNum, int newVal) {
  classInstance = newClassInstance;
  methodNum = newMethodNum;
  val = newVal;
}

template <class T>
Node<T>::Node() {
  classInstance = NULL;
  methodNum = -1;
  val = -1;
}


template <class T, int queueMaxSize>
class Queue {
  private:
    int qFront;
    int qRear;
//    Node<T, queueMaxSize>* qFront;
//    Node<T, queueMaxSize>* qRear;
    int queueSize;
    int currentPosition;
    Node<T> queueArray[queueMaxSize];

  public:
    Queue();
    void clear();
    int size() const;
    bool empty() const;
    T* front();
    int frontVal();
    T* back();
    bool push(T*, int methodNum, int val);
    bool pushFront(T*, int methodNum, int val);
    void pop();
    void popAndRun();
};


/********************************************************************
Method:     Queue

Use:        This default constructor for the class Queue sets
      the queueSize to 0 and the front and rear to -1

Arguments:  None.

Returns:    Nothing.
********************************************************************/
template <class T, int queueMaxSize>
Queue<T, queueMaxSize>::Queue() {
  qFront = -1;
  qRear = -1;
  queueSize = 0;
  currentPosition = 0;
}


/********************************************************************
Method:     clear()

Use:        removes all classInstances from the queue and sets the size to 0

Arguments:  N/A

Returns:    N/A
********************************************************************/
template <class T, int queueMaxSize>
void Queue<T, queueMaxSize>::clear() {
  while(!empty()) { //keep going til the queue is empty
    pop();
  }
  qFront = -1;
  qRear = -1;
  queueSize = 0;
  currentPosition = 0;
}


/********************************************************************
Method:     size()

Use:        get the size of the current queue

Arguments:  N/A

Returns:    int
********************************************************************/
template <class T, int queueMaxSize>
int Queue<T, queueMaxSize>::size() const {
  return queueSize;
}


/********************************************************************
Method:      empty()

Use:        Checks to see if the queue has no front and rear

Arguments:  N/A

Returns:    boolean
********************************************************************/
template <class T, int queueMaxSize>
bool Queue<T, queueMaxSize>::empty() const {
  return ((qFront == -1) && (qRear == -1));
}


/********************************************************************
Method:     front()

Use:        returns the first classInstance of the queue

Arguments:  N/A

Returns:    T
********************************************************************/
template <class T, int queueMaxSize>
T* Queue<T, queueMaxSize>::front() {
  if(!empty()) { // Only return classInstance if the current queue isn't empty
    return queueArray[qFront].classInstance;
  }
}

/********************************************************************
Method:     frontVal()

Use:        returns the value saved with the front most classInstance in the queue

Arguments:  N/A

Returns:    T
********************************************************************/
template <class T, int queueMaxSize>
int Queue<T, queueMaxSize>::frontVal() {
  if(!empty()) { // Only return classInstances val if the current queue isn't empty
    return queueArray[qFront].val;
  }
}


/********************************************************************
Method:     back()

Use:        returns the last classInstance of the queue

Arguments:  N/A

Returns:    T
********************************************************************/
template <class T, int queueMaxSize>
T* Queue<T, queueMaxSize>::back() {
  if(!empty()) { // Only return classInstance if the current queue isn't empty
    return queueArray[qRear].classInstance;
  }
}


/********************************************************************
Method:     push()

Use:        adds classInstance to the end of the queue

Arguments:  T&

Returns:    N/A
********************************************************************/
template <class T, int queueMaxSize>
bool Queue<T, queueMaxSize>::push(T* classInstance, int methodNum, int val) {
//  Serial.print("qFront: ");
//  Serial.print(qFront);
//  Serial.print("\tqRear: ");
//  Serial.print(qRear);
//  Serial.print("\tqueueSize: ");
//  Serial.print(queueSize);
//  Serial.print("\tcurrentPosition: ");
//  Serial.print(currentPosition);
//  Serial.println();
  if(queueSize >= queueMaxSize) {
//    Serial.println("FULL");
    // Queue is too full to add anything else
    return false;
  }
  queueSize++; // Increase the size of the queue
  qRear = ((qRear+1)%queueMaxSize);
  Node<T> *currNode = &queueArray[qRear]; // Get the current Node to modify
  currNode->classInstance = classInstance;
  currNode->methodNum = methodNum;
  currNode->val = val;
  if(qFront == -1) {
    qFront = qRear;
  }
  return true;
}


/********************************************************************
Method:     pushFront()

Use:        adds classInstance to the front of the queue

Arguments:  T&

Returns:    N/A
********************************************************************/
template <class T, int queueMaxSize>
bool Queue<T, queueMaxSize>::pushFront(T* classInstance, int methodNum, int val) {
  if(queueSize >= queueMaxSize) {
//    Serial.println("FULL");
    // Queue is too full to add anything else
    return false;
  }
  queueSize++; // Increase the size of the queue
  if(qFront <= 0) {
    qFront = (queueMaxSize-1);
  } else {
    qFront--;
  }
  currentPosition = qFront;
  Node<T> *currNode = &queueArray[qFront]; // Get the current Node to modify
  currNode->classInstance = classInstance;
  currNode->methodNum = methodNum;
  currNode->val = val;
  return true;
}


/********************************************************************
Method:     popAndRun()

Use:        executes the classInstance on the top of the queue and if the run
      was successful (returned true) the classInstance is poped from the
      queue

Arguments:  N/A

Returns:    N/A
********************************************************************/
template <class T, int queueMaxSize>
void Queue<T, queueMaxSize>::popAndRun() {
  if(!empty()) {
    Node<T>* n = &queueArray[qFront];
    if(n->classInstance->call(n->methodNum, n->val)) { // Execute the front most fnc with the args
      pop(); // Remove the function just executed from the queue
    }
  }
}

/********************************************************************
Method:     pop()

Use:        removes the top classInstance from the queue

Arguments:  N/A

Returns:    N/A
********************************************************************/
template <class T, int queueMaxSize>
void Queue<T, queueMaxSize>::pop() {
  if(!empty()) {
    qFront = ((qFront+1) % queueMaxSize);
    if(qFront != -1) {
      currentPosition = ((currentPosition+1) % queueMaxSize);
      queueSize--;
    }
    if(queueSize == 0) {
      qFront = -1;
      qRear = -1;
      queueSize = 0;
    }
  }
}


#endif /* QUEUE_H */

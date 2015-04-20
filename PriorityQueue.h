/*
 * PriorityQueue.h
 *
 *  Created on: 15 March 2015
 *      Author: shaj
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "Thread.h"
#include <vector>
//___for now
#include <iostream>
#define I_RED 0
#define I_ORANGE 1
#define I_GREEN 2
#define I_BLOCKED 3
using namespace std;

class PriorityQueue
{
public:
	PriorityQueue();
	virtual ~PriorityQueue();
	Thread * popElement();
	int get_quantums(int tid);
	int enqueueElement(Thread* toAdd);
	//__________________________to erase all size matters
	int getSize();
	void increaseSize();
	void decreaseSize();
	int removeElement(int id);
	void block(int id);
	void resume(int id);
	int isBlocked(int id);
	int isQueueEmpty();
private:
	int findElement(int id);
	int findInQueue(int id, int vecNum);
	int _size;
	vector<Thread*> * _helpPtr;
	vector<Thread*> * _Red, * _Orange, *_Green, *_Blocked;
	vector<Thread*> * _queue[I_BLOCKED + 1];
};

#endif /* QUEUE_H_ */

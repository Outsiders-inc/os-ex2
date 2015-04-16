/*
 * thread.h
 *
 *  Created on: 15 March 2015
 *      Author: David Gvirts, Shahar Osovsky
 */

#ifndef THREAD_H_
#define THREAD_H_

#include "uthreads.h"
enum State {RUNNING, READY, BLOCKED};

class Thread
{
public:
	Thread(int tId, Priority priority);
	virtual ~Thread();
	int getNumOfQuantums();
	void increaseQuantums();
	int getId();
	Priority getPriority();
	void changeState(State newState);
	fptr  getFunction();

//	Thread(Thread toCopy);
private:
	int _id;
	Priority _priority;
	int _numOfQuantums;
	State _state;
};


#endif /* THREAD_H_ */

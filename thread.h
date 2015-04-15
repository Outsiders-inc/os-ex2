/*
 * thread.h
 *
 *  Created on: 15 March 2015
 *      Author: David Gvirts, Shahar Osovsky
 */

#ifndef THREAD_H_
#define THREAD_H_

#include "uthreads.h"
typedef void (*fptr)(void);
enum State {RUNNING, READY, BLOCKED};

class Thread
{
public:
	Thread(int tId, Priority priority, fptr f);
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
	fptr _func;
	int _numOfQuantums;
	State _state;
};


#endif /* THREAD_H_ */

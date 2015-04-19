/*
 * thread.cpp
 *
 *  Created on: 15 March 2015
 *      Author: David Gvirts, Shahar Osovsky
 */

#include "Thread.h"

/**
 * A constructor for Thread, receiving it's id as parameter.
 */
Thread::Thread(int tId, Priority priority) :
	_id(tId), _priority(priority), _numOfQuantums(0) , _state(READY)
{
}

///**
// * A copy constructor for Thread, receiving the source Thread as parameter.
// */
//Thread::Thread(Thread toCopy)
//{
//
//}

Thread::~Thread()
{
	// TODO Auto-generated destructor stub
}

/**
 * Returns the number of quantums this thread ran (including the current).
 */
int Thread::getNumOfQuantums()
{
	return this->_numOfQuantums;
}

/**
 * Increases the number of quantums this thread ran by one.
 */
void Thread::increaseQuantums()
{
	this->_numOfQuantums += 1;
}

/**
 * Returns this thread's id;
 */
int Thread::getId()
{
	return this->_id;
}

/**
 * Returns this thread's priority;
 */
Priority Thread::getPriority()
{
	return this->_priority;
}

/**
 * Changes this thread's state to the one given as parameter.
 */
void Thread::changeState(State newState)
{
	this->_state = newState;
}



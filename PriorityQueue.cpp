/*
 * PriorityQueue.cpp
 *
 *  Created on: 15 March 2015
 *      Author: David Gvirts, Shahar Osovsky
 */

#include "PriorityQueue.h"

PriorityQueue::PriorityQueue() :
		_size(0), _Red(), _Orange(), _Green(), _Blocked()
{
}

PriorityQueue::~PriorityQueue()
{
	_Red.clear();
	_Green.clear();
	_Orange.clear();
	_Blocked.clear();
}

/**
 * Pops and returns the first element in the priority-queue, and updates it's size accordingly.
 * If the queue is empty, it returns NULL.
 */
Thread * PriorityQueue::popElement()
{
	if (this->getSize() == 0)
	{
		return NULL;
	}
	Thread* element;
	if (this->_Red.size() > 0)
	{
		element = this->_Red.front();
		_Red.erase(_Red.begin());
	}
	else if (this->_Orange.size() > 0)
	{
		element = this->_Orange.front();
		_Orange.erase(_Orange.begin());
	}
	else
	{
		element = this->_Green.front();
		_Green.erase(_Green.begin());
	}
	return element;
}
/**
 *
 */
Thread * PriorityQueue::getThread(int id)
{
	int location = -1;
	vector<Thread *> * point = findInQueue(id, location);
	if (location == -1)
	{
		location = findElement(id, _Blocked);
		if (location != -1)
		{
			point = &_Blocked;
		}

	}
	if (location != -1)
	{
		return point->at(location);
	}
	return NULL;
}

/**
 * Adds the element received as parameter to the end of the queue, according to its priority.
 */
int PriorityQueue::enqueueElement(Thread* toAdd)
{
	switch (toAdd->getPriority())
	{
	case RED:
		this->_Red.push_back(toAdd);
		break;
	case ORANGE:
		this->_Orange.push_back(toAdd);
		break;
	default:
		this->_Green.push_back(toAdd);
		break;
	}
	increaseSize();
	return 0;
}

/**
 * Returns the priority-queue's size.
 */
int PriorityQueue::getSize()
{
	return this->_size;
}

/**
 * Increases the priority-queue's size by one
 */
void PriorityQueue::increaseSize()
{
	this->_size += 1;
}

/**
 * Decreases the priority-queue's size by one
 */
void PriorityQueue::decreaseSize()
{
	this->_size -= 1;
}

/**
 *
 */
int PriorityQueue::findElement(int id, vector<Thread *> vec)
{
	for (unsigned int i = 0; i < vec.size(); ++i)
	{
		if (vec[i]->getId() == id)
		{
			return i;
		}
	}
	return -1;
}
/**
 * Returns -1 if thread with id is not blocked and
 * location in blocked list else.
 */
int PriorityQueue::isBlocked(int id)
{
	return findElement(id, _Blocked);
}
/**
 *
 */
void PriorityQueue::removeElement(int id)
{
	Thread * toDel = getThread(id);
}
/**
 * Returns a pointer to queue in which the given id can be found.
 * Inserts into location the location in the queue.
 */
vector<Thread *> * PriorityQueue::findInQueue(int id, int &location)
{
	vector<Thread *> * point = NULL;
	int loc = findElement(id, _Red);
	if (loc != -1)
	{
		point = &_Red;
	}
	else
	{
		loc = findElement(id, _Orange);
		if (loc != -1)

		{
			point = &_Orange;
		}
		else
		{
			loc = findElement(id, _Green);
			if (loc != -1)
			{
				point = &_Green;
			}
		}
	}
	location = loc;
	return point;
}
/**
 * Moves thread with given id to blocked list.
 * Has no effect on blocked threads.
 */
void PriorityQueue::block(int id)
{
	int location = -1;
	vector<Thread *> * point = findInQueue(id, location);
	if (location != -1)
	{
		_Blocked.push_back(point->at(location));
		point->erase(point->begin() + location);
	}
}
/**
 * Moves back a blocked thread to a priority queue.
 * Has no effect on unblocked threads.
 */
void PriorityQueue::resume(int id)
{
	int location = isBlocked(id);
	if (location != -1)
	{
		enqueueElement(_Blocked.at(location));
		_Blocked.erase(_Blocked.begin() + location);
	}
}

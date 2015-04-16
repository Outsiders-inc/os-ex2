/*
 * PriorityQueue.cpp
 *
 *  Created on: 15 March 2015
 *      Author: David Gvirts, Shahar Osovsky
 */

#include "PriorityQueue.h"

PriorityQueue::PriorityQueue() :
		_size(0), _Red(), _Orange(), _Green(), _Blocked(),_helpPtr()
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
int PriorityQueue::get_quantums(int id)
{
	int location = findElement(id);
	if (location == -1)
	{
		location = findInQueue(id, _Blocked);
		if (location != -1)
		{
			_helpPtr = &_Blocked;
		}

	}
	if (location != -1)
	{
		return _helpPtr->at(location)->getNumOfQuantums();
	}
	return -1;
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
	//__________________________to erase all size matters
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
//__________________________to erase all size matters
/**
 * Increases the priority-queue's size by one
 */
void PriorityQueue::increaseSize()
{
	this->_size += 1;
}
//__________________________to erase all size matters
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
int PriorityQueue::findInQueue(int id, vector<Thread *> vec)
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
	return findInQueue(id, _Blocked);
}
/**
 *
 */
void PriorityQueue::removeElement(int id)
{
	int location = findElement(id);
	if(location != -1)
	{
		_helpPtr->erase(_helpPtr->begin() + location);
	}
}
/**
 * Returns a pointer to queue in which the given id can be found.
 * Inserts into location the location in the queue.
 */
int PriorityQueue::findElement(int id)
{
	_helpPtr = NULL;
	int loc = findInQueue(id, _Red);
	if (loc != -1)
	{
		_helpPtr = &_Red;
	}
	else
	{
		loc = findInQueue(id, _Orange);
		if (loc != -1)

		{
			_helpPtr = &_Orange;
		}
		else
		{
			loc = findInQueue(id, _Green);
			if (loc != -1)
			{
				_helpPtr = &_Green;
			}
		}
	}

	return loc;
}
/**
 * Moves thread with given id to blocked list.
 * Has no effect on blocked threads.
 */
void PriorityQueue::block(int id)
{
	int location = findElement(id);
	if (location != -1)
	{
		_Blocked.push_back(_helpPtr->at(location));
		_helpPtr->erase(_helpPtr->begin() + location);
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

/*
 * PriorityQueue.cpp
 *
 *  Created on: 15 March 2015
 *      Author: David Gvirts, Shahar Osovsky
 */

#include "PriorityQueue.h"

PriorityQueue::PriorityQueue() :
		_size(0),_helpPtr(NULL),
		_Red(new vector<Thread*>()),_Orange(new vector<Thread*>()),
		_Green(new vector<Thread*>()),
		_Blocked(new vector<Thread*>()),
		_queue()
{
	_queue[I_RED] = _Red;
	_queue[I_ORANGE] = _Orange;
	_queue[I_GREEN] = _Green;
	_queue[I_BLOCKED] = _Blocked;
}

PriorityQueue::~PriorityQueue()
{
	_Red->clear();
	_Green->clear();
	_Orange->clear();
	_Blocked->clear();
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
	if (this->_Red->size() > 0)
	{
		element = this->_Red->front();
		_Red->erase(_Red->begin());
	}
	else if (this->_Orange->size() > 0)
	{
		element = this->_Orange->front();
		_Orange->erase(_Orange->begin());
	}
	else
	{
		element = this->_Green->front();
		_Green->erase(_Green->begin());
	}
	this->decreaseSize();
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
		location = findInQueue(id, I_BLOCKED);
		if (location != -1)
		{
			_helpPtr = _Blocked;
		}

	}
	if (location != -1)
	{
		return _helpPtr->at(location)->getNumOfQuantums();
	}
	cout << "NO SUCH ID FOUND (id = " << id << ")" << endl;
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
		this->_Red->push_back(toAdd);
		break;
	case ORANGE:
		this->_Orange->push_back(toAdd);
		break;
	default:
		this->_Green->push_back(toAdd);
		break;
	}
	//__________________________to erase all size matters
	this->increaseSize();
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
int PriorityQueue::findInQueue(int id, int vecNum)
{
	cout << "\t\t\tinside 'findInQueue' with id = " << id << endl;		////-----ERASE---------------------///
	unsigned int i, size;

	size = _queue[vecNum]->size();
	cout << "\t\t\tThe queue's size is: " << size << endl;		////-----ERASE---------------------///
	for (i = 0; i < size; i++)
	{
		cout << "\t\t\t\titeration number " << i << endl;		////-----ERASE---------------------///
		if (_queue[vecNum]->at(i)->getId() == id)
		{
			return i;
		}
	}
	cout << "id not found in vector num': " << vecNum << endl;		////-----ERASE---------------------///
	return -1;
}
/**
 * Returns -1 if thread with id is not blocked and
 * location in blocked list else.
 */
int PriorityQueue::isBlocked(int id)
{
	return findInQueue(id, I_BLOCKED);
}
/**
 *
 */
int PriorityQueue::removeElement(int id)
{
	int location = findElement(id);
	if(location != -1)
	{
		_helpPtr->erase(_helpPtr->begin() + location);
		this->decreaseSize();
		return 1;
	}
	return 0;
}
/**
 * Returns a pointer to queue in which the given id can be found.
 * Inserts into location the location in the queue.
 */
int PriorityQueue::findElement(int id)
{
//	cout << "\t\t\tinside 'find element' with id = " << id << endl;		////-----ERASE---------------------///

//	_helpPtr = NULL;
	cout << "\t\t\tbefore 'findInQueue' in RED" << endl;		////-----ERASE---------------------///
	int loc = findInQueue(id, I_RED);
//	cout << "\t\t\tsearched the RED queue, loc = " << loc << endl;		////-----ERASE---------------------///
	if (loc != -1)
	{
		_helpPtr = _Red;
	}
	else
	{
		loc = findInQueue(id, I_ORANGE);
		cout << "\t\t\tsearched the ORANGE queue, loc = " << loc << endl;		////-----ERASE---------------------///
		if (loc != -1)

		{
			_helpPtr  =  _Orange;
		}
		else
		{
			loc = findInQueue(id, I_GREEN);
			cout << "\t\t\tsearched the GREEN queue, loc = " << loc << endl;		////-----ERASE---------------------///
			if (loc != -1)
			{
				_helpPtr = _Green;
			}
		}
	}
	cout << "\t\t\tthe location found is loc = " << loc << endl;		////-----ERASE---------------------///
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
		_Blocked->push_back(_helpPtr->at(location));
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
		enqueueElement(_Blocked->at(location));
		_Blocked->erase(_Blocked->begin() + location);
	}
}
/**
 * Returns 0 iff all queues except blocked are empty.
*/
int PriorityQueue::isQueueEmpty()
{
	int size = _Red->size() + _Green->size() + _Orange->size();
	return (size == 0);
}

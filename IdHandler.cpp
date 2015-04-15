/*
 * IdHandler.cpp
 *
 *  Created on: Mar 22, 2015
 *      Author: gilgvirts
 */
#include "IdHandler.h"

IdHandler::IdHandler(int maxNum): _maxNum(maxNum),_idArr(new int[maxNum])
{
	for (int var = 0; var < maxNum; ++var)
	{
		_idArr[var] = 0;
	}
}
IdHandler::~IdHandler()
{
	delete _idArr;
}
int IdHandler::getNewId()
{
	for (int var = 0; var < _maxNum; ++var)
	{
		if(_idArr[var] == 0)
		{
			_idArr[var] = 1;
			return var;
		}
	}
	return -1;
}
void IdHandler::removeId(int id)
{
	_idArr[id] = 0;
}


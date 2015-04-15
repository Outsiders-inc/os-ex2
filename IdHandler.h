/*
 * IdHandler.h
 *
 *  Created on: Mar 22, 2015
 *      Author: gilgvirts
 */

#ifndef IDHANDLER_H_
#define IDHANDLER_H_

class IdHandler{
public:
IdHandler(int maxNum);
~IdHandler();
int getNewId();
void removeId(int id);
private:
int _maxNum;
int * _idArr;
};


#endif /* IDHANDLER_H_ */

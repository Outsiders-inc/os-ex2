/*
 * Itimer.h
 *
 *  Created on: Mar 19, 2015
 *      Author: gilgvirts
 */

#ifndef ITIMER_H_
#define ITIMER_H_

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>


class Itimer{
public:
Itimer(int quanta, void (*func)(int));
void set();
};

#endif /* ITIMER_H_ */

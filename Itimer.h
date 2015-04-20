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
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define SECOND 1000000
class Itimer{
public:
Itimer(int quanta, void (*func)(int));
void set();
};

#endif /* ITIMER_H_ */

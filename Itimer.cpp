/*
 * Interval-timer demo program.
 * Hebrew University OS course.
 * Questions: os@cs.huji.ac.il
 */

#include "Itimer.h"
#include <stdio.h>
#include <iostream>
using namespace std;
struct itimerval tv;


/**
 *
 */
Itimer::Itimer(int quanta , void (*func)(int) )
{
	signal(SIGVTALRM, func);
	//Adapting seconds so decreasing will be effective
	int seconds = floor(quanta/SECOND );
	int microseconds = quanta - seconds*SECOND;
	tv.it_value.tv_sec = seconds; 
	tv.it_value.tv_usec = microseconds; 
	tv.it_interval.tv_sec = seconds; 
	tv.it_interval.tv_usec = microseconds; 
}
void Itimer::set()
{
	if(setitimer(ITIMER_VIRTUAL, &tv, NULL)==-1)
	{
		//error
		exit(1);
	}
}
//int main(void)
//{
//  signal(SIGVTALRM, timer_handler);
//
//
//  tv.it_value.tv_sec = 1;  /* first time interval, seconds part */
//  tv.it_value.tv_usec = 0; /* first time interval, microseconds part */
//  tv.it_interval.tv_sec = 1;  /* following time intervals, seconds part */
//  tv.it_interval.tv_usec = 0; /* following time intervals, microseconds part */
//
//  setitimer(ITIMER_VIRTUAL, &tv, NULL);
//  for(;;) {
//    if (gotit) {
//      printf("Got it!\n");
//      gotit = 0;
//    }
//  }
//  return 0;
//}


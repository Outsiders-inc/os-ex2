/*
 * uthreads.cpp
 *
 *  Created on: 15 March 2015
 *      Author: David Gvirts, Shahar Osovsky
 */

#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

#include "uthreads.h"
#include "Thread.h"
#include "PriorityQueue.h"
#include "Itimer.h"
#include "IdHandler.h"
#define FAILLURE -1

using namespace std;
typedef char * stack[STACK_SIZE];
stack * gMem[MAX_THREAD_NUM];
sigjmp_buf env[MAX_THREAD_NUM];
int gNumOfQuantums;
Thread* running;
PriorityQueue* threads;
Itimer * timer;
IdHandler * gHandler;

void timer_handler(int sig)
{
	// If the running process has no "competition" over CPU time.
	if (threads->isQueueEmpty()) // MAKE SURE TO WRITE "isQueueEMpty()" method
	{
		running->increaseQuantums();
		gNumOfQuantums++;
		return;
	}
	switchRunningThread();


	cout << sig << endl;//// - JUST A PRINT -@#$@#$@#$@#$@#
}

void pauseTimer()
{
	if(sigprocmask(SIG_BLOCK, ? ,NULL) == FAILURE)
	{
		//error
		exit(1);
	}
}
void resumeTimer()
{
	if (sigprocmask(SIG_UNBLOCK, ? ,NULL) == FAILURE)
	{
		//error
		exit(1);
	}
}


#ifdef __x86_64__
/* code for 64 bit Intel arch */

typedef unsigned long address_t;
#define JB_SP 6
#define JB_PC 7

/* A translation is required when using an address of a variable.
 Use this as a black box in your code. */
address_t translate_address(address_t addr)
{
	address_t ret;
	asm volatile("xor    %%fs:0x30,%0\n"
			"rol    $0x11,%0\n"
			: "=g" (ret)
			: "0" (addr));
	return ret;
}

#else
/* code for 32 bit Intel arch */

typedef unsigned int address_t;
#define JB_SP 4
#define JB_PC 5

/* A translation is required when using an address of a variable.
 Use this as a black box in your code. */
address_t translate_address(address_t addr)
{
	address_t ret;
	asm volatile("xor    %%gs:0x18,%0\n"
			"rol    $0x9,%0\n"
			: "=g" (ret)
			: "0" (addr));
	return ret;
}

#endif

/*
 * Initialize the thread library
 */
int uthread_init(int quantum_usecs)
{
	gNumOfQuantums = 1;
	timer = new Itimer(quantum_usecs, timer_handler);
	gHandler = new IdHandler(MAX_THREAD_NUM);
	// Build a "main" Thread and then call the timer's set() function
	Thread * main = new Thread(0, ORANGE);
	running = main;
	timer->set();
	return 0;
}

/** Switches the running thread to be the next in the READY queue, if exist
 * 
 */
void switchRunningThread()
{
	int ret_val;
	gNumOfQuantums++;
	threads->enqueueElement(running);
	ret_val = sigsetjmp(env[uthread_get_tid()],1);

	if (ret_val == 0)
	{
		Thread* toRun;
		toRun = threads->popElement();
		running = toRun;
		running->increaseQuantums();
		siglongjmp(env[uthread_get_tid()], 1);

	}
	
	// Check for clock errors

}

/* Create a new thread whose entry point is f */
int uthread_spawn(void (*f)(void), Priority pr)
{

	int id = gHandler->getNewId();
	if (id == -1)
	{
		return -1;
	}
	address_t sp, pc;

	sp = (address_t) gMem[id] + STACK_SIZE - sizeof(address_t);
	pc = (address_t) f;
	sigsetjmp(env[id], 1);
	(env[id]->__jmpbuf)[JB_SP] = translate_address(sp);
	(env[id]->__jmpbuf)[JB_PC] = translate_address(pc);
	sigemptyset(&env[id]->__saved_mask);
	threads->enqueueElement(new Thread(id, pr, f));
	return id;
}

/* Terminate a thread */
int uthread_terminate(int tid)
{
	gHandler->removeId(tid);
	threads->removeElement(tid);
	delete gMem[tid];
	if (tid == 0)
	{
		delete timer;
		delete gHandler;
		for (int i = 0; i < MAX_THREAD_NUM; ++i)
		{
			delete[] gMem[i];
		}
		delete[] gMem;
		exit(0);
	}
	return 0;
}

/* Suspend a thread */
int uthread_suspend(int tid)
{
	if (tid == 0)
	{
		//Error
		return FAILLURE;
	}
	// If the process is trying to block itself
	if (uthread_get_tid() == tid)
	{
		// move the main block to the READY queue
	}
	threads->block(tid);
	return 0;
}

/* Resume a thread */
int uthread_resume(int tid)
{
	threads->resume(tid);
	if(threads->isBlocked(tid) != -1)
	{
		//error
		return FAILLURE;
	}
	return 0;
}

/* Get the id of the calling thread */
int uthread_get_tid()
{

	return running->getId();
}

/* Get the total number of library quantums */
int uthread_get_total_quantums()
{
	return 0;
}

/* Get the number of thread quantums */
int uthread_get_quantums(int tid)
{
	return threads->get_quantums(tid);
}

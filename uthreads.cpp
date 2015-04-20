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
#define FAILURE -1

#define SIGEMPTY_ERR "system error: text\n"
#define SIGADD_ERR "thread library error: text\n"

using namespace std;
typedef char stack[STACK_SIZE];
stack gMem[MAX_THREAD_NUM];
sigjmp_buf env[MAX_THREAD_NUM];
int gNumOfQuantums;
Thread* gRunning;
PriorityQueue gThreads;
Itimer * gTimer;
IdHandler * gHandler;
sigset_t gSigSet;

void pauseTimer()
{
	if(sigprocmask(SIG_BLOCK, &gSigSet ,NULL) == FAILURE)
	{
		//error
		exit(1);
	}
}
void resumeTimer()
{
	if (sigprocmask(SIG_UNBLOCK, &gSigSet ,NULL) == FAILURE)
	{
		//error
		exit(1);
	}
}

/** Switches the running thread to be the next in the READY queue, if exist
 *
 */
void switchRunningThread()
{
	pauseTimer();
	gNumOfQuantums++;
	if (sigsetjmp(env[uthread_get_tid()],1) != 0)
	{
		resumeTimer();
		return;
	}
	gThreads.enqueueElement(gRunning);
	gRunning = gThreads.popElement();
	gRunning->increaseQuantums();
	cout << "switching  id:" <<uthread_get_tid() <<  endl;	////-----ERASE---------------------/
	resumeTimer();
	siglongjmp(env[uthread_get_tid()], 1);
	cout << "switching finished" <<  endl;							////-----ERASE---------------------/
	// Check for clock errors
}

/** Terminates the running thread and runs the next in the READY queue, if exist
 *
 */
void terminateRunningThread()
{
	pauseTimer();
	gNumOfQuantums++;
	gRunning = gThreads.popElement();
	gRunning->increaseQuantums();
	cout << "switching after terminating  id:" <<uthread_get_tid() <<  endl;	////-----ERASE---------------------/
	resumeTimer();
	siglongjmp(env[uthread_get_tid()], 1);
	cout << "terminating finished" <<  endl;							////-----ERASE---------------------/
	// Check for clock errors
}

/**
 * A method which handles the events that take place at the end of each time-quanta.
 */
void timer_handler(int sig)
{
	// Must pause the timer before starting this method - and resume it upon exiting this metod
	// ........enter code here......//
	pauseTimer();

	// If the running process has no "competition" over CPU time.
	if (gThreads.isQueueEmpty()) // MAKE SURE TO WRITE "isQueueEMpty()" method
	{
		cout << "\ttimer_handler and queue size is " << gThreads.isQueueEmpty() << endl;	////-----ERASE---------------------///
		gRunning->increaseQuantums();
		gNumOfQuantums++;
		resumeTimer();
		return;
	}
	resumeTimer();
	switchRunningThread();
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
//	cout << "\tinside init method with quantum = " << quantum_usecs << endl;	////-----ERASE---------------------///
	gNumOfQuantums = 1;
	gThreads = PriorityQueue();
	gTimer = new Itimer(quantum_usecs, timer_handler);
	gHandler = new IdHandler(MAX_THREAD_NUM);

//--------------------------------------------------------
	// Must activate "spawn" on Main, so it will exist as a thread with relevant stack and buffer
	// send 0 as the void(*) that spawn expect. NEED TO FIGURE OUT HOW TO DO IT.......
	Thread * main = new Thread(0, ORANGE);
	uthread_spawn(0, ORANGE);
	// I think what we have up here /\ is meaningless.
//--------------------------------------------------------

	cout << "\t\tcreated a Main thread" << endl;								////-----ERASE---------------------///
	gRunning = main;
	gRunning->increaseQuantums();
	cout << "\t\tAssigned Main thread to be the running thread" << endl;		////-----ERASE---------------------///
	gTimer->set();
	cout << "\t\ttimer was set" << endl;										////-----ERASE---------------------///
	if (sigemptyset(&gSigSet) == FAILURE)
	{
		cerr << SIGEMPTY_ERR << endl;
		exit(1);
	}
	if (sigaddset(&gSigSet,SIGVTALRM) == FAILURE)
	{
		cerr << SIGADD_ERR << endl;
		exit(1);
	}
//	cout << "\t\tNo failures in the timer run\n" << endl;							////-----ERASE---------------------///
	return 0;
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
	gThreads.enqueueElement(new Thread(id, pr));
	return id;
}

/* Terminate a thread */
int uthread_terminate(int tid)
{

	if (tid == 0)
	{
		delete gTimer;
		delete gHandler;
//		for (int i = 0; i < MAX_THREAD_NUM; ++i)
//		{
//			delete[] gMem[i];		// Bring back if we turn gMem() into an array of pointers again
//		}
//		delete *gMem;				// Bring back if we turn gMem() into an array of pointers again
		exit(0);
	}
	//Not found in queue
	if(!gThreads.removeElement(tid))
	{
		if(tid == uthread_get_tid())
		{
//			delete gMem[tid];			// Bring back if we turn gMem() into an array of pointers again
			gHandler->removeId(tid);
			terminateRunningThread();
			delete gRunning;
		}
		//error
		return FAILURE;
	}
//	delete gMem[tid];				// Bring back if we turn gMem() into an array of pointers again
	gHandler->removeId(tid);
	return 0;
}

/* Suspend a thread */
int uthread_suspend(int tid)
{
	if (tid == 0)
	{
		//Error
		return FAILURE;
	}
	// If the process is trying to block itself
	if (uthread_get_tid() == tid)
	{
		// move the main block to the READY queue
		switchRunningThread();
	}
	gThreads.block(tid);
	return 0;
}

/* Resume a thread */
int uthread_resume(int tid)
{
	gThreads.resume(tid);
	if(gThreads.isBlocked(tid) != -1)
	{
		//error
		return FAILURE;
	}
	return 0;
}

/* Get the id of the calling thread */
int uthread_get_tid()
{

	return gRunning->getId();
}

/* Get the total number of library quantums */
int uthread_get_total_quantums()
{
	return gNumOfQuantums;
}

/* Get the number of thread quantums */
int uthread_get_quantums(int tid)
{
//	cout << "\t\t\tinside uthread_get_quantums with tid = " << tid << endl;		////-----ERASE---------------------///
	if (tid == uthread_get_tid())
	{
		// If the id belongs to the running thread
//		cout << "\t\t\tid belongs to the running thread" << endl;						////-----ERASE---------------------///
		return gRunning->getNumOfQuantums();
	}

	int quantums =  gThreads.get_quantums(tid);
	if (quantums == FAILURE)
	{
		cout << "\t\t\tNO SUCH ID FOUND (id = " << tid << ")" << endl;
	}
	return quantums;
}

/**
 * A TEST FOR THOSE WHO CANNOT FAIL
 */
#define ITER_NUM 50

#include "uthreads.h"
#include "Thread.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "PriorityQueue.h"
#define ITER_NUM 50

#include "uthreads.h"
#include "Thread.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "PriorityQueue.h"

using namespace std;

void f()
{
	cout<<"In f"<<endl;
}

int main(int argc, char **argv)
{
		// --------------- Testing Thread Class ----------------- //
	cout << "\t\t\t------- Testing Thread Class --------" << endl;

				// ------- ID test ------- //
	int idNum = 0;
	for (int i= 0; i < ITER_NUM; i++)
	{
		Thread th1 = Thread(idNum, RED);
		if (th1.getId() != idNum)
		{
			cout <<	"Thread's ID don't match, got " << th1.getId() << " instead of "
					<< idNum << endl;
		}
		idNum++;
	}
				// ------- PRIORITY test ------- //
	Thread th1 =  Thread(3, RED);
	Thread th2 = Thread(1, ORANGE);
	Thread th3 = Thread(2, GREEN);
	if (th1.getPriority() != RED)
	{
		cout <<	"Thread's priority don't match, got " << th1.getPriority() << " instead of "
				<< RED << endl;
	}
	if (th2.getPriority() != ORANGE)
	{
		cout <<	"Thread's priority don't match, got " << th2.getPriority() << " instead of "
				<< ORANGE << endl;
	}
	if (th3.getPriority() != GREEN)
	{
		cout <<	"Thread's priority don't match, got " << th3.getPriority() << " instead of "
				<< GREEN << endl;
	}
	cout << "\t\t\t------- Testing priority queue --------" << endl;
	PriorityQueue q;
	q.enqueueElement(&th1);
	q.enqueueElement(&th2);
	q.enqueueElement(&th3);
	if (q.getSize() != 3)
	{
			cout <<	"Failed enqueue got " << q.getSize() << " instead of "
					<< 3 << endl;
	}
	q.popElement();
	q.popElement();
	q.popElement();
	if (q.getSize() != 0)
		{
				cout <<	"Failed pop got " << q.getSize() << " instead of "
						<< 0 << endl;
		}
	q.enqueueElement(&th1);
		q.enqueueElement(&th2);
		q.enqueueElement(&th3);
		cout << "\t\t\t------- Testing  remove --------" << endl;
		q.removeElement(1);
		q.removeElement(2);
		q.removeElement(3);
		if (q.getSize() != 0)
		{
						cout <<	"Failed remove got " << q.getSize() << " instead of "
								<< 0 << endl;
		}

	cout << "\t\t\t-------  Testing Thread management --------" << endl;
	uthread_init(1);
	for(int i= 0; i<100000; i++)
	{

	}
	cout << "\t\t\t------- here --------" << endl;
	// ------- NUM_OF_QUANTUMS test ------- //

	// need to run the Thread and so increase it's quantums.....
	// ........
	// then check if the num is correct.....

}

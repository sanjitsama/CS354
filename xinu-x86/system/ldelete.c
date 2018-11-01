/*	ldelete.c - ldelete 	*/
#include <xinu.h>

/* Lab 3: Complete this function */

syscall ldelete( 
		int32 ldes	/* lock descriptor */
	)
{
	// your implementation

	intmask mask;           /* Saved interrupt mask     */
	struct lockent *lockptr; //Pointer to locktab entry



	lockptr = &locktab[ldes];
	if(lockptr->lstate == S_FREE) {
		restore(mask);
		return SYSERR;
	}

	lockptr->lstate = S_FREE;

	//How do i access the process waiting on the lock?
		//Keep a queue of the processes waiting on the lock. 

	//If deque until macro empty is hit....
		//If not empty then ready the process.	


	resched_cntl(DEFER_START);

	int32 output;
	output = dequeue(lockptr->lqueue);

	while(output != EMPTY) {
		ready(output);
		output = dequeue(lockptr->lqueue);
	}

	resched_cntl(DEFER_STOP);
	restore(mask);
	return OK;
}



/*
-> Remove all process from the current lock - DONE
-> Make all the process removed from the lock to ready(pid) - DONE 
-> Dfer on top and below of semaphore delete function. Copy this and keep this on the alloc. Dfer does not reschedule.  
-> If the lock is below 0 or above 50 throw syserror. 
-> Check if there are process in the queue or not. 
-> resched_cntl(DEFER_START);
*/
/*	lock.c - lock */
#include <xinu.h>

/* Lab 3: Complete this function */

syscall lock(int32 ldes, int32 type, int32 lpriority) {

    //your implementation goes here
	//most of the logic can be implemented here
	//consider different cases as mentioned in the handout


	intmask mask;
	struct  lockent *lockptr; //Pointer to locktab entry
	struct	procent *prptr;		/* Ptr to process' table entry	*/


	mask = disable();
	//dobadsem stuff

	lockptr = &locktab[ldes]; 
	prptr = &proctab[currpid];

	if(lockptr->lstate == UNUSED)
	{
		lockptr->lstate = type;
		lockptr->holdingCount++;
		prptr->lockROW = type;
		restore(mask);
		return OK; //Implies that the lock is not being used and the program should move forward
	}

	if(type == READ)
	{


		if(lockptr->lstate == READ)
		{
			lockptr->holdingCount++;
			prptr->lockROW = type;
			restore(mask);
			return OK; 
		}



		else
		{
			lockptr->waitingCount++;
			prptr->lockROW = type;
			prptr->prstate = PR_WAIT;
			insert(currpid,lockptr->lqueue,lpriority);
			resched();
			restore(mask);
			return OK; 
		}		


		restore(mask);
		return OK;
	}

	if(type == WRITE)
	{
			lockptr->waitingCount++;
			prptr->lockROW = type;
			prptr->prstate = PR_WAIT;
			insert(currpid,lockptr->lqueue,lpriority);
			resched();
			restore(mask);
			return OK; 
	}

	restore(mask);
	return OK;
}



//FOOD FOR THOUGHT:
//If your lock is used but does not have a state no read no write. Acquire this lock. Lock state will be the typed that was passed to lock. Return mask and ok
//If the lock is read the num of readers must be incremented. Writers must be incremented. Return mask and ok
//have mask and restore mask. Return mask and ok
//If the lock is on type write. and the numbers of writers and readers are equal to 0. Then increment wrtiersnum by 1. Else u increment numOfReaders. 
//Insert the lqueue structure. Add the process to the queue. You should remove the process that is current. You should either suspend it. Take it off the ready list.
//If the lock is read and the current type is write. Then you add the lock to your qeueu then you either wait or suspend the work.
//If the type is read and your number of writers are 0. Then you increment the number of the readers and stop with restoring the mask and return ok. 
//If your nnum of writer is more than 0 and your lpriority is larger than the largest write process inside of your qeueue. Then increment the number of readers. 
//If your writer
//you should add an attribute inside of your procentt to know whether it is is a read or write bc a process reader can go inside of a locked writer and vice versa.  
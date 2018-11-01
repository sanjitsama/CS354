/*	linit.c	- linit	initialize lock system */
#include <xinu.h>

/* Lab 3: Complete this function */

// declare any global variables here

void linit(void) {
  
	// your implementation goes here
        // make sure to call this in initialize.c

	struct lockent *lockptr; //Pointer to locktab entry   
	int32 i;

	for (i = 0; i < NLOCKS; i++)
	{
		lockptr = &locktab[i];
		lockptr->lstate = DELETED;
		lockptr->lcount = 0;
		lockptr->lqueue = newqueue();
		lockptr->holdingCount = 0;
		lockptr->waitingCount = 0;
		lockptr->currentMaxPr = 0;
		lockptr->lockOwner = currpid;
	}
}

//has lcreate been called at all or but has lcreate been called but ldelete

//in the lock function specify if it is being used for reading or writed

//a lock exists to allow for exactly one process to write and NPROC number of process to READ. 

// Only one process can write and be assured that no other process is reading or writing.

//a process can call lock read then lock write

//make sure write processes happen atomicily. 

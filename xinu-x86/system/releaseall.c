/*	releaseall.c - releaseall	*/
#include <xinu.h>
#include <stdarg.h>

/* Lab 3: Complete this function */

syscall releaseall (int32 numlocks, ...) {
	
	//your implementation goes here

	intmask mask;			/* Saved interrupt mask		*/
	struct lockent *lockptr; //Pointer to locktab entry
	struct	procent	*prptr;		/* Ptr to process table entry	*/

	 mask = disable();
	// if (isbadsem(sem)) {
	// 	restore(mask);
	// 	return SYSERR;
	// }

	//Steps

	// va_list vl;

	// va_start(vl, numlocks);
	// int i =0;

	// int32 lockDes;
	// pid32 processToReady;
	// for(i = 0; i < numlocks; i++)
	// {
	// 	kprintf("check OK");
	// 	lockDes=va_arg(vl,int32);


	// 	//Access the queue associated with the lockdescriptor
	// 	lockptr = &locktab[lockDes];

	// 	int y = 0;

	// 	for(y=0; y < lockptr->waitingCount; y++)
	// 	{
	// 		processToReady = dequeue(lockptr->lqueue); 
	// 			kprintf("check 1");
	// 			//You need to make sure that each of the lock descriptors must correspond to a lock being held by the calling process.
	// 			if(processToReady == lockptr->lockOwner) {
	// 				kprintf("check 2");
	// 				ready(processToReady);
	// 			}
			
	// 	}


	// }



	va_list vl;

	va_start(vl, numlocks);
	int i =0;

	//int32 lockDes;
	pid32 processToReady;
	//Run some checks?

	for(i = 0; i < numlocks; i++)
	{
			//Decrease Lock Holding Count
	lockptr->holdingCount--;

		if(lockptr->holdingCount == 0) 
		{
			if(lockptr->waitingCount > 0) 
			{
				
				processToReady = dequeue(lockptr->lqueue);
				
				if(prptr->lockCounter == 0)
				{
					ready(processToReady);
				}

				if(prptr -> lockROW == READ)
				{
					processToReady = dequeue(lockptr->lqueue);
					while(prptr->lockROW == READ)
					{
						//prptr= &proctab[processToReady];
						
						if(prptr->lockCounter == 0)
						{
							ready(processToReady);
							//Decrease Waiting Increase Holding
							lockptr->holdingCount++;
							lockptr->waitingCount--;
						}

						processToReady = dequeue(lockptr->lqueue);
						prptr= &proctab[processToReady];

					}

					//reinsert the last process back into the Queue
					insert(currpid,lockptr->lqueue,lpriority);
				}


				if(prptr->lockROW == WRITE)
				{
					processToReady = dequeue(lockptr->lqueue);
				
					if(prptr->lockCounter == 0)
					{
						ready(processToReady);
					}
				}

			}
		}
	}
	

	va_end(vl);
	resched();
	restore(mask);
	return OK;
}

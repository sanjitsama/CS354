//This is a test file
#include <xinu.h>

/*------------------------------------------------------------------------
 *  getmem  -  Allocate heap storage, returning lowest word address
 *------------------------------------------------------------------------
 */
char  	*getmem_r(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr, *leftover;

	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	int originalSize = nbytes;
	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/


	prev = &memlist;
	curr = memlist.mnext;

	//My Edits
//	struct memLinkedList *heapPrev, *heapCurr;
	struct	procent	*prptr;
	prptr = &proctab[currpid];


	while (curr != NULL) {			/* Search free list	*/

		if (curr->mlength == nbytes) {	/* Block is exact match	*/
			prev->mnext = curr->mnext;
			memlist.mlength -= nbytes;

			//MY EDITS ADDING NODE TO THE END OF THE LINKED LIST
			while(prptr->heapMemList->hnext != NULL)
			{
				prptr->heapMemList = prptr->heapMemList->hnext;
			}

			prptr->heapMemList->hnext = (struct memLinkedList*) getmem(sizeof(struct memLinkedList));
			prptr->heapMemList->hnext->sizeRequested = nbytes;
			prptr->heapMemList->hnext->ogSize = originalSize;
			prptr->heapMemList->hnext->blkaddr = (char *)(curr);
			prptr->heapMemList->hnext->hnext = NULL;

			

			restore(mask);
			return (char *)(curr);

		} else if (curr->mlength > nbytes) { /* Split big block	*/
			leftover = (struct memblk *)((uint32) curr +
					nbytes);
			prev->mnext = leftover;
			leftover->mnext = curr->mnext;
			leftover->mlength = curr->mlength - nbytes;
			memlist.mlength -= nbytes;

			//MY EDITS ADDING NODE TO THE END OF THE LINKED LIST
			while(prptr->heapMemList->hnext != NULL)
			{
				prptr->heapMemList = prptr->heapMemList->hnext;
			}

			prptr->heapMemList->hnext = (struct memLinkedList*) getmem(sizeof(struct memLinkedList));
			prptr->heapMemList->hnext->sizeRequested = nbytes;
			prptr->heapMemList->hnext->ogSize = originalSize;
			prptr->heapMemList->hnext->blkaddr = (char *)(curr);
			prptr->heapMemList->hnext->hnext = NULL;

			restore(mask);
			return (char *)(curr);
		} else {			/* Move to next block	*/
			prev = curr;
			curr = curr->mnext;
		}
	}
	restore(mask);
	return (char *)SYSERR;
}

// void push(memLinkedList * head, uint32 sizeRequested, char * blkaddr)
// {
// 	memLinkedList * current = head;

// 	while(current->hnext != NULL)
// 	{
// 		current = current->hnext;
// 	}

// 	current->hnext = (struct memLinkedList*) getmem(sizeof(struct memLinkedList));
// 	current->hnext->sizeRequested = sizeRequested;
// 	current->hnext->hnext = NULL;
// }
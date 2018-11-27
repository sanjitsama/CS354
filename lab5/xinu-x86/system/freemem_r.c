/* freemem_r.c - freemem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  freemem  -  Free a memory block, returning the block to the free list
 *  and update heap memory list if any mem is deallocated by a process.
 *------------------------------------------------------------------------
 */
syscall	freemem_r(
	  char		*blkaddr,	/* Pointer to memory block	*/
	  uint32	nbytes		/* Size of block in bytes	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*next, *prev, *block;
	uint32	top;

	mask = disable();
	if ((nbytes == 0) || ((uint32) blkaddr < (uint32) minheap)
			  || ((uint32) blkaddr > (uint32) maxheap)) {
		restore(mask);
		return SYSERR;
	}

	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/
	block = (struct memblk *)blkaddr;

	prev = &memlist;			/* Walk along free list	*/
	next = memlist.mnext;
	while ((next != NULL) && (next < block)) {
		prev = next;
		next = next->mnext;
	}

	if (prev == &memlist) {		/* Compute top of previous block*/
		top = (uint32) NULL;
	} else {
		top = (uint32) prev + prev->mlength;
	}

	/* Ensure new block does not overlap previous or next blocks	*/

	if (((prev != &memlist) && (uint32) block < top)
	    || ((next != NULL)	&& (uint32) block+nbytes>(uint32)next)) {
		restore(mask);
		return SYSERR;
	}

	memlist.mlength += nbytes;

	/* Either coalesce with previous block or add to free list */

	if (top == (uint32) block) { 	/* Coalesce with previous block	*/
		prev->mlength += nbytes;
		block = prev;
	} else {			/* Link into list as new node	*/
		block->mnext = next;
		block->mlength = nbytes;
		prev->mnext = block;
	}

	/* Coalesce with next block if adjacent */

	if (((uint32) block + block->mlength) == (uint32) next) {
		block->mlength += next->mlength;
		block->mnext = next->mnext;
	}

    //MY EDITS
    struct	procent	*prptr;
    prptr = &proctab[currpid];
    struct memLinkedList *temp_node;
    //*temp_node = (struct memLinkedList *)NULL; 

    while((prptr->heapMemList->hnext->blkaddr != blkaddr) && (prptr->heapMemList->hnext->sizeRequested != nbytes))
    {
        prptr->heapMemList = prptr->heapMemList->hnext;
    }

    //EDGE CASES MUST BE HANDLED FOR LAST NODE
    if(prptr->heapMemList->hnext->hnext == NULL)
    {
        temp_node = prptr->heapMemList->hnext;
        prptr->heapMemList->hnext = NULL;
        freemem(&temp_node,temp_node->sizeRequested);
    }

    else{
        temp_node = prptr->heapMemList->hnext;
        prptr->heapMemList->hnext = prptr->heapMemList->hnext->hnext;
        freemem(&temp_node,temp_node->sizeRequested);
    }
    





	restore(mask);
	return OK;
}

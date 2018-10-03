/* system.c */

#include <xinu.h>


syscall	system(
	  pid32	 pid	/* Semaphore on which to wait  */
	)
{
	intmask mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	struct	sentry *semptr;		/* Ptr to sempahore table entry	*/

	mask = disable();
	if (isbadsem(pid)) {
		restore(mask);
		return SYSERR;
	}

	for (int i = 1; i < NPROC; i++)
	{
		prptr = &proctab[i];

		prptr->prbaseprio = 0;
	}

	restore(mask);
	return OK;
}

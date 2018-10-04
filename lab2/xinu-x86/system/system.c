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


	//Supposed to change the process of the priority given in the PID?
	
	// for (int i = 1; i < NPROC; i++)
	// {
	// 	prptr = &proctab[i];

	// 	prptr->prbaseprio = 0;
	// }

	prptr = &proctab[pid];
	prptr->prbaseprio =0;
	prptr->new_prprio = prptr->prbaseprio + (2 * prptr->prextprio) + prptr->prrecent;

	if(prptr->new_prprio > 127)
	{
		prptr->new_prprio = 127;
    }
	if(prptr->new_prprio < 0)
	{
		prptr->new_prprio = 0;
	}	


	restore(mask);
	return OK;
}

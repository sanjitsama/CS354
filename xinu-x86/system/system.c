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
	prptr->prprio = prptr->prbaseprio + (2 * prptr->prextprio) + prptr->prrecent;

	if(prptr->prrecent < 0)
	{
		prptr->prrecent = 0;
	}
	if(prptr->prprio > 127)
	{
		prptr->prprio = 127;
    }
	if(prptr->prprio < 0)
	{
		prptr->prprio = 0;
	}	

	// for(int i = 1; i < NPROC; i++){
				
		// if(proctab[pid].prstate == PR_READY) {
		// 		pid32 id = getitem(pid);
		// 		insert(id,readylist, proctab[id].prprio);
				
		// }
	// }

	resched();
	restore(mask);
	return OK;
}

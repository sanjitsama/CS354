/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */
	//kprintf("reached before dfer.ndefers in resched.c\n");
	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	//kprintf("reached before ptold in resched.c\n");
	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		if (ptold->prprio > firstkey(readylist)) {
			return;
		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
	//	kprintf("AKSJF;LJSDL;JFL;AJSD;LJF;AJSD;JFJA;SDJF;ASDFKJ;ALSDJF;JA;SDJFKJASDF;JASDJF;AJSD;F;AJSDF;AJSD;KLJ;LASJDFJALSDJFJASLK;DJFAJSD;FJA;SDJFLJAS;DF");
		// DBG_PRINT("ptold->prprio: %d\n", ptold->prprio);
		// DBG_PRINT("ptold->new_prprio: %d\n", ptold->new_prprio);
		//printf("currpid: %d\n\n\n\n\n\n\n\n",currpid );
		insert(currpid, readylist, ptold->prprio);
	}

	/* Force context switch to highest priority ready process */
	//kprintf("reached before force context switch in resched.c\n");
	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;		/* Reset time slice for process	*/
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}

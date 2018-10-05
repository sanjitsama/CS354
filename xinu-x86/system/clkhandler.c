/* clkhandler.c - clkhandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 * clkhandler - high level clock interrupt handler
 *------------------------------------------------------------------------
 */
void	clkhandler()
{
	static	uint32	count1000 = 1000;	/* Count to 1000 ms	*/

	static uint32	count10 = 10;	/* Count to 10 ms	*/

	/* Decrement the ms counter, and see if a second has passed */

	if((--count1000) <= 0) {

		/* One second has passed, so increment seconds count */

		clktime++;

		/* Reset the local ms counter for the next second */

		count1000 = 1000;
	}

	/* Handle sleeping processes if any exist */

	if(!isempty(sleepq)) {

		/* Decrement the delay for the first process on the	*/
		/*   sleep queue, and awaken if the count reaches zero	*/

		if((--queuetab[firstid(sleepq)].qkey) <= 0) {
			wakeup();
		}
	}

	/* Decrement the preemption counter, and reschedule when the */
	/*   remaining time reaches zero			     */

	if((--preempt) <= 0) {
		//preempt = QUANTUM;
		preempt = proctab[currpid].prquantum;
		resched();
	}

	//Increment prrecent
	if((--count10) <= 0)
	{
		if(currpid!=0)
		{
			proctab[currpid].prrecent = proctab[currpid].prrecent + 1 ;	
		}
		

		count10 = 10;

		

		//This below is not changing test2 by much if
		// if(currpid != 0)
		// {
		// proctab[currpid].prprio = proctab[currpid].prbaseprio + (2 * proctab[currpid].prextprio) + proctab[currpid].prrecent;	
		// }


		//prptr->prprio = prptr->prbaseprio + (2 * prptr->prextprio) + prptr->prrecent;
		//proctab[currpid].prprio = proctab[currpid].prbaseprio + (2 * proctab[currpid].prextprio) + proctab[currpid].prrecent;
		
	}

	//resched();

	
}

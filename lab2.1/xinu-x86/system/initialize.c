/* initialize.c - nulluser, sysinit, sizmem */ //LAB2.1 OK

/* Handle system initialization and become the null process */

#include <xinu.h>
#include <string.h>

extern	void	start(void);	/* Start of Xinu code			*/
extern	void	*_end;		/* End of Xinu code			*/

/* Function prototypes */

extern	void main(void);	/* Main is the first process created	*/
extern	void xdone(void);	/* System "shutdown" procedure		*/
static	void sysinit(); 	/* Internal system initialization	*/
extern	void meminit(void);	/* Initializes the free memory list	*/

/* Declarations of major kernel variables */

struct	procent	proctab[NPROC];	/* Process table			*/
struct	sentry	semtab[NSEM];	/* Semaphore table			*/
struct	memblk	memlist;	/* List of free memory blocks		*/

/* Active system status */

int	prcount;		/* Total number of live processes	*/
pid32	currpid;		/* ID of currently executing process	*/

/*------------------------------------------------------------------------
 * nulluser - initialize the system and become the null process
 *
 * Note: execution begins here after the C run-time environment has been
 * established.  Interrupts are initially DISABLED, and must eventually
 * be enabled explicitly.  The code turns itself into the null process
 * after initialization.  Because it must always remain ready to execute,
 * the null process cannot execute code that might cause it to be
 * suspended, wait for a semaphore, put to sleep, or exit.  In
 * particular, the code must not perform I/O except for polled versions
 * such as kprintf.
 *------------------------------------------------------------------------
 */

void daemonProcess()
{

	//Call Disable 
	intmask	mask;


	struct	procent	*prptr;		/* Ptr to process table entry	*/

	int32 aveload = 0;


	//Declare Array....what happens when the limit is reached? 
	int instaLoadHistory[60] = {0};
	
	//Number Of Items in the process table which are "ready"
	int instaLoad = 0;

	//Value of all the added up 
	int runningTotal = 0;


	//value to know how many values are inside of the instaLoadHistory array
	int counter = 0;


	int temp = 0;



	// (a) Recalculate aveload.
	// (b) Recalculate prrecent for each process to forget old usage as discussed above. 
	// (c) Recalculate prprio for each process.
	// (d) Recalculate prquantum for each process.
	// (e) Reorder the ready list to reflect the new priorities.

	while(1)
	{

		mask = disable();
		



		instaLoad = 0;
	  
	// (a) Recalculate aveload.

			//loop through ready process table and find the number in the PR_TABLE with the ready state
			for (int i = 1; i < NPROC; i++)
			{

				prptr = &proctab[i];

				if(prptr->prstate == PR_READY)
				{
					instaLoad++; //InstaneousLoad??? When I'm done with the foor loop
				}


				

				// if(counter == 59)
				// {
				// 	counter = 0;
				// }
			}

			temp = instaLoadHistory[counter%60];
			instaLoadHistory[counter%60] = instaLoad; 
			counter++;

			//
			

			/* Loop through the instaLoadHistory to add up all the values */
			// for (int i = 0; i < counter; i++)
			// {
			// 	runningTotal += instaLoadHistory[counter];
			// }

			runningTotal = runningTotal - (temp - instaLoad);

			//kprintf("runningTotal: %d\n",runningTotal);
			//kprintf("counter: %d\n",counter);

			if(counter>60)
			{
				aveload = runningTotal/60;
			}

			else{
				aveload = runningTotal/counter;
						
			}


			for (int i = 1; i < NPROC; i++)
			{

			// (b) Recalculate prrecent for each process to forget old usage as discussed above. 
			//prrecent = (aveload * prrecent) / (( 2 * aveload ) + 1) + prextprio

				prptr = &proctab[i];
				prptr->prrecent = (aveload * prptr->prrecent) / ((2 * aveload )  + 1) + prptr->prextprio;



			// (c) Recalculate prprio for each process.
			//prprio = prbaseprio + ( 2 * prextprio ) + prrecent
				prptr->new_prprio = prptr->prbaseprio + (2 * prptr->prextprio) + prptr->prrecent;

				if(prptr->new_prprio > 127)
				{
					prptr->new_prprio = 127;
				}

				if(prptr->new_prprio < 0)
				{
					prptr->new_prprio = 0;
				}


			// (d) Recalculate prquantum for each process.
			//prquantum = QUANTUM + prrecent
				prptr->prquantum = QUANTUM + prptr->prrecent;

				if(prptr->new_prprio > 0)
				{
				//	DBG_PRINT("THE new_prprio IS > 0: %d\n", prptr->new_prprio);
				}

			}


			DBG_PRINT("*********************** START ***********************\n",NULL);
			DBG_PRINT("prptr->new_prprio = prptr->prbaseprio + (2 * prptr->prextprio) + prptr->prrecent; \n", NULL);
			DBG_PRINT("prptr->prbaseprio: %d\n", prptr->prbaseprio);
			DBG_PRINT("(2 * prptr->prextprio): %d\n", prptr->prextprio * 2);
			DBG_PRINT("prptr->prrecent: %d\n", prptr->prrecent);
			DBG_PRINT("new_prprio: %d\n", prptr->new_prprio);
	

			//DBG_PRINT("prrecent: %d\n",prptr->prrecent);
			DBG_PRINT("temp: %d\n",temp);
			DBG_PRINT("counter: %d\n", counter);
			DBG_PRINT("instaLoad: %d\n",instaLoad);
			DBG_PRINT("runningTotal: %d\n",runningTotal);	
			DBG_PRINT("aveload: %d\n",aveload);	
			DBG_PRINT("*********************** END ***********************\n",NULL);

			
			for(int i = 1; i < NPROC; i++)
			{
				if(proctab[i].prstate == PR_READY) {
					pid32 id = getitem(i);
					insert(id,readylist, proctab[id], prprio);
				}

			}

		restore(mask);
			
		sleepms(1000);
		
	}	
}



void	nulluser()
{	
	struct	memblk	*memptr;	/* Ptr to memory block		*/
	uint32	free_mem;		/* Total amount of free memory	*/
	
	/* Initialize the system */
		
	sysinit();

	kprintf("\n\r%s\n\n\r", VERSION);
	
	/* Output Xinu memory layout */
	free_mem = 0;
	for (memptr = memlist.mnext; memptr != NULL;
						memptr = memptr->mnext) {
		free_mem += memptr->mlength;
	}
	kprintf("%10d bytes of free memory.  Free list:\n", free_mem);
	for (memptr=memlist.mnext; memptr!=NULL;memptr = memptr->mnext) {
	    kprintf("           [0x%08X to 0x%08X]\r\n",
		(uint32)memptr, ((uint32)memptr) + memptr->mlength - 1);
	}

	kprintf("%10d bytes of Xinu code.\n",
		(uint32)&etext - (uint32)&text);
	kprintf("           [0x%08X to 0x%08X]\n",
		(uint32)&text, (uint32)&etext - 1);
	kprintf("%10d bytes of data.\n",
		(uint32)&ebss - (uint32)&data);
	kprintf("           [0x%08X to 0x%08X]\n\n",
		(uint32)&data, (uint32)&ebss - 1);

	/* Enable interrupts */



	//Create Daemeon Process
	//daemonProcess();


	   
	pid32 dProcess = create((void *)daemonProcess, INITSTK,  -20, "SDaemon", 0,
           NULL);


	DBG_PRINT("dProcess: %d\n", dProcess);

	resume(dProcess);

	enable();

	/* Create a process to execute function main() */


	pid32 mainPid = create((void *)main, INITSTK, 0, "Main process", 0,
           NULL);

	DBG_PRINT("mProcess: %d\n", mainPid);
	resume (mainPid);

	/* Become the Null process (i.e., guarantee that the CPU has	*/
	/*  something to run when no other process is ready to execute)	*/



	while (TRUE) {
		;		/* Do nothing */
	}

}

/*------------------------------------------------------------------------
 *
 * sysinit  -  Initialize all Xinu data structures and devices
 *
 *------------------------------------------------------------------------
 */
static	void	sysinit()
{
	int32	i;
	struct	procent	*prptr;		/* Ptr to process table entry	*/
	struct	sentry	*semptr;	/* Ptr to semaphore table entry	*/

	/* Platform Specific Initialization */

	platinit();

	/* Initialize the interrupt vectors */

	initevec();
	
	/* Initialize free memory list */
	
	meminit();

	/* Initialize system variables */

	/* Count the Null process as the first process in the system */

	prcount = 1;

	/* Scheduling is not currently blocked */

	Defer.ndefers = 0;

	/* Initialize process table entries free */

	for (i = 0; i < NPROC; i++) {
		prptr = &proctab[i];
		prptr->prstate = PR_FREE;
		prptr->prname[0] = NULLCH;
		prptr->prstkbase = NULL;
		prptr->prprio = 0;

		//TODO MY EDITS
		// prptr->prprio = 0;
	}

	/* Initialize the Null process entry */	

	prptr = &proctab[NULLPROC];
	prptr->prstate = PR_CURR;
	prptr->prprio = 128; //TODO THIS SHOULD BE CHANGED TO 128
	strncpy(prptr->prname, "prnull", 7);
	prptr->prstkbase = getstk(NULLSTK);
	prptr->prstklen = NULLSTK;
	prptr->prstkptr = 0;
	currpid = NULLPROC;


	
	/* Initialize semaphores */

	for (i = 0; i < NSEM; i++) {
		semptr = &semtab[i];
		semptr->sstate = S_FREE;
		semptr->scount = 0;
		semptr->squeue = newqueue();
	}

	/* Initialize buffer pools */

	bufinit();

	/* Create a ready list for processes */

	readylist = newqueue();

	/* Initialize the real time clock */

	clkinit();

	for (i = 0; i < NDEVS; i++) {
		init(i);
	}
	return;
}
//this hsould make

int32	stop(char *s)
{
	kprintf("%s\n", s);
	kprintf("looping... press reset\n");
	while(1)
		/* Empty */;
}

int32	delay(int n)
{
	DELAY(n);
	return OK;
}

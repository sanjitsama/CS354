/* process.h - isbadpid */

/* Maximum number of processes in the system */

#ifndef NPROC
#define	NPROC		8
#endif		

/* Process state constants */

#define	PR_FREE		0	/* Process table entry is unused	*/
#define	PR_CURR		1	/* Process is currently running		*/
#define	PR_READY	2	/* Process is on ready queue		*/
#define	PR_RECV		3	/* Process waiting for message		*/
#define	PR_SLEEP	4	/* Process is sleeping			*/
#define	PR_SUSP		5	/* Process is suspended			*/
#define	PR_WAIT		6	/* Process is on semaphore queue	*/
#define	PR_RECTIM	7	/* Process is receiving with timeout	*/

/* Miscellaneous process definitions */

#define	PNMLEN		16	/* Length of process "name"		*/
#define	NULLPROC	0	/* ID of the null process		*/

/* Process initialization constants */

#define	INITSTK		65536	/* Initial process stack size		*/
#define	INITPRIO	20	/* Initial process priority		*/
#define	INITRET		userret	/* Address to which process returns	*/

/* Inline code to check process ID (assumes interrupts are disabled)	*/

#define	isbadpid(x)	( ((pid32)(x) < 0) || \
			  ((pid32)(x) >= NPROC) || \
			  (proctab[(x)].prstate == PR_FREE))

/* Number of device descriptors a process can have open */

#define NDESC		5	/* must be odd to make procent 4N bytes	*/

/* Definition of the process table (multiple of 32 bits) */

struct procent {		/* Entry in the process table		*/
	uint16	prstate;	/* Process state: PR_CURR, etc.		*/
	pri16	prprio;		/* Process priority			*/
	char	*prstkptr;	/* Saved stack pointer			*/
	char	*prstkbase;	/* Base of run time stack		*/
	uint32	prstklen;	/* Stack length in bytes		*/
	char	prname[PNMLEN];	/* Process name				*/
	uint32	prsem;		/* Semaphore on which process waits	*/
	pid32	prparent;	/* ID of the creating process		*/
	umsg32	prmsg;		/* Message sent to this process		*/
	bool8	prhasmsg;	/* Nonzero iff msg is valid		*/
	int16	prdesc[NDESC];	/* Device descriptors for process	*/



    /* My Implementation
	1. You can start with the daemon, and instead of updating prprio, just print the should-be new value 
	(and other related variables). This part involves adding the daemon process and running it at a high priority
	(according to the old meaning of priorities), a couple changes in process.h and create.c for 
	 the new variables and the initialization for them.
     */
	
	//Need a variable for testing priprio
	//prprio is limited to a maximum value of 127, and a minimum value of 0.
	pri16 new_prprio; 

	//prbaseprio = 0 if this is a system process or 50 if this is a user process. Will be updated 
	int32 prbaseprio;

	/* prextprio is the externally assigned priority of the process. 
	This external priority is the priority specified by the user as an argument to create. 
	This value should be limited to the range MAXEXTPRIO (set to -20) to MINEXTPRIO (set to 20). 
	(Again, a low value signifies a high priority.) 
	If a value outside that range is passed to create or chprio, update it to the closest value that falls within the range.
	*/

	int32 prextprio;



	//prrecent provides an estimate of the recent CPU utilization of the process. This value is incremented by 1 every 10 ms a process executes.
	int32 prrecent;



	//aveload is a moving load average over the past 60 seconds.
	// int32 aveload;

	int32 prquantum;

};

/* Marker for the top of a process stack (used to help detect overflow)	*/
#define	STACKMAGIC	0x0A0AAAA9

extern	struct	procent proctab[];
extern	int32	prcount;	/* Currently active processes		*/
extern	pid32	currpid;	/* Currently executing process		*/

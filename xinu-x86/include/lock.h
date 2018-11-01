/*  lock.h	*/


// declare variables, #defines, 
// #define DELETE 10
// #define READ 11
// #define WRITE 12

// #define L_FREE 0
// #define L_USED 1


#ifndef NLOCKS
#define NLOCKS 50
#endif
#ifndef UNUSED
#define UNUSED 2 
#endif
#ifndef READ 
#define READ 0
#define READL READ
#endif
#ifndef WRITE 
#define WRITE 1
#define WRITEL WRITE
#endif
#ifndef LFREE
#define LFREE (-4)
#endif


struct	lockent {

	//Status if in use 

	int16 lstate;  /* Free or Used */	

	int32 lcount; /* Count for the lock */

	qid16 lqueue; /*Queue of the process that are waiting on this lock*/ 
 
	pid32 lockOwner;
	int32 waitingCount;
	int32 holdingCount;
	int16 currentMaxPr; 
};

/* Lab 3 lock table */

//extern struct lockent locktab[NLOCKS];

extern struct lockent locktab[NLOCKS];





/////////////////////////// NOTES /////////////////////////////////
//need to know if the particular lock is being used or not

//Look at Create.c specifically getpid function. 
	//What's a good way to find the lock id? 

//Which process have the current lock and reason? (read or write) Check Piazza for a bit factor or a boolean array. T if pid has lock (ex)

//Only one single writer...any num of readers... find a way to have type for the lock struct. 

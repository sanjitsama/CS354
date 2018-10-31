/*  lock.h	*/


// declare variables, #defines, 
#define DELETE 10
#define READ 11
#define WRITE 12

#define L_FREE 0
#define L_USED 1

struct	lockent {

	//Status if in use 
	
	int32 lstatus; 	/* delete, read, or write */ 

	int32 lstate;  /* Free or Used */	

	int32 lcount; /* Count for the lock */

	qid16 lqueue; /*Queue of the process that are waiting??? DO I NEED THIS? */ 
 


};

/* Lab 3 lock table */

//extern struct lockent locktab[NLOCKS];

struct lockent locktab[NLOCKS];





/////////////////////////// NOTES /////////////////////////////////
//need to know if the particular lock is being used or not

//Look at Create.c specifically getpid function. 
	//What's a good way to find the lock id? 

//Which process have the current lock and reason? (read or write) Check Piazza for a bit factor or a boolean array. T if pid has lock (ex)

//Only one single writer...any num of readers... find a way to have type for the lock struct. 

/*	lcreate.c - lcreate	*/
#include <xinu.h>

/* Lab 3: Complete this function */

int32 lcreate() {

  // your implementation
	  struct lockent *lockptr; //Pointer to locktab entry
	  static  int32   nextDes = 0;    /* Next lock descriptor to try  */
          int32   lockDes;            /* Lock Descriptor to return   */
          int32   i;          /* Iterate through # entries    */


	for(i = 0; i < NLOCKS; i++)
	{
		//lockptr = &locktab[i];
		lockDes = nextDes++;
		
		if(nextDes > NLOCKS) 
		{
		 	nextDes = 0;	
		}

		lockptr = &locktab[lockDes];

		if(lockptr->lstate == S_FREE) 
		{
			lockptr->lstate == S_USED;
			return lockDes;
		}

}	

  return SYSERR;
}

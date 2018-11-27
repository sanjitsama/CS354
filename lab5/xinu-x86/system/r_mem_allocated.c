//r_mem_allocated
#include <xinu.h>

syscall r_mem_allocated(
        pid32       pid,
        uint32     *buf
    )
   
{
    int32 totalMem = 0;
    struct	procent	*prptr;
    prptr = &proctab[pid];

    while(prptr->heapMemList->hnext != NULL){
        totalMem = totalMem + prptr->heapMemList->hnext->ogSize;
        prptr->heapMemList=prptr->heapMemList->hnext;
    }

    *buf = totalMem;
    kprintf("R_MEM_ALLOCATED: %d: \n",totalMem);
    return OK;
}
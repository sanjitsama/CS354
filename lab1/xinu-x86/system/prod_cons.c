/*------------------------------------------------------------------------
 *  prod_cons.c  - implementation of the producer/consumer simulation
 *------------------------------------------------------------------------
 */

#include <xinu.h>
#include "prod_cons.h"

/* the global shared buffer that contains the data */
uint8 buffer[BUFFERSIZE];	

/* array to keep track of number of items produced by each producer */
int32 prod_item_count[NPRODUCERS]={0};	

/* 2D array to keep track of number of items consumed for each producer-consumer pair */
int32 cons_item_count[NCONSUMERS][NPRODUCERS]={{0}};  


/* insert other global variables here */
int pindex = 0;
int cindex = 0;

int y;

int pidProducers[NPRODUCERS];
int pidConsumers[NCONSUMERS];

sid32 produced;
sid32 consumed;
sid32 locked;

/*------------------------------------------------------------------------
 * Function used by producer to insert item into buffer
 *
 * Item inserted into buffer with locks. Increments producer index.
 * Resets producer index if buffersize is reached. Mimics a circular buffer
 *------------------------------------------------------------------------
 */
void insert_buffer(uint8 item)
{
  wait(locked); //0

  if(pindex==BUFFERSIZE){
  	// kprintf("P INDEX HAS BEEN RESET");
	pindex = 0;
  } 

  buffer[pindex] = item; //Hold Producer Tags
//  kprintf("this is insert_buffer\n");
    // kprintf("Insert Buffer of buffer[pindex]: %c and pindex: %c\n",(char) buffer[pindex],pindex );
//  kprintf("above is buffer[index]\n");
  // kprintf("item: %d\n",item);
  // kprintf("buffer[%d]: %c\n", pindex,(char)buffer[pindex]);
  pindex++;
  signal(locked);
} /* insert_buffer */

/*------------------------------------------------------------------------
 * Function used by consumer to remove an item from buffer
 * Removes item from buffer. Replaces the index with NULL allowing for 
 * the slot to be inserted with something else. 
 * Consumer index is reset to 0 if buffersize is met. Mimics cirucular buffer
 *------------------------------------------------------------------------
 */
uint8 remove_buffer()
{ 

  wait(locked);
  if(cindex==BUFFERSIZE){
  	// kprintf("C INDEX HAS BEEN RESET");
	cindex = 0;
  } 
//  kprintf("this is remove_buffer\n");	
  uint8 item = buffer[cindex];
  // kprintf("Remove Buffer item: %c\n", (char) item );
  buffer[cindex] = NULL;  
  // kprintf("Remove Buffer buffer[%d]: %c\n",cindex,(char)buffer[cindex]);
  cindex++;
  signal(locked);

  return item;
}
 /* remove_buffer */

/*------------------------------------------------------------------------
 * Function to implement producer process. It has the following parameters:
 * producer_tag: a label that consists of a single character used to 
 * label items produced and in the output is used to identify the producer.
 * prod_count: An integer that specifies the number of items to 
 * insert during each time interval.
 * prod_sleep_time: An integer that specifies the number of milliseconds 
 * to delay between each interval.
 *
 * Calls insert_buffer placing the given producer tag from parameter.
 * Increments counter to keep track of number produced. 
 *------------------------------------------------------------------------
 */
void producer(uint8 producer_tag, int32 prod_count, int32 prod_sleep_time)
{
int z;
int i;
	// kprintf("This is producer tag: %c\n", producer_tag);


//kprintf("prod_count: %d\n",prod_count);
while(1){
	for(i=0; i< prod_count; i++) {	
			wait(consumed);
			insert_buffer(producer_tag);
		//	kprintf("made it");
		for(z=0; z < NPRODUCERS; z++) {

			// kprintf("(producer_tags[%d]): %d == %d (producer_tag): \n",z, producer_tags[z],producer_tag);
			if(producer_tags[z] == producer_tag) {
				prod_item_count[z]++;
				// kprintf("prod_item_count[%d] = %d\n",z,prod_item_count[z]);
				// kprintf("producer -> producer_tag[z]: %c and prod_item_count[z]: %c", producer_tags[z], prod_item_count[z]);
			}
		}	
		signal(produced);		
	}
			
  
			sleepms(prod_sleep_time);
}
	
} /* producer */

/*------------------------------------------------------------------------
 * Function to implement consumer process. It has the following parameters:
 * consumer_tag: a label that consists of a single character and is used in 
 * the output to identify the consumer.
 * cons_count: An integer that specifies the number of items to extract 
 * during each time interval.
 * cons_sleep_time: An integer that specifies the number of milliseconds 
 * to delay between each interval.
 *
 *  Iterates through 2D array to find the removed buffer tag then
 *  increment counter. 
 *------------------------------------------------------------------------
 */
void consumer(uint8 consumer_tag, int32 cons_count, int32 cons_sleep_time)
{
int zz;
int zzz;
uint8 ans;
	while(1){
		for(y=0; y<cons_count; y++)
		{
		
			wait(produced);
			//kprintf("cons_count: %d\n",cons_count);
			ans = remove_buffer(consumer_tag);
			
				for(zz=0; zz < NCONSUMERS; zz++) {
					if(consumer_tags[zz] == consumer_tag) {
						for(zzz=0; zzz< NPRODUCERS; zzz++) {
							if(producer_tags[zzz] == ans) {
								cons_item_count[zz][zzz]++;
							//kprintf("cons_item_count[%d][%d]:%d\n",zz,zzz,cons_item_count[zz][zzz]);
							}
						}
				 	}    	
				} 
			signal(consumed);
		}

		sleepms(cons_sleep_time);
	
}
			
} /* consumer */


/*------------------------------------------------------------------------
 *  start_prod_con  -  Initializes and starts the producer/consumer
 *                           simulation
 * Creates and resumes processes and stored pid in array
 *------------------------------------------------------------------------
 */
void start_prod_con(void)
{
	consumed = semcreate(1);
	produced = semcreate(0);
	locked = semcreate(1);
	int a;
	int b;

	//Unique priority value test
	//int increment = 20;

	for(a = 0; a < NPRODUCERS ; a++) {

		pidProducers[a] = create(producer, 1024, 20, "prod", 3, producer_tags[a], producer_counts[a], producer_sleep_times[a]);
		resume(pidProducers[a]);   
		//resume( create(producer, 1024, 20, "cons", 2, consumed, produced) );
		//resume( create(producer, 1024, 20, "cons", 0) );		
		
	}

	for (b = 0; b < NCONSUMERS ; b++) {

		pidConsumers[b] = create(consumer, 1024, 20, "cons", 3, consumer_tags[b], consumer_counts[b],consumer_sleep_times[b] );
		resume( pidConsumers[b]);
		increment = increment + 10;
	}

} /* start_prod_con */

/*------------------------------------------------------------------------
 *  stop_prod_con  -  Stops the currently executing producer/consumer
 *                           simulation
 *------------------------------------------------------------------------
 */
void stop_prod_con(void)
{
	// kprintf("this is stop_prod_con\n");
//	semdelete(consumed);
//	semdelete(produced);
	//kill(getpid());  

	int a;
	int b;

	for (a = 0; a < NPRODUCERS; a++)
	{
		kill(pidProducers[a]);
	}


	for (b = 0; b < NCONSUMERS; b++)
	{
		kill(pidConsumers[b]);
	}


} /* stop_prod_con */

/*------------------------------------------------------------------------
 *  print_report  -  Prints the final report for the producer/consumer
 *                           simulation
 *------------------------------------------------------------------------
 */
void print_report(void)
{
int print_count;
	
	//kprintf("this is print_report\n");
	for(print_count = 0; print_count < NPRODUCERS; print_count++) 
	{
		kprintf("Producer %c: created %d items\n",producer_tags[print_count],prod_item_count[print_count]);
	}  	

	kprintf("\n\n");

	// kprintf("Consumer a: deleted %d items from producer A, %d items from producer B, and %d items from producer C \n",cons_item_count[0][0],cons_item_count[0][1],cons_item_count[0][2]);
	// kprintf("Consumer b: deleted %d items from producer A, %d items from producer B, and %d items from producer C \n",cons_item_count[1][0],cons_item_count[1][1],cons_item_count[1][2]);
	// kprintf("Consumer c: deleted %d items from producer A, %d items from producer B, and %d items from producer C \n",cons_item_count[2][0],cons_item_count[2][1],cons_item_count[2][2]);

	// kprintf("V2 \n\n\n");

	for (int i = 0; i < NCONSUMERS; i++)
	{
		kprintf("Consumer %c: deleted ",consumer_tags[i]);

		for (int y = 0; y < NPRODUCERS; y++)
		{
			kprintf("%d items from producer %c",cons_item_count[i][y],producer_tags[y]);

			if(y != (NPRODUCERS-1))
			{
				kprintf(", ");
			}
		}

		kprintf("\n");
	}

	kprintf("\n\n");

int counter;
	kprintf("The shared buffer contains:");
	for(int i = 0; i < NPRODUCERS; i++)
	{
		counter = 0;
		for (print_count = 0; print_count < BUFFERSIZE; print_count++)
		{
			if(producer_tags[i] == buffer[print_count])
			{
				counter++;
			}
		}

		kprintf(" %d items from producer %c",counter, producer_tags[i]);

		if(i != (NPRODUCERS-1))
		{
			kprintf(",");
		}
	}
	
//kprintf("buffer[%d]: %d\n", print_count,buffer[print_count]);


	//kprintf("The shared buffer contains: %d items from producer A, %d items from producer B, and %d items from producer C",);

	//for(print_count = 0; print_count < NCONSUMERS; print_count++) 
	//{
		//kprintf("Consumer %c: deleted",consumer_tags[print_count],cons_item_count[print_count][print]);
	//}
} /* print_report */


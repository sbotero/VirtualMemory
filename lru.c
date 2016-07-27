#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"
#include "sim.h"

extern int debug;

extern struct frame *coremap;

// simulated timestamp of OS
static int timestamp;

/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int lru_evict() {
    
	int i = 0;
	int earliest_time = coremap[i].time;
	int evicted_frame = 0;

	// Find the frame with the earliest time
	for (i = 0; i < memsize; i++) {

		if (coremap[i].time < earliest_time) {
			
			evicted_frame = i;
		}
		
	}

	return evicted_frame;
}

/* This function is called on each access to a page to update any information
 * needed by the lru algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void lru_ref(pgtbl_entry_t *p) {

	timestamp++;

	// Update timestamp info when referenced
	coremap[p->frame >> PAGE_SHIFT].time = timestamp;

	return;
}


/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void lru_init() {

	int i;

	// initialize timestamp to 0
	for (i = 0; i < memsize; i++) {

		coremap[i].time = 0;
	}

	timestamp = 1;
}

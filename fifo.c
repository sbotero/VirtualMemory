#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"
#include "sim.h"

extern int debug;

extern struct frame *coremap;

static int evict_frame;

/* Page to evict is chosen using the fifo algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int fifo_evict() {

	evict_frame++;

	// Sequential eviction
	if (evict_frame >= memsize) {

		evict_frame -= memsize;	
	}

	return evict_frame;
}

/* This function is called on each access to a page to update any information
 * needed by the fifo algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void fifo_ref(pgtbl_entry_t *p) {
	
}

/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void fifo_init() {

	evict_frame = -1;
}

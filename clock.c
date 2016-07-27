#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"
#include "sim.h"

extern int debug;

extern struct frame *coremap;

static int clock_frame_location;

/* Page to evict is chosen using the clock algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int clock_evict() {

    // Check reference bit is on
    while (coremap[clock_frame_location].pte->frame & PG_REF) {

    	// Turn off the reference bit
        coremap[clock_frame_location].pte->frame &= ~PG_REF;

        // Move the clock handle to the next frame
        clock_frame_location++;

        // If the clock handle was at the last frame,
        // point to the first frame in the clock
        if (clock_frame_location >= memsize) {
        	
            clock_frame_location -= memsize;
        }
    }

    // the frame to be evicted
    int return_val = clock_frame_location;

    // after page is evicted,
    // move the clock handle to next frmae
    clock_frame_location++;

    if (clock_frame_location >= memsize) {

        clock_frame_location -= memsize;
    }

    return return_val;
}

/* This function is called on each access to a page to update any information
 * needed by the clock algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void clock_ref(pgtbl_entry_t *p) {

    p->frame |= PG_REF;
}

/* Initialize any data structures needed for this replacement
 * algorithm. 
 */
void clock_init() {

    clock_frame_location = 0;
}
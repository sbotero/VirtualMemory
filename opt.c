#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"
#include "sim.h"

#define ERROR 1

extern int debug;

extern struct frame *coremap;

// Tracefile size
int tf_size;

unsigned long * v_addrs;

// Index for virtual addresses v_addrs 
int v_addrs_idx = 0;

/* Page to evict is chosen using the optimal (aka MIN) algorithm. 
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int opt_evict() {


	int i, j, last, evict_frame;
	last = evict_frame = 0;

	for (i = 0; i < memsize; i++) {

		for (j = v_addrs_idx; j < tf_size; j++) {

			// Check if vaddr referenced later in trace
			if (coremap[i].vaddr == v_addrs[j]) {

				// If referenced again, check if it's later than last vaddr reference found
				if (j >= last) {

					last = j;
					evict_frame = i; 

				}

				break;
			// If vaddr is not referenced again, return current frame
			} else if (j == tf_size - 1) {

				return i;
			}
		}

	}


	return evict_frame;
}

/* This function is called on each access to a page to update any information
 * needed by the opt algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void opt_ref(pgtbl_entry_t *p) {


	int frame = p -> frame >> PAGE_SHIFT;

	// Update vaddr info
	coremap[frame].vaddr = v_addrs[v_addrs_idx];

	v_addrs_idx++;

	return;
}

/* Initializes any data structures needed for this
 * replacement algorithm.
 */
void opt_init() {

	// Index for virtual addresses v_addrs 
	int i = 0;

	char type;
	tf_size = 0;
	addr_t vaddr = 0;

	// Tracefile pointer
	FILE * tf_ptr;

	if (!(tf_ptr = fopen(tracefile, "r"))) {
		fprintf(stderr, "Open tracefile error:\n");
		exit(ERROR);
	}

	char buffer[MAXLINE];

	// Get size of tracefile
	while (fgets(buffer, MAXLINE, tf_ptr)) {
		if (buffer[0] != '=') {

			tf_size++;
		}
	}

	fclose(tf_ptr);

	// Initialize all vaddr values to 0
	for (i = 0; i < memsize; i++) {
	
		coremap[i].vaddr = 0;
	}

	if (!(tf_ptr = fopen(tracefile, "r"))) {
		fprintf(stderr, "Open tracefile error:\n");
		exit(ERROR);
	}

	// Save virtual addresses from tracefile in v_addrs
	v_addrs = malloc(tf_size * sizeof(unsigned long));

	i = 0;

	while (fgets(buffer, MAXLINE, tf_ptr)) {
		if (buffer[0] != '=') {

			sscanf(buffer, "%c %lx", &type, &vaddr);
			v_addrs[i] = vaddr;
			i++;
		}
	}

	fclose(tf_ptr);

}


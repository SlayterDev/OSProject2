#include <unistd.h>
#include "blk.h"

#define NALLOC 	  4096 // Minimum # of bytes to request from the phys manager
#define PAGE_SIZE 4096 // In other words, a page

#define MEM_DEBUG 0

static Header base; 		 // Used to get started on first call
static Header *freep = NULL; // Pointer to free list

void myFree(void *ap) {
	Header *bp, *p;

	bp = (Header *)ap - 1; // Point to actual header

	for (p = freep; !(bp > p && bp < p->blk.next); p = p->blk.next) {
		if (p >= p->blk.next && (bp > p || bp < p->blk.next))
			break; // Freed block at start or end of arena
	}

	if (bp + bp->blk.size == p->blk.next) { // Join to upper
		bp->blk.size += p->blk.next->blk.size;
		bp->blk.next = p->blk.next->blk.next;
	} else {
		bp->blk.next = p->blk.next;
	}

	if (p + p->blk.size == bp) { // Join to lower
		p->blk.size += bp->blk.size;
		p->blk.next = bp->blk.next;
	} else {
		p->blk.next = bp;
	}

	freep = p;
}

static Header *morecore(unsigned int nu) {
	char *cp;
	Header *up;

	if (nu < NALLOC)
		nu = NALLOC;

	cp = (char *)sbrk(nu * sizeof(Header)); // Grab moar memory
	if (cp == (char *)-1) // No memory left
		return NULL;

	up = (Header *)cp;
	up->blk.size = nu;
	myFree((void *)(up+1));
	return freep;
}

void *myMalloc(unsigned int nbytes) {
	Header *p, *prevp;
	unsigned int nunits;

	nunits = (nbytes + sizeof(Header)-1) / sizeof(Header) + 1;
	if ((prevp = freep) == NULL) { // Free List doesn't exist yet
		base.blk.next = freep = prevp = &base;
		base.blk.size = 0;
	}

	for (p = prevp->blk.next; ; prevp = p, p = p->blk.next) {
		if (p->blk.size >= nunits) { 	 // Big enough space
			if (p->blk.size == nunits) { // Exactly big enough
				prevp->blk.next = p->blk.next;
			} else { // return tail end of free block
				p->blk.size -= nunits;
				p += p->blk.size;
				p->blk.size = nunits;
			}
			freep = prevp;
			
			return (void *)(p+1);
		}
		if (p == freep) { // Wrapped around the list
			if ((p = morecore(nunits)) == NULL)
				return NULL; // Out of memory
		}
	}

	return NULL; // Something bad happened
}
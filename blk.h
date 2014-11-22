typedef long Align; // To align our blocks

union header {
	struct {
		union header *next; // Pointer to next memory block
		unsigned int size;		// Size of the block
	} blk;
	Align x; // Force alignment
};

typedef union header Header;

extern void *myMalloc(unsigned int nbytes);
extern void myFree(void *ap);
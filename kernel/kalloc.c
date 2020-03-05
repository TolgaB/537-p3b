// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "spinlock.h"
#include "rand.h"

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

extern char end[]; // first address after kernel loaded from ELF file

int dumpFrames[512];
int numDumpFrames;
// Initialize free list of physical pages.
void
kinit(void)
{
  char *p;

  initlock(&kmem.lock, "kmem");
  p = (char*)PGROUNDUP((uint)end);
  for(; p + PGSIZE <= (char*)PHYSTOP; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || (uint)v >= PHYSTOP) 
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(v, 1, PGSIZE);
  for (int i = 0; i < 512; i++) {
	if (dumpFrames[i] == (int )v) {
		numDumpFrames--;
		dumpFrames[i] = 0;
		break;
	}
  }
  acquire(&kmem.lock);
  r = (struct run*)v;
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;
  struct run *sizeF;
  //do everything inside to avoid race conditions	
  acquire(&kmem.lock);
  //try to get size
  sizeF = kmem.freelist;
  unsigned int size = 0;
  while (sizeF) {
          size++;
          sizeF = sizeF->next;
  }
  int randInt = xv6_rand();
  int y = (randInt % size);
  r = kmem.freelist;
  if (r) { 
	struct run *prev;
	for (int i = 0; i < y; i++) {
	 	 prev = r;
		  r = r->next;
	  
 	 }
	if (prev) {  
    		prev->next = r->next;
	}
	else {
		kmem.freelist = r->next;
	}
  }
  release(&kmem.lock);
  for (int i = 0; i < 512; i++) {
	  if (dumpFrames[i] == 0) {
		  dumpFrames[i] = (int)r;
		  numDumpFrames++;
		  break;
	  }
  }
  return (char*)r;
}

int
dump_helper_kalloc(int *frames, int numFrames) {
        if ((numFrames > numDumpFrames) || (numFrames <= 0)) {
                return -1;
        }
        int j = 0;
        for (int i = 511; i >= 0; i--) {
                if (dumpFrames[i] != 0) {
                        frames[j] = dumpFrames[i];
                        j++;
                        if (numFrames == j) {
                                break;
                        }
                }
        }
        return 0;
}

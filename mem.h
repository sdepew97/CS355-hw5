#ifndef __MEM_H_
#define __MEM_H_

#define E_NO_SPACE            1 //run out of space //TODO: ask Rachel what each of these mean in the context of the project?
#define E_CORRUPT_FREESPACE   2 //when header overwritten overwrite checksum
#define E_PADDING_OVERWRITTEN 3 //PADDING_OVERWRITTEN error raised when header padding overwritten...
#define E_BAD_ARGS            4 //BAD_ARGS is used a lot
#define E_BAD_POINTER         5 //use with Mem_Free...if someone passes in bad pointer

extern int m_error;

int Mem_Init(long sizeOfRegion);

void *Mem_Alloc(long size); //TODO: error check once fine at the end...
//headers of both lists should point to the same values to begin

/*
 * 1) round up to a multiple of 8
 * 2) Find location of WF free in list (head)
 * 3) Add on header and adjust pointers
 * 4) Re-sort the free list after new insertion (determine if list is partially sorted)
 */

int Mem_Free(void *ptr, int coalesce);

/*
 * 0) no coalesce then mark as free and add pointers (done) else if coalesce is true
 * 1) Mark byte as free rather than occupied (don't have to go through list)
 * 2) Update free pointers in free list and locally coalesce
 * 3) Coalesce globally if needed (don't do global coalesce if you always call coalesce and code should know??)
 * 4) Think about situation where you don't have to go thorough the whole list when you call this (have to decide if list has to be traversed)
 * 5) Set flag if you need a global traversal or not...
 * 6) Sort free list (if you're sorting it) (no matter flag for coalesce) //TODO: figure out if optimization is needed...
 */

void Mem_Dump();

/*
 * Order of Development Recommended:
 * 1) Init
 * 2) Allocation
 * 3) Free
 * 4) test along the way
 * 5) add the components mentioned earlier
 * 6) Add complexity/optimize
 */
#endif



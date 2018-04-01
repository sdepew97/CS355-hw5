#ifndef __MEM_H_
#define __MEM_H_

#define E_NO_SPACE            1 //run out of space
#define E_CORRUPT_FREESPACE   2 //when header overwritten overwrite checksum
#define E_PADDING_OVERWRITTEN 3 //PADDING_OVERWRITTEN error raised when header padding overwritten...
#define E_BAD_ARGS            4 //BAD_ARGS is used a lot
#define E_BAD_POINTER         5 //use with Mem_Free...if someone passes in bad pointer

extern int m_error;

int Mem_Init(long sizeOfRegion);
void *Mem_Alloc(long size);
int Mem_Free(void *ptr, int coalesce);
void Mem_Dump();

/*
 * Order of Development Recommended:
 * 1) Init (done)
 * 2) Allocation (done)
 * 3) Free (working on it...done!)
 * 4) test along the way (test all API's used)
 * 5) add the components mentioned earlier
 * 6) Add complexity/optimize, add all the error checking components here
 */
#endif



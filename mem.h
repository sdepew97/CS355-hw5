#ifndef __MEM_H_
#define __MEM_H_

#define E_NO_SPACE            1 //run out of space //TODO: ask Rachel what each of these mean in the context of the project?
#define E_CORRUPT_FREESPACE   2 //
#define E_PADDING_OVERWRITTEN 3 //
#define E_BAD_ARGS            4 //BAD_ARGS is used a lot
#define E_BAD_POINTER         5 //use with Mem_Free...if someone passes in bad pointer

extern int m_error;

int Mem_Init(long sizeOfRegion);

void *Mem_Alloc(long size);

int Mem_Free(void *ptr, int coalesce);

void Mem_Dump();

#endif



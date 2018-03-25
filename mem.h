#ifndef __MEM_H_
#define __MEM_H_

#define E_NO_SPACE            1 //TODO: ask Rachel what each of these mean in the context of the project?
#define E_CORRUPT_FREESPACE   2
#define E_PADDING_OVERWRITTEN 3
#define E_BAD_ARGS            4
#define E_BAD_POINTER         5

extern int m_error;

size_t roundToPage(int currentSize); //TODO: remove after testing!

int Mem_Init(long sizeOfRegion);

void *Mem_Alloc(long size);

int Mem_Free(void *ptr, int coalesce);

void Mem_Dump();

#endif



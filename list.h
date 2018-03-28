//
// Created by Sarah Depew on 3/25/18.
//

#ifndef HW5_LIST_H
#define HW5_LIST_H

typedef struct header {
    char checkSum; //unique value to check upon reading next area of memory
    char free; //byte flag of free or not
    struct header *nextHeader; //first linked list
    long amountAllocated; //to be used for compaction
    struct header *nextFree; //second linked list pointers
    //TODO: figure out how to add another check sum here...
    //TODO: ask about the automatic padding here????
} header;

extern header *headMainList;
extern header *headFreeList;

header *worstFit(header *head);
void coalesceList(header *head);

//go through free list

//combine neighboring memory sections

#endif //HW5_LIST_H

//
// Created by Sarah Depew on 3/25/18.
//

#ifndef HW5_LIST_H
#define HW5_LIST_H

typedef struct header {
//    boolean free;
    //look up type for single bit for free or not... //TODO
    //TODO: pad to 32 bytes here and then set check sums
//    short firstCheckSum;
    boolean free;
    struct header *nextHeader; //first linked list
    struct header *nextFree; //second linked list
    long amountAllocated; //to be used for compaction
    // know what the user has because you round up to nearest multiple of 8
//    short secondCheckSum;
    //TODO: add check sums
} header;

extern header *headMainList;
extern header *headFreeList;

header *worstFit(header *head);
void coalesceList(header *head);

//go through free list

//combine neighboring memory sections

#endif //HW5_LIST_H

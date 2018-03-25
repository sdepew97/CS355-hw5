//
// Created by Sarah Depew on 3/25/18.
//

#include "boolean.h"

#ifndef HW5_LIST_H
#define HW5_LIST_H

typedef struct node {
    boolean free;
    void *prevHeader;
    void *nextHeader;
    int sizeOfRegion;
    int amountAllocated; //to be used for compaction
    //TODO: ask Rachel what else I could need here? (Done)
} node;

extern node *head;

void coalesceList(node *head) {
    //go through free list

    //combine neighboring memory sections
}

#endif //HW5_LIST_H

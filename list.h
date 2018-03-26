//
// Created by Sarah Depew on 3/25/18.
//

#include "boolean.h"

#ifndef HW5_LIST_H
#define HW5_LIST_H

typedef struct node {
    boolean free;
    node *prevHeader;
    node *nextHeader;
    int sizeOfRegion;
    int amountAllocated; //to be used for compaction
    //TODO: ask Rachel what else I could need here? (Done)
} node;

extern node *head;
extern node *tail;

void *worstFit(node *head);
void coalesceList(node *head);

//go through free list

//combine neighboring memory sections

#endif //HW5_LIST_H

//
// Created by Sarah Depew on 3/25/18.
//

#ifndef HW5_LIST_H
#define HW5_LIST_H

typedef struct node {
    //TODO: determine the type of information needed here :)
    boolean free;
    void *prevHeader;
    int lengthOfRegion;
//    int amountRequested; //to be returned to the user/what they think they have //TODO: ask Rachel about how to check this?? (Done)
    int amountAllocated; //to be used for compaction
    void *nextHeader;
    //TODO: ask Rachel what else I could need here? (Done)
} node;

extern node *head;

void coalesceList(node *head) {
    //go through free list

    //combine neighboring memory sections
}

#endif //HW5_LIST_H

//
// Created by Sarah Depew on 3/31/18.
//
#include "list.h"

header *worstFit(header *head) {
    //assume that the free list is sorted
    return head;
}

void addHeader (header **head, header *newHeader, header *previous) {
    if (previous == NULL) {
        //Add to start of list
        newHeader->nextHeader = *head;
        *head = newHeader;
    } else if (previous->nextHeader == NULL) {
        //Add to end of list
        newHeader->nextHeader = previous->nextHeader;
        previous->nextHeader = newHeader;
    } else {
        //Add to the middle of the list
        newHeader->nextHeader = previous->nextHeader;
        previous->nextHeader = newHeader;
    }
}

//TODO: error check this method
/*
 * Method to remove a header from the free list
 */
void removeFreeHeader (header **head, header *headerToRemove, header *previous) {
    if (previous == NULL) {
        //Remove at start of list
        *head = headerToRemove->nextFree;
        headerToRemove->nextFree = NULL;
    } else if (previous->nextFree == NULL) {
        //Remove at end of list
        previous->nextFree = NULL;

    } else {
        //Remove at the middle of the list
        previous->nextFree = headerToRemove->nextFree;
        headerToRemove->nextFree = NULL;
    }
}

//TODO: error check this method (something isn't right with this method and how it runs...keeps creating circular linked lists...)
void sortList (header **head) {
    //Put the largest available node as the header of the list
    long worstFitValue = (*head)->amountAllocated;
    header *worstFit = *head;
    header *worstFitPrevious = NULL;

    header *currentHeader = *head;
    header *previousHeader = NULL;

    while (currentHeader != NULL) {
        if (currentHeader->amountAllocated > worstFitValue) {
            worstFitValue = currentHeader->amountAllocated;
            worstFitPrevious = previousHeader;
            worstFit = currentHeader;
        }
        previousHeader = currentHeader;
        currentHeader = currentHeader->nextFree;
    }

    if (worstFitPrevious == NULL) {
        //we are done, since our node is already the head
    } else {
        //remove worstFit node
        removeFreeHeader(&headFreeList, worstFit, worstFitPrevious);

        //add it to the head
        worstFit->nextFree = *head;
        *head = worstFit;
    }
}

void localCoalesceForGlobal(header *ptr, header *prev) {
    if (ptr != NULL) {
        if (ptr->nextHeader != NULL && ptr->nextHeader->free == 't') {
            if ((((void *) ptr) + sizeof(header) + roundToWord(ptr->amountAllocated)) == ptr->nextHeader) {
                //interesting case where adjacent blocks are free
                ptr->amountAllocated = roundToWord(ptr->amountAllocated) + sizeof(header) +
                                       roundToWord(ptr->nextHeader->amountAllocated);

                //adjust both lists according to the local coalesce that just happened (need to remove ptr->nextHeader from free list and from other list
                removeFreeHeader(&headFreeList, ptr->nextHeader, prev);
                ptr->nextHeader = ptr->nextHeader->nextHeader;
            } else {
                //do nothing since not adjacent
            }
        } else {
            //do nothing since already at end of list
        }
    } else {
        //do nothing, since error value was passed in
    }
}

void localCoalesce(header *ptr) {
    if (ptr != NULL) {
        if (ptr->nextHeader != NULL && ptr->nextHeader->free == 't') {
            if ((((void *) ptr) + sizeof(header) + roundToWord(ptr->amountAllocated)) == ptr->nextHeader) {
                //interesting case where adjacent blocks are free
                ptr->amountAllocated = roundToWord(ptr->amountAllocated) + sizeof(header) +
                                       roundToWord(ptr->nextHeader->amountAllocated);

                //adjust both lists according to the local coalesce that just happened (need to remove ptr->nextHeader from free list and from other list
                if (ptr->nextHeader == headFreeList) {
                    removeFreeHeader(&headFreeList, ptr->nextHeader, NULL);
                } else {
                    removeFreeHeader(&headFreeList, ptr->nextHeader, findPreviousFree(headFreeList, ptr));
                }

                ptr->nextHeader = ptr->nextHeader->nextHeader;
            } else {
                //do nothing since not adjacent
            }
        } else {
            //do nothing since already at end of list
        }
    } else {
        //do nothing, since error value was passed in
    }
}

header *findPreviousFree(header *head, header *ptr) {
    header *currentHeader = head;

    if (ptr == NULL || head == NULL || currentHeader == ptr || currentHeader->nextFree == NULL) {
        return NULL;
    } else {
        while (currentHeader->nextFree != NULL) {
            if (currentHeader->nextFree == ptr) {
                return currentHeader;
            }

            currentHeader = currentHeader->nextFree;
        }
    }

    return NULL;
}

//TODO: check this works
void coalesceList(header *head) {
    //nothing yet here...
    boolean coalesceOccurred = FALSE;
    header *currentHeader;
    header *previousHeader;

    do {
        currentHeader = head;
        previousHeader = NULL;
        coalesceOccurred = FALSE;

        while (currentHeader != NULL) {
            if (currentHeader->free == 't') {
                localCoalesceForGlobal(currentHeader, previousHeader);
                coalesceOccurred = TRUE;
            }

            previousHeader = currentHeader;
            currentHeader = currentHeader->nextHeader;
        }

    } while (coalesceOccurred);
}

int checkValid(header *head, void *ptr) {
    header *currentHeader = head;

    while (currentHeader != NULL) {
        if (((void *) currentHeader + sizeof(header)) == ptr) {
            return TRUE;
        }

        currentHeader = currentHeader->nextHeader;
    }

    return FALSE;
}
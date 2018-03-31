//
// Created by Sarah Depew on 3/31/18.
//
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include "boolean.h"
#include "helper.h"
#include "list.h"

/*
 * Only needed for list tests, comment out for mem tests
 */
//header *headMainList = NULL;
//header *headFreeList = NULL;

//general methods for lists
header *worstFit(header *head) {
    //assume that the free list is sorted
    return head;
}

//methods specifically for main list
void addHeaderMain (header **head, header *newHeader, header *previous) {
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

void removeHeaderMain(header **head, header *newHeader, header *previous) {
    //TODO: implement body here to replace code in main
}

int checkValidPtrMain(header *head, void *ptr) {
    header *currentHeader = head;

    while (currentHeader != NULL) {
        if (((void *) currentHeader + sizeof(header)) == ptr) {
            return TRUE;
        }

        currentHeader = currentHeader->nextHeader;
    }

    return FALSE;
}

//methods specifically for free list

void addHeaderFree(header **head, header *headerToRemove, header *previous) {
    //TODO: implement body here to replace code in main
}

//TODO: error check this method
/*
 * Method to remove a header from the free list
 */
void removeHeaderFree(header **head, header *headerToRemove, header *previous) {
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
void sortFreeList (header **head) {
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
        removeHeaderFree(head, worstFit, worstFitPrevious);

        //add it to the head
        worstFit->nextFree = *head;
        *head = worstFit;
    }
}

void localCoalesceFree(header **head, header *ptr) {
    if (ptr != NULL) {
        if (ptr->nextHeader != NULL && ptr->nextHeader->free == 't') {
            if ((((void *) ptr) + sizeof(header) + roundToWord(ptr->amountAllocated)) == ptr->nextHeader) {
                //interesting case where adjacent blocks are free
                ptr->amountAllocated = roundToWord(ptr->amountAllocated) + sizeof(header) +
                                       roundToWord(ptr->nextHeader->amountAllocated);

                //adjust both lists according to the local coalesce that just happened (need to remove ptr->nextHeader from free list and from other list
                if (ptr->nextHeader == headFreeList) {
                    removeHeaderFree(head, ptr->nextHeader, NULL);
                } else {
                    removeHeaderFree(head, ptr->nextHeader, findPreviousFree(headFreeList, ptr->nextHeader));
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

//TODO: check this works and actually program it!
void coalesceFreeList(header *head) {
    //nothing yet here...
    boolean coalesceOccurred = FALSE;
    header *currentHeader;
    header *previousHeader;

    do {
        currentHeader = head;
        previousHeader = NULL;
        coalesceOccurred = FALSE;

        while (currentHeader != NULL) {
            if (currentHeader->free == 't' && currentHeader->nextHeader != NULL &&
                currentHeader->nextHeader->free == 't') {
                localCoalesceFree(&headFreeList, currentHeader);
                coalesceOccurred = TRUE;
            }

            previousHeader = currentHeader;
            currentHeader = currentHeader->nextHeader;
        }

    } while (coalesceOccurred);
}

/*
 * //TODO: fix this method, so that it correctly returns the prior node
 */
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
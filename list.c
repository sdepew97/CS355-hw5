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
int checkPadding(header *header1) {
    if(header1 == NULL) {
        return TRUE;
    } else {
        return (header1->checkSum == 's');
    }
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

int checkValidPtrMain(header *head, long sizeOfList, void *ptr) {
    //check if in memory region
    void *min = (void *) head;
    void *max = (void *) head + sizeOfList;

    if ((ptr - sizeof(header)) < min || ptr > max) {
        return FALSE;
    } else {
        return checkPadding(ptr - sizeof(header));
    }
}

//methods specifically for free list
header *worstFitFree(header *head) {
    //assume that the free list is sorted
    sortFreeList(&head); //first have to sort the free list
    return head;
}

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

int sortFreeList (header **head) {
    //Put the largest available node as the header of the list
    long worstFitValue = (*head)->amountAllocated;
    header *worstFit = *head;
    header *worstFitPrevious = NULL;

    header *currentHeader = *head;
    header *previousHeader = NULL;

    while (currentHeader != NULL) {
        if(checkPadding(currentHeader) == FALSE) {
            return FALSE;
        }

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

    return TRUE;
}

//TODO: ask about if I have to keep coalescing down or just do direct neighbor? (no, not two adjacent)
int localCoalesceFree(header **head, header *ptr) {
    if (checkPadding(ptr)) {
        if (ptr != NULL && ptr->free == 't') {
            if (ptr->nextHeader != NULL && ptr->nextHeader->free == 't') {
                if ((((void *) ptr) + sizeof(header) + roundToWord(ptr->amountAllocated)) == ptr->nextHeader) {
                    //interesting case where adjacent blocks are free
                    ptr->amountAllocated = roundToWord(ptr->amountAllocated) + sizeof(header) +
                                           roundToWord(ptr->nextHeader->amountAllocated);

                    //adjust both lists according to the local coalesce that just happened (need to remove ptr->nextHeader from free list and from other list, since it has been combined)
                    if (ptr->nextHeader == headFreeList) {
                        removeHeaderFree(head, ptr->nextHeader, NULL);
                    } else {
                        removeHeaderFree(head, ptr->nextHeader, findPreviousFree(headFreeList,
                                                                                 ptr->nextHeader)); //have to find the previous in the free list...
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
    } else {
        //the padding has been corrupted
        return FALSE;
    }

    return TRUE;
}

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
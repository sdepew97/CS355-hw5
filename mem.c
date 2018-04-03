//
// Created by Sarah Depew on 3/24/18.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <math.h>
#include "mem.h"
#include "helper.h"
#include "boolean.h"
#include "list.h"

int m_error;
long howMuchUserHasLeftToRequest;
long sizeOfList;
boolean lastWasGlobal;
header *headMainList = NULL;
header *headFreeList = NULL;


int Mem_Init(long sizeOfRegion) {
    //check sizeOfRegion is a valid, non-negative or zero value
    if (sizeOfRegion <= 0) {
        m_error = E_BAD_ARGS;
        return ERROR;
    }

    //check that Mem_Init hasn't been called more than once
    if (headMainList != NULL) {
        //has already been called once, so this should raise an error
        m_error = E_BAD_ARGS;
        return ERROR;
    }

    //Request that much memory from mmap
    //TODO: pont define values connected to amountToMMap
    long amountToMmap = WORSTCASE;

    void *mapReturn = mmap(NULL, amountToMmap, PROT_EXEC | PROT_READ | PROT_WRITE,
                           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (mapReturn == MMAPERROR) {
        m_error = E_BAD_POINTER;
        return ERROR;
    }

    //Setup first node in lists (heads)
    //Attach record keeping pointers to the memory
    headMainList = (header *) mapReturn;
    headMainList->checkSum = 's';
    headMainList->free = 't';
    headMainList->nextHeader = NULL;
    headMainList->amountAllocated = amountToMmap - sizeof(header); //amount of memory after the header

    headFreeList = (header *) mapReturn;
    headFreeList->nextFree = NULL;

    //Setup Globals
    howMuchUserHasLeftToRequest = sizeOfRegion;
    lastWasGlobal = FALSE;
    sizeOfList = amountToMmap;

    //return the appropriate value
    return SUCCESS;
}

void *Mem_Alloc(long size) {
    //check sizeOfRegion is a valid, non-negative or zero value
    if (size <= 0) {
        m_error = E_BAD_ARGS;
        return NULL;
    }

    if (size > howMuchUserHasLeftToRequest) {
        //user requested more memory than they were allowed left
        m_error = E_BAD_ARGS;
        return NULL;
    }

    long sizeToWordSize = roundToWord(size); //only allocate word sizes
    //We need room for the header AND we need room for more word-aligned memory
    long totalSought = sizeToWordSize + sizeof(header) + SIZEOFWORD;

    //search through the list to get the largest available
    header *worstFitReturn = worstFitFree(headFreeList);

    if (worstFitReturn == NULL) {
        m_error = E_BAD_POINTER;
        return NULL;
    }

    if (checkPadding(worstFitReturn) == FALSE) {
        m_error = E_PADDING_OVERWRITTEN;
        return NULL;
    }

    if (worstFitReturn->amountAllocated < sizeToWordSize) {
        //not enough at all
        m_error = E_NO_SPACE;
        return NULL;
    } else if (worstFitReturn->amountAllocated == sizeToWordSize) {
        //exactly enough, so simply switch free bit to FALSE
        worstFitReturn->free = 'f';

        removeHeaderFree(&headFreeList, worstFitReturn,
                         NULL); //WE ARE ASSUMING THAT THE HEAD OF THE LIST IS CHOSEN HERE (which is how the worst fit algorithm works)

        //update what the user may request
        howMuchUserHasLeftToRequest -= size;

        return ((void *) worstFitReturn + sizeof(header));
    } else {
        //if there is also room for a header and at least another 8 bytes of memory, then proceed
        if (worstFitReturn->amountAllocated > totalSought) {
            header *newHeader = (void *) worstFitReturn + sizeToWordSize + sizeof(header); //location of new header
            newHeader->checkSum = 's';
            newHeader->free = 't';
            newHeader->amountAllocated = worstFitReturn->amountAllocated - sizeof(header) - sizeToWordSize;
            if (checkPadding(worstFitReturn->nextHeader)) {
                newHeader->nextHeader = worstFitReturn->nextHeader;
            } else {
                m_error = E_PADDING_OVERWRITTEN;
                return NULL;
            }

            if (checkPadding(headFreeList->nextFree)) {
                newHeader->nextFree = headFreeList->nextFree; //we want nextFree here, since we are removing the head of the list as it is being allocated and turned to false as available
                headFreeList->nextFree = NULL;
                headFreeList = newHeader;

            } else {
                m_error = E_PADDING_OVERWRITTEN;
                return NULL;
            }

            //make header not free anymore
            worstFitReturn->free = 'f';
            worstFitReturn->amountAllocated = size;
            addHeaderMain(&headMainList, newHeader,
                          worstFitReturn); //worstFitReturn is previous, since the memory was added to the top and the header, below

            //update what the user may request
            howMuchUserHasLeftToRequest -= size;

            return (void *) worstFitReturn + sizeof(header); //this is the section where we made room for the memory! :)
        }
            //else there is not enough room to split the memory into the section plus header, so fails!
        else {
            //not enough to fit another header AND the memory sought
            m_error = E_NO_SPACE;
            return NULL;
        }
    }
}

int Mem_Free(void *ptr, int coalesce) {
    if (coalesce > 1 || coalesce < 0) {
        m_error = E_BAD_ARGS;
        return ERROR;
    }

    if (ptr == NULL) {
        //don't mark anything as free, since ptr is NULL
    } else {
        //Mark as free and add to free list
        if (checkValidPtrMain(headMainList, sizeOfList, ptr)) {
            //check if the ptr has been overwritten
            if (ptr != NULL && checkPadding((header *) (ptr - sizeof(header))) == FALSE) {
                m_error = E_PADDING_OVERWRITTEN;
                return ERROR;
            }

            //check if already free and if so, then don't add to free list, since it is already there
            if (((header *) (ptr - sizeof(header)))->free == 't') {
                //do nothing and return an error, since the ptr should not be freed a second time
                m_error = E_BAD_ARGS;
                return ERROR;
            } else {
                howMuchUserHasLeftToRequest += ((header *) (ptr - sizeof(header)))->amountAllocated;
                ((header *) (ptr -
                             sizeof(header)))->free = 't'; //was removed from list, since is false, so no circular linking is gonna happen here
                ((header *) (ptr - sizeof(header)))->nextFree = headFreeList;
                //WE ARE ASSUMING THAT THE HEAD OF THE LIST IS CHOSEN HERE for worstFitReturn
                headFreeList = ((header *) (ptr - sizeof(header)));

            }
        } else {
            m_error = E_BAD_POINTER;
            return ERROR;
        }
    }

    //check for coalesce, now
    if (coalesce == FALSE) {
        //TODO: fix current bug here that would cause coalesce to be called each time on mem_free(NULL, 0) call etc...add code to work on not calling a ton (do 2 million free(NULL, 1) in a row...only go over list once)
        //false means don't want a global coalesce
        if (ptr == NULL) {
            if (localCoalesceFree(&headFreeList, NULL) == FALSE) {
                m_error = E_PADDING_OVERWRITTEN;
                return ERROR;
            }
        } else {
            if (localCoalesceFree(&headFreeList, ((header *) (ptr - sizeof(header)))) == FALSE) {
                m_error = E_PADDING_OVERWRITTEN;
                return ERROR;
            }
        }

        lastWasGlobal = FALSE;
        return SUCCESS;
    } else {
        //do something here, now, since we are asked to coalesce
        //go through the free list and combine memory sections
        if (ptr == NULL) {
            if (localCoalesceFree(&headFreeList, NULL) == FALSE) {
                m_error = E_PADDING_OVERWRITTEN;
                return ERROR;
            }
        } else {
            header *newPtr = findPreviousMain(headMainList, (header *) (ptr - sizeof(header)));

            if (newPtr == NULL || newPtr->free == 'f') {
                //then ptr is first node and should not try to coalesce from that one
                newPtr = (header *) (ptr - sizeof(header));
            } else {
                //have to coalesce above...
                if (localCoalesceFree(&headFreeList, newPtr) == FALSE) {
                    m_error = E_PADDING_OVERWRITTEN;
                    return ERROR;
                }
            }

            //and below...
            if (localCoalesceFree(&headFreeList, newPtr) == FALSE) {
                m_error = E_PADDING_OVERWRITTEN;
                return ERROR;
            }
        }

        //only coalesce if requested AND the last coalesce was local and not global
        if (coalesce && !lastWasGlobal) {
//        if (coalesce) {
            coalesceFreeList(headMainList);
        }

        lastWasGlobal = TRUE;
        return SUCCESS;
    }

    //default to return success
    return SUCCESS;
}

void Mem_Dump() {
    printf("\n*****************\n*    GLOBALS    %d, %ld, %d, %p, %p*\n*****************\n", m_error,
           howMuchUserHasLeftToRequest, lastWasGlobal, headMainList, headFreeList);
    printf("\n*****************\n*    FREE MEMORY    *\n*****************\n");
    header *currentHeader = headMainList;
    long location = 0;

    if (currentHeader == NULL) {
        printf("*****************\n*    NO INIT    *\n*****************");
        return;
    }

    while (currentHeader != NULL) {
        //print header and then print occupied or free
        if (currentHeader == headMainList && currentHeader->free == 't') {
            printf("*****************\n%ld*   HEADER   *%ld\n*****************", location,
                   location + sizeof(header));
        } else if (currentHeader->free == 't') {
            printf("%ld*   HEADER   *%ld\n*****************", location,
                   location + sizeof(header));
        }
        location = location + sizeof(header);
        if (currentHeader->free == 't') {
            printf("\n%ld*   FREE   *%ld, %ld\n*****************\n", location,
                   location + currentHeader->amountAllocated, location + roundToWord(currentHeader->amountAllocated));
            location = location + roundToWord(currentHeader->amountAllocated);
        } else {
            //We don't need to print allocated here
            location = location + roundToWord(currentHeader->amountAllocated);
        }
        currentHeader = currentHeader->nextHeader;
    }

    printf("\n*****************\n*    ALL MEMORY    *\n*****************\n");
    currentHeader = headMainList;
    location = 0;

    if (currentHeader == NULL) {
        printf("*****************\n*    NO INIT    *\n*****************");
        return;
    }

    while (currentHeader != NULL) {
        //print header and then print occupied or free
        if (currentHeader == headMainList) {
            printf("*****************\n%ld*   HEADER   *%ld\n*****************", location,
                   location + sizeof(header));
        } else {
            printf("%ld*   HEADER   *%ld\n*****************", location,
                   location + sizeof(header));
        }
        location = location + sizeof(header);
        if (currentHeader->free == 't') {
            printf("\n%ld*   FREE   *%ld, %ld\n*****************\n", location,
                   location + currentHeader->amountAllocated, location + roundToWord(currentHeader->amountAllocated));
            location = location + roundToWord(currentHeader->amountAllocated);
        } else {
            printf("\n%ld*   ALLOCATED   *%ld, %ld\n*****************\n", location,
                   location + currentHeader->amountAllocated, location + roundToWord(currentHeader->amountAllocated));
            location = location + roundToWord(currentHeader->amountAllocated);
        }
        currentHeader = currentHeader->nextHeader;
    }
}
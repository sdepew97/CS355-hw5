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
boolean needGlobal;
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
    int amountToMmap = roundToPage((sizeOfRegion * SIZEOFWORD) + (sizeOfRegion * sizeof(header)) +
                                   sizeof(header) + sizeof(header *) + sizeof(header *) + sizeof(int) + sizeof(long)); //worst case what we need to have mmaped to give the user what they requested

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
    howMuchUserHasLeftToRequest = sizeOfRegion; //TODO: add error checking to ensure the user doesn't ask for memory beyond this amount
    needGlobal = FALSE;

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

    //TODO: subtract what the user asked for with size, so we can error check here as well...

    int sizeToWordSize = roundToWord(size); //only allocate word sizes
    //We need room for the header AND we need room for more word-aligned memory
    int totalSought = sizeToWordSize + sizeof(header) + SIZEOFWORD;

    //search through the list to get the largest available
    header *worstFitReturn = worstFit(headFreeList);

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
        sortFreeList(&headFreeList);

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
                sortFreeList(&headFreeList); //need to ensure largest header is indeed at head of the list
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

    if (checkPadding(((header *) ptr - sizeof(header))) == FALSE) {
        m_error = E_PADDING_OVERWRITTEN;
        return ERROR;
    }

    if (checkFreeSpace(ptr)) {
        //check that the user didn't write into memory they were not allowed to write into
        //TODO: ask Rachel how we can check the free space without cleaning the memory?? (emailed Dianna)
    }

    if (ptr == NULL) {
        //don't mark anything as free, since ptr is NULL
    } else {
        //Mark as free and add to free list
        if (checkValidPtrMain(headMainList, ptr)) {
            //check if already free and if so, then don't add to free list, since it is already there
            if (((header *) (ptr - sizeof(header)))->free == 't') {
                //do nothing, since it should not be added a second time
            } else {
                ((header *) (ptr -
                             sizeof(header)))->free = 't'; //was removed from list, since is false, so no circular linking is gonna happen here
                ((header *) (ptr - sizeof(header)))->nextFree = headFreeList;
                //WE ARE ASSUMING THAT THE HEAD OF THE LIST IS CHOSEN HERE for worstFitReturn
                headFreeList = ((header *) (ptr - sizeof(header)));
            }

            //TODO: think about the placement of this statement
            if (sortFreeList(&headFreeList) == FALSE) {
                m_error = E_PADDING_OVERWRITTEN;
                return ERROR;
            }
        } else {
            m_error = E_BAD_POINTER;
            return ERROR;
        }
    }

    //check for coalesce, now
    if (coalesce == FALSE) {
        //TODO: figure out how to set this value, so that we don't coalesce each time! EFFICIENCY! (ask Rachel tomorrow)
        //TODO: fix current bug here that would cause coalesce to be called each time on mem_free(NULL, 0) call
        //false means don't want a global coalesce
        localCoalesceFree(&headFreeList, ((header *) (ptr - sizeof(header))));
        needGlobal = TRUE; //need a global coalesce next time //TODO: this is not necessarily true...need to fix this bug
        return SUCCESS;
    } else {
        //do something here, now, since we are asked to coalesce
        //go through the free list and combine memory sections
        if (localCoalesceFree(&headFreeList, ((header *) (ptr - sizeof(header)))) == FALSE) {
            m_error = E_PADDING_OVERWRITTEN;
            return ERROR;
        }

        //TODO: figure out what's wrong with globalCoalesce
        if (needGlobal || coalesce) {
            coalesceFreeList(headMainList);
        }

        if (sortFreeList(&headFreeList) == FALSE) {
            m_error = E_PADDING_OVERWRITTEN;
            return ERROR;
        }

        return SUCCESS;
    }

    //default to return success
    return SUCCESS;
}

void Mem_Dump() {
    printf("\n*****************\n*    GLOBALS    %d, %ld, %d, %p, %p*\n*****************\n", m_error,
           howMuchUserHasLeftToRequest, needGlobal, headMainList, headFreeList);
    printf("\n*****************\n*    FREE MEMORY    *\n*****************\n");
    header *currentHeader = headMainList;
    int location = 0;

    if (currentHeader == NULL) {
        printf("*****************\n*    NO INIT    *\n*****************");
        return;
    }

    while (currentHeader != NULL) {
        //print header and then print occupied or free
        if (currentHeader == headMainList && currentHeader->free == 't') {
            printf("*****************\n%d*   HEADER   *%ld\n*****************", location,
                   location + sizeof(header));
        } else if (currentHeader->free == 't') {
            printf("%d*   HEADER   *%ld\n*****************", location,
                   location + sizeof(header));
        }
        location = location + sizeof(header);
        if (currentHeader->free == 't') {
            printf("\n%d*   FREE   *%ld, %ld\n*****************\n", location,
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
            printf("*****************\n%d*   HEADER   *%ld\n*****************", location,
                   location + sizeof(header));
        } else {
            printf("%d*   HEADER   *%ld\n*****************", location,
                   location + sizeof(header));
        }
        location = location + sizeof(header);
        if (currentHeader->free == 't') {
            printf("\n%d*   FREE   *%ld, %ld\n*****************\n", location,
                   location + currentHeader->amountAllocated, location + roundToWord(currentHeader->amountAllocated));
            location = location + roundToWord(currentHeader->amountAllocated);
        } else {
            printf("\n%d*   ALLOCATED   *%ld, %ld\n*****************\n", location,
                   location + currentHeader->amountAllocated, location + roundToWord(currentHeader->amountAllocated));
            location = location + roundToWord(currentHeader->amountAllocated);
        }
        currentHeader = currentHeader->nextHeader;
    }
}
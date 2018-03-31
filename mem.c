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
long howMuchUserRequested;
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

    howMuchUserRequested = sizeOfRegion; //TODO: add error checking to ensure the user doesn't ask for memory beyond this amount
    needGlobal = FALSE;

    //Request that much memory from mmap
    //TODO: check with Dianna this value tomorrow in OH (pont define these values)
    int amountToMmap = roundToPage((sizeOfRegion * 8) + (sizeOfRegion * sizeof(header)) +
                                   sizeof(header) + sizeof(header *) * 2 + sizeof(int) + sizeof(long)); //worst case what we need to have mmaped to give the user what they requested
    void *mapReturn = mmap(NULL, amountToMmap, PROT_EXEC | PROT_READ | PROT_WRITE,
                           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (*((int *) mapReturn) == ERROR) {
        m_error = E_BAD_POINTER; //TODO: check this is correct error to be raising (check for this...) (check error value for what to return here)
        return ERROR;
    }

    printf("%p\n", mapReturn); //printing pointer?

    //Setup first node in lists (heads)
    //Attach record keeping pointers to the memory
    headMainList = (header *) mapReturn; //TODO: gdb to check this... (how, ask her tomorrow after class!)
    headFreeList = (header *) mapReturn;

    headMainList->checkSum = 's';
    headMainList->free = 't';
    headMainList->nextHeader = NULL;
    headMainList->amountAllocated = amountToMmap - sizeof(header); //amount of memory after the header

    headFreeList->nextFree = NULL;

    printf("head %p\n", headMainList);
    printf("Amount Allocated %ld\n", headMainList->amountAllocated);

    //return the appropriate value
    return SUCCESS;
}

/*
 * 1) round up to a multiple of 8
 * 2) Find location of WF free in list (head)
 * 3) Add on header and adjust pointers
 * 4) Re-sort the free list after new insertion (determine if list is partially sorted)
 */
void *Mem_Alloc(long size) {
    //TODO: subtract what the user asked for with size, so we can error check here as well...

    int sizeToWordSize = roundToWord(size); //only allocate word sizes
    int totalSought =
            sizeToWordSize + sizeof(header) +
            8; //We need room for the header AND we need room for more word-aligned memory

    //search through the list to get the largest available
    header *worstFitReturn = worstFit(headFreeList); //TODO: could this ever return NULL?

    if (worstFitReturn->amountAllocated < sizeToWordSize) {
        //not enough at all
        m_error = E_NO_SPACE;
        return NULL;
    } else if (worstFitReturn->amountAllocated == sizeToWordSize) {
        //exactly enough, so simply switch free bit to FALSE
        worstFitReturn->free = 'f';

        removeHeaderFree(&headFreeList, worstFitReturn, NULL); //WE ARE ASSUMING THAT THE HEAD OF THE LIST IS CHOSEN HERE
        sortFreeList(&headFreeList);

        return ((void *) worstFitReturn + sizeof(header)); //TODO: check pointer arithmetic with her, tomorrow
    } else {
        //if there is also room for a header and at least another 8 bytes of memory, then proceed
        if (worstFitReturn->amountAllocated > totalSought) {
            //add a node here!

            header *newHeader = (void *) worstFitReturn + sizeToWordSize + sizeof(header); //location of new header
            newHeader->checkSum = 's'; //TODO: add in check sum checking for errors
            newHeader->free = 't';
            newHeader->amountAllocated = worstFitReturn->amountAllocated - sizeof(header) - sizeToWordSize;
            newHeader->nextHeader = worstFitReturn->nextHeader;

//            newHeader->nextFree = headFreeList->nextFree; //WE ARE ASSUMING THAT THE HEAD OF THE LIST IS CHOSEN HERE for worstFitReturn
            newHeader->nextFree = headFreeList->nextFree; //we want nextFree here, since we are removing the head of the list as it is being allocated and turned to false as available
//            headFreeList->nextFree = NULL; //TODO: determine if this line is correct?
            headFreeList->nextFree = NULL; //TODO: determine if this line is correct? (yes)
            headFreeList = newHeader;
            sortFreeList(&headFreeList); //need to ensure largest header is indeed at head of the list

            //make header not free anymore
            //TODO: add check sums, here
            worstFitReturn->free = 'f';

            worstFitReturn->amountAllocated = size;  //Note: changed to permit for error checking
            addHeaderMain(&headMainList, newHeader,
                      worstFitReturn); //worstFitReturn is previous, since the memory was added to the top and the header, below

            printf("%p pointer to returned region\n", (void *) worstFitReturn + sizeof(header));
            return (void *) worstFitReturn + sizeof(header); //this is the section where we made room for the memory! :)
        }
            //else there is not enough room to split the memory into the section plus header, so fails!
        else {
            //not enough to fit another header AND the memory sought
            m_error = E_NO_SPACE;
            return NULL;
        }
    }

    //return NULL as a default value
    return NULL;
}

/*
 * 0) No coalesce then mark as free and add pointers (done) else if coalesce is true
 * 1) Mark byte as free rather than occupied (don't have to go through list)
 * 2) Update free pointers in free list and locally coalesce
 * 3) Coalesce globally if needed (don't do global coalesce if you always call coalesce and code should know??)
 * 4) Think about situation where you don't have to go thorough the whole list when you call this (have to decide if list has to be traversed)
 * 5) Set flag if you need a global traversal or not...
 * 6) Sort free list (if you're sorting it) (no matter flag for coalesce) //TODO: figure out if optimization is needed... (yes, and already have optimized)
 */
int Mem_Free(void *ptr, int coalesce) {
    if (ptr == NULL) {
        //don't mark anything as free, since ptr is NULL
    } else {
        //Mark as free and add to free list
        if (checkValidPtrMain(headMainList, ptr)) {
            //check if already free and if so, then don't add to free list, since it is already there
            if (((header *) (ptr - sizeof(header)))->free == 't') {
                //do nothing, since it should not be added a second time
            } else {
                //Don't need to add to free list again and sort
                ((header *) (ptr - sizeof(header)))->free = 't';
                ((header *) (ptr - sizeof(header)))->nextFree = headFreeList;
                //WE ARE ASSUMING THAT THE HEAD OF THE LIST IS CHOSEN HERE for worstFitReturn
                headFreeList = ((header *) (ptr - sizeof(header)));
            }

            //TODO: think about the placement of this statement
            sortFreeList(&headFreeList);
        } else {
            m_error = E_BAD_POINTER;
            return ERROR;
        }
    }

    //check for coalesce, now
    if (coalesce == FALSE) {
        //TODO: figure out how to set this value, so that we don't coalesce each time! EFFICIENCY!
        //TODO: fix current bug here that would cause coalesce to be called each time on mem_free(NULL, 0) call
        //False means don't want a global coalesce
        localCoalesceFree(&headFreeList, ((header *) (ptr - sizeof(header))));
        needGlobal = TRUE; //need a global coalesce next time
        return SUCCESS;
    } else {
        //do something here, now, since we are asked to coalesce
        //go through the free list and combine memory sections
        localCoalesceFree(&headFreeList, ((header *) (ptr - sizeof(header))));

        //TODO: figure out what's wrong with globalCoalesce
        if (needGlobal || coalesce) {
            coalesceFreeList(headMainList);
        }

        sortFreeList(&headFreeList); //TODO: determine if needed? Yes, because there are cases when needed
        return SUCCESS;
    }

    //default to return success
    return SUCCESS;
}

void Mem_Dump() {
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
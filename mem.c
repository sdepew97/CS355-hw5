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
    printf("head allocated value %ld\n", headMainList->amountAllocated);

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
    int sizeToWordSize = roundToWord(size); //only allocate word sizes
    int totalSought =
            sizeToWordSize + sizeof(header) +
            8; //We need room for the header AND we need room for more word-aligned memory

    //search through the list to get the largest available
    header *worstFitReturn = worstFit(headFreeList);

    if (worstFitReturn->amountAllocated < sizeToWordSize) {
        //not enough at all
        m_error = E_NO_SPACE;
        return NULL;
    } else if (worstFitReturn->amountAllocated == sizeToWordSize) {
        //exactly enough, so simply switch free bit to FALSE
        worstFitReturn->free = 'f';

        removeFreeHeader(headFreeList, worstFitReturn, NULL); //WE ARE ASSUMING THAT THE HEAD OF THE LIST IS CHOSEN HERE
        sortList(headFreeList);

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

            newHeader->nextFree = headFreeList->nextFree; //WE ARE ASSUMING THAT THE HEAD OF THE LIST IS CHOSEN HERE for worstFitReturn
            headFreeList = newHeader;
            sortList(headFreeList); //need to ensure largest header is indeed at head of the list

            //make header not free anymore
            //TODO: add check sums, here
            worstFitReturn->free = 'f';
            worstFitReturn->amountAllocated = sizeToWordSize;
            addHeader(headMainList, newHeader,
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

int Mem_Free(void *ptr, int coalesce) {
//    if (ptr == NULL) {
//        //do nothing
//    } else {
//
//        //ptr is not null
//
//        //free memory object ptr points to
//    }
//
//    //check for coalesce, now
//    if (coalesce == FALSE) {
//        //simply can return, now
//    } else {
//        //do something here, now, since we are asked to coalesce
//        //go through the free list and combine memory sections
//        coalesceList(head);
//    }
}

void Mem_Dump() {}
//    //TODO: ask about what if the user requests memory not in an increment of page for sizeOfRegion and how that affects printout here...
//    node *currentNode = head;
//    int location = 0;
//
//    if(currentNode == NULL) {
//        printf("*****************\n*    NO INIT    *\n*****************");
//        return;
//    }
//
//    while (currentNode != NULL) {
//        //print header and then print occupied or free
//        if(currentNode == head) {
//            printf("*****************\n%d*   HEADER   *%d\n*****************", location, location + currentNode->sizeOfRegion - currentNode->amountAllocated);
//        } else {
//            printf("%d*   HEADER   *%d\n*****************", location, location + currentNode->sizeOfRegion - currentNode->amountAllocated);
//        }
//        location = location + SIZEOFHEADER;
//        if (currentNode->free == TRUE) {
//            printf("\n%d*   FREE   *%d\n*****************\n", location,
//                   location + currentNode->amountAllocated);
//            location = location + currentNode->amountAllocated;
//        } else {
//            printf("\n%d*   ALLOCATED   *%d\n*****************\n", location,
//                   location + currentNode->amountAllocated);
//            location = location + currentNode->amountAllocated;
//        }
//        currentNode = currentNode->nextHeader;
//    }
//}

//helper functions
size_t roundToPage(int currentSize) {
    //TODO: implement method that rounds the currentSize to a page size
    int pageSize = getpagesize();
    printf("Page Size: %d\n", pageSize);
    if ((currentSize / pageSize) * pageSize == currentSize) {
        //we are requesting a multiple of page size bytes
        return (size_t) currentSize;
    }

    //we do not have a multiple of the page size, yet, so we must round
    return (size_t) ((((currentSize / pageSize) + 1) * pageSize) - pageSize) + pageSize;
}

size_t roundToWord(int currentSize) {
    int wordSize = SIZEOFWORD;
    printf("Word Size: %d\n", wordSize);
    if ((currentSize / wordSize) * wordSize == currentSize) {
        //we are requesting a multiple of word size bytes
        return (size_t) currentSize;
    }

    //we do not have a multiple of the word size, yet, so we must round
    return (size_t) ((((currentSize / wordSize) + 1) * wordSize) - wordSize) + wordSize;
}

header *worstFit(header *head) {
    //assume that the free list is sorted
    return head;
}

void addHeader (header *head, header *newHeader, header *previous) {
    if (previous == NULL) {
        //Add to start of list
        newHeader->nextHeader = head;
        head = newHeader;
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

void removeFreeHeader (header *head, header *headerToRemove, header *previous) {
    if (previous == NULL) {
        //Remove at start of list
        head = headerToRemove->nextFree;
    } else if (previous->nextHeader == NULL) {
        //Remove at end of list
        previous->nextHeader = NULL;
    } else {
        //Remove at the middle of the list
        previous->nextHeader = headerToRemove->nextFree;
    }
}

void sortList (header *head) {
    //Put the largest available node as the header of the list
    long worstFitValue = head->amountAllocated;
    header *currentHeader = head;
    header *previousHeader = NULL;

    while(currentHeader->nextFree != NULL) {
        if(currentHeader->amountAllocated > worstFitValue) {
            worstFitValue = currentHeader->amountAllocated;
            worstFit = currentHeader;
        }
        previousHeader = currentHeader;
        currentHeader = currentHeader->nextFree;
    }

    if(previousHeader == NULL) {
        //we are done, since our node is already the head
    } else if(currentHeader->nextFree == NULL) {
        previousHeader->nextFree = NULL;
        currentHeader->nextFree = head;
        head = currentHeader;
    } else {
        previousHeader->nextFree = currentHeader->nextFree;
        currentHeader->nextFree = head;
        head = currentHeader;
    }
}
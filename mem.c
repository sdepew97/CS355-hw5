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

//TODO: clarify data structure, above and discuss implementations with rachel (Done)

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
    //TODO: check with Dianna this value tomorrow in OH
    int amountToMmap = roundToPage((sizeOfRegion * 8) + (sizeOfRegion * sizeof(header)) +
                                   sizeof(header)); //worst case what we need to have mmaped to give the user what they requested
    void *mapReturn = mmap(NULL, amountToMmap, PROT_EXEC | PROT_READ | PROT_WRITE,
                           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (*((int *) mapReturn) == ERROR) {
        m_error = E_BAD_POINTER; //TODO: check this is correct error to be raising (check for this...) (check error value for what to return here)
        return ERROR;
    }

    printf("%p\n", mapReturn); //printing pointer?

    //Setup first node in lists (heads)
    //Attach record keeping pointers to the memory
    //TODO: check with Dianna how I am doing this, here and check about how values are stored in memory in layout specifically
    headMainList = mapReturn;
    headFreeList = mapReturn;

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

void *Mem_Alloc(long size) {
    int sizeToWordSize = roundToWord(size); //only allocate word sizes
    int totalSought =
            sizeToWordSize + sizeof(header) + 8; //We need room for the header AND we need room for the word-aligned memory

    //search through the list to get the largest available
    header *worstFitReturn = worstFit(headFreeList); //TODO: fix this function...

    //TODO: think through if the fit is perfect, then only need one header, so that's a problem, since could fit exactly! what I need...otherwise need room for a header (think through cases)

    if (worstFitReturn->amountAllocated < sizeToWordSize) {
        //not enough at all
        m_error = E_NO_SPACE;
        return NULL;
    } else if (worstFitReturn->amountAllocated == sizeToWordSize) {
        //exactly enough, so simply switch free bit to FALSE
        worstFitReturn->free = 'f';
        //TODO: sort list here
        return ((void *) worstFitReturn + sizeof(header)); //TODO: check pointer arithmetic with her, tomorrow
    } else {
        //if there is also room for a header and at least another 8 bytes of memory, then proceed
        if (worstFitReturn->amountAllocated > totalSought) {
            //add a node here!

//            node *newHeader = worstFitReturn + sizeToWordSize + SIZEOFHEADER; //location of new header
//            newHeader->free = TRUE;
//            newHeader->sizeOfRegion = worstFitReturn->sizeOfRegion - SIZEOFHEADER - sizeToWordSize;
//            newHeader->amountAllocated = newHeader->sizeOfRegion - SIZEOFHEADER;
//            newHeader->nextHeader = worstFitReturn->nextHeader;
//            newHeader->prevHeader = worstFitReturn;

            //make header not free anymore

//            worstFitReturn->free = FALSE;
//            worstFitReturn->amountAllocated = sizeToWordSize;
//            worstFitReturn->sizeOfRegion = sizeToWordSize + SIZEOFHEADER;
//            worstFitReturn->nextHeader = newHeader;

//            printf("%p pointer to returned region\n", worstFitReturn + SIZEOFHEADER);
            //TODO: sort list here
            return (void *) worstFitReturn + SIZEOFHEADER; //this is the section where we made room for the memory! :)
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
    return NULL;
}
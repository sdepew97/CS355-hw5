//
// Created by Sarah Depew on 3/24/18.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include "mem.h"

#define SUCCESS 0
#define ERROR -1
#define TRUE 1
#define FALSE 0
#define SIZEOFHEADER 32

int m_error;

//TODO: determine how to track if init has already been called; ask Rachel if solution is acceptable?
typedef unsigned int boolean;

boolean initialized = FALSE;

typedef struct node {
    //TODO: determine the type of information needed here :)
    boolean free;
    void *nextAvailable;
    int lengthOfRegion;
    int amountRequested; //to be returned to the user/what they think they have //TODO: ask Rachel about how to check this??
    int amountAllocated; //to be used for compaction
    void *nextHeader;
    //TODO: ask Rachel what else I could need here?
} node;

node *head;

//TODO: clarify data structure, above and discuss implementations with rachel

//helper functions
size_t roundToPage(int currentSize) {
    //TODO: implement method that rounds the currentSize to a page size
    int pageSize = getpagesize();
    printf("%d\n", pageSize);
    if((currentSize/pageSize)*pageSize == currentSize) {
        //we are requesting a multiple of page size bytes
        return (size_t) currentSize;
    }

    //we do not have a multiple of the page size, yet, so we must round
    return (size_t) ((((currentSize/pageSize)+1)*pageSize)-pageSize) + pageSize;
}

int Mem_Init(long sizeOfRegion) {
    //check sizeOfRegion is a valid, non-negative or zero value
    if(sizeOfRegion <= 0) {
        m_error = E_BAD_ARGS;
        return ERROR;
    }

    //check that Mem_Init hasn't been called more than once
    if(initialized == TRUE) {
        //has already been called once, so this should raise an error
        m_error = E_BAD_ARGS; //TODO: determine if this is the correct error to call here
        initialized = TRUE;
        return ERROR;
    } else {
        initialized = TRUE; //we have called this, now
    }

    //Round memory sizeOfRegion to page size using roundToPage //TODO: ask Rachel about what else to do here

    //Request that much memory from mmap

    //TODO: check on mmap call here and ask what the heck this is doing...
    void *mapReturn = mmap(NULL, roundToPage(sizeOfRegion), PROT_EXEC|PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

//    void *mapReturn = mmap(NULL, sizeOfRegion, PROT_EXEC|PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

    if(*((int *) mapReturn) == ERROR) {
        m_error = E_BAD_POINTER; //TODO: check this is correct error to be raising
        return ERROR;
    }

    printf("%p\n", mapReturn); //printing pointer?

    //Attach record keeping pointers to the memory //TODO: figure out how to do this...
    head = mapReturn;
    head->free = TRUE;
    head->nextAvailable = mapReturn[SIZEOFHEADER]; //TODO: clarify with rachel how this is supposed to go...
    head->amountRequested = sizeOfRegion;
    head->amountAllocated = roundToPage(sizeOfRegion);
    head->nextHeader = NULL;

    printf("head %p", head);
    printf("head free %d", head->free);

    //return the appropriate value
    return SUCCESS;
}

void *Mem_Alloc(long size);

int Mem_Free(void *ptr, int coalesce) {
    if(ptr == NULL) {
        //do nothing
    } else {

        //ptr is not null

        //free memory object ptr points to
    }

    //check for coalesce, now
    if(coalesce == FALSE) {
        //simply can return, now
    } else {
        //do something here, now, since we are asked to coalesce
        //go through the free list and combine memory sections
    }
}

void coalesceList(linkedList *head) {
    //go through free list

    //combine neighboring memory sections
}

void Mem_Dump() {
    //TODO: ask about what to do here...
    //TODO: ask about what if the user requests memory not in an increment of page for sizeOfRegion and how that affects printout here...
}
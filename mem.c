//
// Created by Sarah Depew on 3/24/18.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include "mem.h"
#include "helper.h"
#include "boolean.h"
#include "list.h"

int m_error;
node *head = NULL;

//TODO: clarify data structure, above and discuss implementations with rachel (Done)

int Mem_Init(long sizeOfRegion) {
    //check sizeOfRegion is a valid, non-negative or zero value
    if (sizeOfRegion <= 0) {
        m_error = E_BAD_ARGS;
        return ERROR;
    }

    //check that Mem_Init hasn't been called more than once
    if (head != NULL) {
        //has already been called once, so this should raise an error
        m_error = E_BAD_ARGS; //TODO: determine if this is the correct error to call here
        return ERROR;
    } else {

    }

    //Request that much memory from mmap

    //TODO: check on mmap call here and ask what the heck this is doing... (DONE)
    void *mapReturn = mmap(NULL, roundToPage(sizeOfRegion), PROT_EXEC | PROT_READ | PROT_WRITE,
                           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (*((int *) mapReturn) == ERROR) {
        m_error = E_BAD_POINTER; //TODO: check this is correct error to be raising (check for this...)
        return ERROR;
    }

    printf("%p\n", mapReturn); //printing pointer?

    //Setup first node in list (head)
    //Attach record keeping pointers to the memory //TODO: figure out how to do this...
    head = mapReturn;
    head->free = TRUE;
    head->prevHeader = NULL;
    head->nextHeader = NULL;
    head->sizeOfRegion = sizeOfRegion;
    head->amountAllocated = sizeOfRegion - SIZEOFHEADER;

    printf("head %p\n", head);
    printf("head free %d\n", head->free);
    printf("head next available%p\n", head->nextHeader);

    //return the appropriate value
    return SUCCESS;
}

void *Mem_Alloc(long size);

int Mem_Free(void *ptr, int coalesce) {
    if (ptr == NULL) {
        //do nothing
    } else {

        //ptr is not null

        //free memory object ptr points to
    }

    //check for coalesce, now
    if (coalesce == FALSE) {
        //simply can return, now
    } else {
        //do something here, now, since we are asked to coalesce
        //go through the free list and combine memory sections
        coalesceList(head);
    }
}

void Mem_Dump() {
    //TODO: ask about what to do here...
    //TODO: ask about what if the user requests memory not in an increment of page for sizeOfRegion and how that affects printout here...
    node *currentNode = head;

    while(currentNode != NULL) {
        if(currentNode->free == TRUE) {
            printf("****************\n*\tFREE\t*\n****************\n");
        } else {

        }

        currentNode = currentNode->nextHeader;
    }
}

//helper functions
size_t roundToPage(int currentSize) {
    //TODO: implement method that rounds the currentSize to a page size
    int pageSize = getpagesize();
    printf("%d\n", pageSize);
    if ((currentSize / pageSize) * pageSize == currentSize) {
        //we are requesting a multiple of page size bytes
        return (size_t) currentSize;
    }

    //we do not have a multiple of the page size, yet, so we must round
    return (size_t) ((((currentSize / pageSize) + 1) * pageSize) - pageSize) + pageSize;
}
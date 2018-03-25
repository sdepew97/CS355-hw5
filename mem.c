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

int m_error;

//TODO: determine how to track if init has already been called; ask Rachel if solution is acceptable?
typedef unsigned int boolean;

boolean initialized = FALSE;

typedef struct information {
    //TODO: determine the type of information needed here :)
} information;

typedef struct node {
    struct information *info;
    struct node *next;
    struct node *prev;
} node;

typedef struct linkedList {
    struct node *head;
    struct node *tail;
    unsigned int size;
} linkedList;

static linkedList *free = NULL;
static linkedList *occupied = NULL;

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

    if((int) mapReturn == ERROR) {
        m_error = E_BAD_POINTER; //TODO: check this is correct error to be raising
        return ERROR;
    }

    printf("%p\n", mapReturn); //printing pointer?

    //Attach record keeping pointers to the memory //TODO: figure out how to do this...

    //return the appropriate value
    return SUCCESS;
}

void *Mem_Alloc(long size);

int Mem_Free(void *ptr, int coalesce);

void Mem_Dump();
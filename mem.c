//
// Created by Sarah Depew on 3/24/18.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include "mem.h"

int m_error;

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

int Mem_Init(long sizeOfRegion) {
    //Round memory sizeOfRegion to page size using roundToPage //TODO: ask Rachel about what else to do here

    //Request that much memory from mmap

    //Attach record keeping pointers to the memory //TODO: figure out how to do this...

    //Set E_Bad_Args if needed

    //return the appropriate value
}

void *Mem_Alloc(long size);

int Mem_Free(void *ptr, int coalesce);

void Mem_Dump();

int roundToPage(int currentSize) {
    //TODO: implement method that rounds the
    return 0;
}

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
node *tail = NULL;

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
    int newSizeOfRegion = roundToPage(sizeOfRegion);
    //TODO: check on mmap call here and ask what the heck this is doing... (DONE)
    void *mapReturn = mmap(NULL, newSizeOfRegion, PROT_EXEC | PROT_READ | PROT_WRITE,
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
    head->sizeOfRegion = newSizeOfRegion;
    head->amountAllocated = newSizeOfRegion - SIZEOFHEADER;
    tail = head;

    printf("head %p\n", head);
    printf("head free %d\n", head->free);
    printf("head next available%p\n", head->nextHeader);

    //return the appropriate value
    return SUCCESS;
}

void *Mem_Alloc(long size) {
    int sizeToWordSize = roundToWord(size);
    int totalSought =
            sizeToWordSize + SIZEOFHEADER; //We need room for the header AND we need room for the word-aligned memory

    //search through the list to get the largest available
    node *worstFitReturn = worstFit(head);

    //TODO: think through if the fit is perfect, then only need one header, so that's a problem, since could fit exactly! what I need...otherwise need room for a header (think through cases)

    if (worstFitReturn->amountAllocated < sizeToWordSize) {
        //not enough at all
        m_error = E_NO_SPACE;
        return NULL;
    } else if (worstFitReturn->amountAllocated == sizeToWordSize) {
        //exactly enough, so simply switch free bit to FALSE
        worstFitReturn->free = FALSE;
        return (void *) worstFitReturn + SIZEOFHEADER;
    } else {
        //worstFit->amountAllocated > totalSought

        //if there is also room for a header, then proceed
        if (worstFitReturn->amountAllocated > totalSought) {
            //add a node here!
        }
            //else there is not enough room to split the memory into the section plus header, so fails!
        else {
            //not enough to fit a header AND the memory sought
            m_error = E_NO_SPACE;
            return NULL;
        }
    }


}

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
    //TODO: ask about what if the user requests memory not in an increment of page for sizeOfRegion and how that affects printout here...
    node *currentNode = head;
    int location = 0;

    if(currentNode == NULL) {
        printf("*****************\n*    NO INIT    *\n*****************");
        return;
    }

    while (currentNode != NULL) {
        //print header and then print occupied or free
        printf("*****************\n%d*   HEADER   *%d\n*****************", location, location + SIZEOFHEADER);
        location = location + SIZEOFHEADER + 1;
        if (currentNode->free == TRUE) {
            printf("\n%d*   FREE   *%d\n*****************\n", location,
                   location + currentNode->amountAllocated);
            location = location + currentNode->amountAllocated + 1;
        } else {
            printf("\n%d*   ALLOCATED   *%d\n*****************\n", location,
                   location + currentNode->amountAllocated);
            location = location + currentNode->amountAllocated + 1;
        }
        currentNode = currentNode->nextHeader;
    }
}

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

node *worstFit(node *head) {
    node *currentNode = head;

    if (head == NULL) {
        printf("return here");
        return NULL;
    }

    int sizeOfLargest = 0;
    node *headerOfLargest = NULL;

    while (currentNode != NULL) {
        if (currentNode->free == TRUE) {
            //must be true to check for largest or not
            if (currentNode->amountAllocated > sizeOfLargest) { //TODO: confirm field used here
                sizeOfLargest = currentNode->amountAllocated;
                headerOfLargest = currentNode;
            }
        }
        currentNode = currentNode->nextHeader;
    }

    return headerOfLargest;
}

void coalesceList(node *head) {
    //go through free list
    // combine neighboring memory sections
}
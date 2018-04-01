//
// Created by Sarah Depew on 3/25/18.
//

#ifndef HW5_LIST_H
#define HW5_LIST_H

typedef struct header {
    char checkSum; //unique value to check upon reading next area of memory
    char free; //byte flag of free or not
    struct header *nextHeader; //first linked list
    long amountAllocated; //to be used for compaction
    struct header *nextFree; //second linked list pointers
    //TODO: figure out how to add another check sum here... (ask TA in person)
} header;

extern header *headMainList;
extern header *headFreeList;

//general methods for lists
header *worstFit(header *head);
int checkPadding(header *header1);
int checkFreeSpace(header *header1);

//methods specifically for main list
void addHeaderMain (header **head, header *newHeader, header *previous);
void removeHeaderMain(header **head, header *newHeader, header *previous);
int checkValidPtrMain(header *head, void *ptr);

//methods specifically for free list
void addHeaderFree(header **head, header *headerToRemove, header *previous);
void removeHeaderFree(header **head, header *headerToRemove, header *previous);
int sortFreeList (header **head);
int localCoalesceFree(header **head, header *ptr);
void coalesceFreeList(header *head);
header *findPreviousFree(header *head, header *ptr);

#endif //HW5_LIST_H

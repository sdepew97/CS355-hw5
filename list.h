//
// Created by Sarah Depew on 3/25/18.
//

#ifndef HW5_LIST_H
#define HW5_LIST_H

#define WORSTCASE (roundToPage((sizeOfRegion * SIZEOFWORD) + (sizeOfRegion * sizeof(header)) + sizeof(header) + sizeof(header *) + sizeof(header *) + sizeof(int) + sizeof(long))) //worst case what we need to have mmaped to give the user what they requested
#define ALIGNED //TODO: determine this case
#define AVERAGE //TODO: determine this case

typedef struct header {
    char checkSum; //unique value to check upon reading next area of memory
    char free; //byte flag of free or not
    char padding[6];
    struct header *nextHeader; //first linked list
    long amountAllocated; //to be used for compaction
    struct header *nextFree; //second linked list pointers
} header;

extern header *headMainList;
extern header *headFreeList;

//general methods for lists
int checkPadding(header *header1);

//methods specifically for main list
void addHeaderMain (header **head, header *newHeader, header *previous);
void removeHeaderMain(header **head, header *newHeader, header *previous);
int checkValidPtrMain(header *head, long sizeOfList, void *ptr);
header *findPreviousMain(header *head, header *ptr);

//methods specifically for free list
header *worstFitFree(header **head);
void addHeaderFree(header **head, header *headerToRemove, header *previous);
void removeHeaderFree(header **head, header *headerToRemove, header *previous);
int sortFreeList (header **head);
int localCoalesceFree(header **head, header *ptr);
void coalesceFreeList(header *head);
header *findPreviousFree(header *head, header *ptr);

#endif //HW5_LIST_H

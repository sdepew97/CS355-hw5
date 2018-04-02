#include "mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>

#define NUM_ALLOC 10
#define FREE_FREQ 100     // 1:100
#define BYTE 8

clock_t begin, end;

static void print_execution_time(clock_t begin, clock_t end) {
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Execution time: %.2f seconds\n", time_spent);
}

void test_8_byte() {
  int result = Mem_Init(8504);
  assert(result == 0);

  printf("--------------6 byte allocation-----------------\n");
  void* ptr0 = Mem_Alloc(6);
  if (ptr0 == NULL) {
    exit(EXIT_FAILURE);
  }
  Mem_Dump();
  printf("The pointer ptr0 is at address %p.\n", ptr0);

  printf("--------------14 byte allocation-----------------\n");
  void* ptr1 = Mem_Alloc(14);
  if (ptr1 == NULL) {
    exit(EXIT_FAILURE);
  }
  Mem_Dump();
  printf("The pointer ptr1 is at address %p.\n", ptr1);

  printf("--------------30 byte allocation-----------------\n");
  void* ptr2 = Mem_Alloc(30);
  if (ptr2 == NULL) {
    exit(EXIT_FAILURE);
  }
  Mem_Dump();
  printf("The pointer ptr2 is at address %p.\n", ptr2);

  printf("--------------62 byte allocation-----------------\n");
  void* ptr3 = Mem_Alloc(62);
  if (ptr3 == NULL) {
    exit(EXIT_FAILURE);
  }
  Mem_Dump();
  printf("The pointer ptr3 is at address %p.\n", ptr3);

  printf("--------------126 byte allocation-----------------\n");
  void* ptr4 = Mem_Alloc(126);
  if (ptr4 == NULL) {
    exit(EXIT_FAILURE);
  }
  Mem_Dump();
  printf("The pointer ptr4 is at address %p.\n", ptr4);

  printf("--------------254 byte allocation-----------------\n");
  void* ptr5 = Mem_Alloc(254);
  if (ptr5 == NULL) {
    exit(EXIT_FAILURE);
  }
  Mem_Dump();
  printf("The pointer ptr5 is at address %p.\n", ptr5);

  printf("--------------510 byte allocation-----------------\n");
  void* ptr6 = Mem_Alloc(510);
  if (ptr6 == NULL) {
    exit(EXIT_FAILURE);
  }
  Mem_Dump();
  printf("The pointer ptr6 is at address %p.\n", ptr6);

  printf("--------------1022 byte allocation-----------------\n");
  void* ptr7 = Mem_Alloc(1022);
  if (ptr7 == NULL) {
    exit(EXIT_FAILURE);
  }
  Mem_Dump();
  printf("The pointer ptr7 is at address %p.\n", ptr7);

  printf("--------------2046 byte allocation-----------------\n");
  void* ptr8 = Mem_Alloc(2046);
  if (ptr8 == NULL) {
    exit(EXIT_FAILURE);
  }
  Mem_Dump();
  printf("The pointer ptr8 is at address %p.\n", ptr8);

  printf("--------------4094 byte allocation-----------------\n");
  void* ptr9 = Mem_Alloc(4094);
  if (ptr9 == NULL) {
    exit(EXIT_FAILURE);
  }
  Mem_Dump();
  printf("The pointer ptr9 is at address %p.\n", ptr9);

  end = clock();
  print_execution_time(begin, end);
  
}


int main() {
  test_8_byte();
  return EXIT_SUCCESS;
}
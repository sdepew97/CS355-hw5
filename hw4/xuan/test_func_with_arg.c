
#include "userthread.h"
#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>

typedef struct {int num; char* arg;} PAIR;

void printArg(void* argv){
    PAIR* in = (PAIR* )argv;
    char* word = in->arg;
    int num = in->num;
	//		poll(NULL, 0, 100);
    printf("num: %d; string: %s\n", num, word);
  
	//while(1);{printf("task: %d\n", num);}
}


int testLl(){
	printf("\n\nTesting user functions with arguments\n");
	printf("Should print the number and string passed in\n\n\n");


	thread_libinit(SJF); // FIFO SJF PRIORITY

	PAIR ins, ins2, ins3;
	ins.num = 1;
	ins.arg = "the message1";

	ins2.num = 2;
	ins2.arg = "the message2";

	ins3.num = 3;
	ins3.arg = "the message3";

	//create
	int tid1 = thread_create(printArg, &ins, 0);
	int tid2 = thread_create(printArg, &ins2, -1);
	int tid3 = thread_create(printArg, &ins3, 1);


	// join
	thread_join(tid1);
	thread_join(tid2);
	thread_join(tid3);


	// term
	thread_libterminate();

	return 1;
}


int main(){
  testLl();
  exit(EXIT_SUCCESS);
}

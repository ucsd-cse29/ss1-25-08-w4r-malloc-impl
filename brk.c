#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main() {
	int* nums = malloc(12);
	printf("%p\n", nums);
	for(int* a = nums; 1; a += 1) {
		*a = 777;
		// read all of memory
	}
	void* start_of_heap = sbrk(10000);
	printf("%p\n", start_of_heap);


}

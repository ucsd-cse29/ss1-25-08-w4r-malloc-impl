#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

void* HEAP_START = NULL;
uint64_t* current = NULL;

void* mymalloc(size_t size) {
	if(HEAP_START == NULL) {
		HEAP_START = sbrk(1000);
		current = HEAP_START;
	}

	uint64_t block_size;
	if(size % 8 == 0) { block_size = size; }
	else { block_size = size + (8 - (size % 8)); }

	printf("Rounded block size: %ld %ld\n", size, block_size);
	uint64_t header = block_size | 1;
	uint64_t* block_location = current;
	block_location[0] = header;
	
	current += 1 + (block_size / 8); // this would move current by 3 * 8 bytes

	return block_location + 1;
}

void myfree(void* ptr) {
	// EXAMPLE: get address 0x..20 and we set the LSB at address
	// 0x18 to be 0
	uint64_t* p = ptr - 8; // because sizeof(void) is 1, this subtracts 8
	p[0] = p[0] - 1;
}

void print_heap() {
	uint64_t* current = HEAP_START;
	uint64_t bytes_seen = 0;
	while(bytes_seen < 1000 && *current != 0) {
		uint64_t size = *current & (~1);
		if(*current & 1) {
			printf("%ld (allocated)\n", size);
		}
		else {
			printf("%ld (free)\n", size);
		}
		current += (size / 8) + 1;
		bytes_seen += size + 8;
	}
}

int main() {

	char* a = mymalloc(13);
	printf("HEAP_START vs a: %p %p\n", HEAP_START, a);
	printf("first heap loc: 0x%lx\n", *(uint64_t*)HEAP_START);
	char* b = mymalloc(19);
	printf("HEAP_START vs b: %p %p\n", HEAP_START, b);
	char* c = mymalloc(8);
	printf("HEAP_START vs c: %p %p\n", HEAP_START, c);
	uint64_t* b_header = (uint64_t*)(b - 8);
	printf("b's header: %ld 0x%lx\n", b_header[0], b_header[0]);
	myfree(b);
	printf("b's header: %ld 0x%lx\n", b_header[0], b_header[0]);

	char* d = mymalloc(20); // should use b's space!
				// But it just goes forward!
				// What to do?

	print_heap();

}

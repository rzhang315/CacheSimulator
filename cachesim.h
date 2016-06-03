#ifndef __CACHESIM_H
#define __CACHESIM_H
typedef unsigned long long addr_t;
typedef unsigned long long counter_t;

//The stack have a size value which correspond to the associativities, and an array of 100 which stores the tag value.
typedef struct stack{
	int size;
	int array[128];
}stack;

//The set have a size value which correspond to the associativities, and three arrays of size 100 which stores the tag value, valid bit, and dirty bit
typedef struct set{
	int size;
	int tag[128];
	int valid[128];
	int dirty[128];
}set;



void cachesim_init(int block_size, int cache_size, int ways);
void cachesim_access(addr_t physical_add, int write);

void cachesim_print_stats(void);

#endif

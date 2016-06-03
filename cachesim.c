#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cachesim.h"

#include <math.h> // add log function

counter_t accesses = 0, hits = 0, misses = 0, writebacks = 0, write_access=0, write_misses=0;
int writeback_volume;
int memory_access_volume;

set * s;
stack * st;
int block_value;
int set_index;
int tag_value;
int block_length;
int index_length;
int tag_length;

int hitIndex;
int i,j,k;
int write_number;
int N;
int assoc;
int block_size;
int cache_size;
int set_total;

void cachesim_init(int blocksize, int cachesize, int ways) {
    
	block_length = log(blocksize) / log(2);  //calculate the length of bit for block	
	set_total = cachesize / blocksize / ways;	 //calculate how many index there are
	index_length = log(set_total) / log(2);  //calculate the length of bit for index	
	tag_length = 32 - block_length - index_length; //calculate the length of bit for tag
	
	//printf("block=%d\n",block_length);
	//printf("index=%d\n",index_length);
	//printf("tag=%d\n",tag_length);


	st = (stack*)malloc(sizeof(stack)*set_total);
	s = (set*)malloc(sizeof(set)*set_total);
	
	
	for (i = 0; i < set_total; i++){
		//initialize each stack with -1 (tag_value);
		
		st[i].size = ways;
		for ( j = 0; j < ways; j++){
			st[i].array[j] = -1;
		}
		
		//initialize each set to -1 , 0 , 0;
		s[i].size = ways; 
		for ( k = 0; k < ways; k++){
			s[i].tag[k] = -1;
			s[i].dirty[k] = 0;
			s[i].valid[k] = 0;
		}
		
	}
	
	assoc = ways;
	block_size = blocksize;
	cache_size = cachesize;
	
}



void cachesim_access(addr_t physical_addr, int write) {
	
	
	accesses++;
	if (write == 1){
			write_access++; //increment write access
	}
	
	
	//bitwise AND operation to get the address, used 1s to get rid off byte offset
	block_value = (physical_addr) & ((1<<block_length) - 1); 
	set_index = (physical_addr >> (block_length))&((1<<index_length) - 1);
	tag_value = (physical_addr >> (block_length + index_length ))&((1<<tag_length) - 1);
	
	
	//printf("block value=%d\n",block_value);
	
	//printf("block length=%d\n",block_length);
	
	//printf("index_length=%d\n",index_length);
	
	//printf("set_value=%d\n",set_index);
	//printf("tag_value=%d\n",tag_value);
	
	
	int checkHit = 0;
	int hitIndex = -1;
	
	for (i = 0; i < assoc; i++){
		if ((s[set_index].tag[i] == tag_value) && (s[set_index].valid[i] == 1)){
			checkHit = 1;
		}
	}
	for (j = 0; j < assoc; j++){
		if (st[set_index].array[j] == tag_value){
			hitIndex = j;
		}
	}
	
	
	int removed_value;
	//int remove_index;
	
    //update the cache
	


	if ((hitIndex !=-1)){  //hit and then update the cache
		
		//printf("hit");
		
		//remove_index = hitIndex;
		
		removed_value = st[set_index].array[hitIndex]; //update the LRU by first finding the hit value
		for (i = 0; i < assoc; i++){
			if (i >= hitIndex){
				st[set_index].array[i] = st[set_index].array[i + 1]; //simply shifting the array to the left
			}
		}
		st[set_index].array[assoc - 1] = removed_value; //move the hit value to the end(most used value)
		
		//if it is a hit already, then writeback is not necessary	
			
		//update the set accordingly; 
		
		for ( i = 0; i < assoc; i++){
		if (s[set_index].tag[i] == removed_value){
			//s[set_index].tag[i] = tag_value;
			s[set_index].valid[i] = 1;
			if (write == 1){
				s[set_index].dirty[i] = 1;
			}
		}
		}
		
		
		
		
		
		hits++;
		
		
	}
	
	// missed
	else {
		// printf("miss");
		// update the stack
		
		
		memory_access_volume = memory_access_volume + block_size;
		
		//remove_index = 0;
		removed_value = st[set_index].array[0];
		for (i = 0; i < assoc; i++){
			if (i >= 0){
				st[set_index].array[i] = st[set_index].array[i + 1];
			}
		}
		st[set_index].array[assoc - 1] = tag_value;
		
		//update the writeback when misses;
		
		
		int remove_set_index=-1;
		for ( i = 0; i < assoc; i++){
			if (s[set_index].tag[i] == removed_value){
				remove_set_index = i;
			}
		}
		
		if (s[set_index].dirty[remove_set_index] == 1){
			writeback_volume = writeback_volume + block_size;
			writebacks++;
			s[set_index].dirty[remove_set_index] = 0;
		}
		
		//update the set;
		//if write then set dirty to 1
		
		
			s[set_index].tag[remove_set_index] = tag_value;
			s[set_index].valid[remove_set_index] = 1;
			if (write == 1){
				s[set_index].dirty[remove_set_index] = 1;
			}
		
		
		misses++;
		
		if (write == 1){
			write_misses++;
		}
		
	}
	
	
	
	
	
}


void cachesim_print_stats() {
  printf("%llu, %llu, %llu, %llu\n", accesses, hits, misses, writebacks);
  /*
    FILE * outputfp = fopen("P5out	.csv", "a");
	if (outputfp == NULL) 
    {
      fprintf(stderr, "Can't open output file!\n");
      exit(1);
    }
  
  fprintf(outputfp, "%d,%d,%d,%llu,%llu,%llu,%llu,%llu,%llu, %d, %d", assoc , block_size , cache_size, misses, accesses, 
  (misses-write_misses),(accesses-write_access), write_misses, write_access, writeback_volume, memory_access_volume); 
  fprintf(outputfp, "\n");  //print the result into csv
  */ 
  
}

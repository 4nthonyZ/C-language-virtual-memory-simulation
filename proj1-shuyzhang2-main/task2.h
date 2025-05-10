#ifndef TASK2_H
#define TASK2_H

#include <stdint.h>
#include <stdio.h>

#define PAGE_TABLE_SIZE 1024
#define FRAME_COUNT 256
#define PAGE_SIZE 4096

typedef struct {
    int valid;
    int frame_number;
} PageTableEntry;

void run_task2(const char *filename);


int page_table_lookup(PageTableEntry *table, int page_number,
                      int *page_fault, int *allocated_pages, int *next_free_frame);

#endif

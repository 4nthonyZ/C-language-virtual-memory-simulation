#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "task1.h"  
#include "task2.h"  
#include "task3.h"

void run_task3(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file");
        exit(1);
    }

    PageTableEntry page_table[PAGE_TABLE_SIZE] = {0};

    int fifo_queue[FRAME_COUNT];  
    for (int i = 0; i < FRAME_COUNT; i++) fifo_queue[i] = -1;

    int fifo_head = 0;           
    int allocated_pages = 0;   
    int next_free_frame = 0;
    //init the fifo
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        int logical_address = (int) strtoul(line, NULL, 10);
        AddressComponents addr = parse_logical_address(logical_address);

        int page_fault = 0;
        int frame_number;
        int evicted_page = -1;

        if (page_table[addr.page_number].valid) {
        // Page table hit, no need to replace

            frame_number = page_table[addr.page_number].frame_number;
            page_fault = 0;
        } else {
            page_fault = 1;
            //If the frame is not full
            if (allocated_pages < FRAME_COUNT) {
                frame_number = page_table_lookup(
                    page_table, addr.page_number,
                    &page_fault, &allocated_pages, &next_free_frame
                );
                fifo_queue[allocated_pages - 1] = addr.page_number;
            } else {
                evicted_page = fifo_queue[fifo_head];
                frame_number = page_table[evicted_page].frame_number;
                page_table[evicted_page].valid = 0;
                fifo_queue[fifo_head] = addr.page_number;
                fifo_head = (fifo_head + 1) % FRAME_COUNT;
                page_table[addr.page_number].valid = 1;
                page_table[addr.page_number].frame_number = frame_number;
                //If the frame is full, FIFO replacement starts
            }
        }

        int physical_address = frame_number * PAGE_SIZE + addr.offset;
    //print output
        printf("logical-address=%u,page-number=%u,offset=%u\n",
               logical_address, addr.page_number, addr.offset);

        if (evicted_page != -1) {
            printf("evicted-page=%d,freed-frame=%d\n", evicted_page, frame_number);
        }

        printf("page-number=%u,page-fault=%d,frame-number=%d,physical-address=%u\n",
               addr.page_number, page_fault, frame_number, physical_address);
    }

    fclose(fp);
}

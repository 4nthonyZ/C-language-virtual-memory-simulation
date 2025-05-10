#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "task1.h"
#include "task2.h"
#include "task4.h"

#define TLB_SIZE 32

typedef struct {
    int valid;
    int page_number;
    int frame_number;
    unsigned int last_used;
} TLBEntry;

static TLBEntry tlb[TLB_SIZE];
static unsigned int usage_counter = 0;

// Search TLB for the page, update usage if found
int tlb_lookup(int page_number, int *frame_number) {
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb[i].valid && tlb[i].page_number == page_number) {
            *frame_number = tlb[i].frame_number;
            tlb[i].last_used = ++usage_counter;
            return 1;
        }
    }
    return 0;
}

// Insert or replace using LRU policy
int tlb_insert(int page_number, int frame_number) {
    int index = -1;
    unsigned int least_used = ~0u;

    for (int i = 0; i < TLB_SIZE; i++) {
        if (!tlb[i].valid) {
            index = i;
            break;
        }
        if (tlb[i].last_used < least_used) {
            least_used = tlb[i].last_used;
            index = i;
        }
    }

    int removed_page = (tlb[index].valid) ? tlb[index].page_number : -1;

    tlb[index].valid = 1;
    tlb[index].page_number = page_number;
    tlb[index].frame_number = frame_number;
    tlb[index].last_used = ++usage_counter;

    return removed_page;
}

void run_task4(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file");
        exit(1);
    }

    PageTableEntry page_table[PAGE_TABLE_SIZE] = {0};
    int fifo_queue[FRAME_COUNT];
    for (int i = 0; i < FRAME_COUNT; i++) fifo_queue[i] = -1;
    for (int i = 0; i < TLB_SIZE; i++) tlb[i].valid = 0;

    int fifo_head = 0;
    int allocated_pages = 0;
    int next_free_frame = 0;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        int logical_address = (int)strtoul(line, NULL, 10);
        AddressComponents addr = parse_logical_address(logical_address);

        int page_fault = 0;
        int frame_number = -1;
        int evicted_page = -1;

        printf("logical-address=%u,page-number=%u,offset=%u\n",
               logical_address, addr.page_number, addr.offset);

        if (tlb_lookup(addr.page_number, &frame_number)) {
            int physical_address = frame_number * PAGE_SIZE + addr.offset;
            printf("tlb-hit=1,page-number=%u,frame=%d,physical-address=%u\n",
                   addr.page_number, frame_number, physical_address);
            continue;
        } else {
            printf("tlb-hit=0,page-number=%u,frame=none,physical-address=none\n", addr.page_number);

            // If not in TLB, check page table
            if (page_table[addr.page_number].valid) {
                frame_number = page_table[addr.page_number].frame_number;
            } else {
                page_fault = 1;

                // Allocate or replace frame using FIFO
                if (allocated_pages < FRAME_COUNT) {
                    frame_number = next_free_frame;
                    fifo_queue[allocated_pages] = addr.page_number;
                    allocated_pages++;
                    next_free_frame++;
                } else {
                    evicted_page = fifo_queue[fifo_head];
                    frame_number = page_table[evicted_page].frame_number;
                    page_table[evicted_page].valid = 0;
                    printf("evicted-page=%d,freed-frame=%d\n", evicted_page, frame_number);
                    for (int i = 0; i < TLB_SIZE; i++) {
                        if (tlb[i].valid && tlb[i].page_number == evicted_page) {
                            tlb[i].valid = 0;
                    
                            int valid_count = 0;
                            for (int j = 0; j < TLB_SIZE; j++) {
                                    if (tlb[j].valid) valid_count++;
                            }
                    
                            printf("tlb-flush=%d,tlb-size=%d\n", evicted_page, valid_count);
                            break;
                        }
                    }
                    fifo_queue[fifo_head] = addr.page_number;
                    fifo_head = (fifo_head + 1) % FRAME_COUNT;
                }

                page_table[addr.page_number].valid = 1;
                page_table[addr.page_number].frame_number = frame_number;
            }

            // Insert into TLB
            int removed = tlb_insert(addr.page_number, frame_number);
            if (removed == -1)
                printf("tlb-remove=none,tlb-add=%u\n", addr.page_number);
            else
                printf("tlb-remove=%d,tlb-add=%u\n", removed, addr.page_number);
        }

        int physical_address = frame_number * PAGE_SIZE + addr.offset;
        printf("page-number=%u,page-fault=%d,frame-number=%d,physical-address=%u\n",
               addr.page_number, page_fault, frame_number, physical_address);
    }

    fclose(fp);
}
//1
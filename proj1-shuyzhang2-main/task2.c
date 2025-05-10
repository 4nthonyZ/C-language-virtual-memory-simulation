#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "task1.h"
#include "task2.h"

// Allocates a frame for a page if it's not valid yet.
// Sets page_fault to 1 if allocation happens.
int page_table_lookup(PageTableEntry *table, int page_number,
                      int *page_fault, int *allocated_pages, int *next_free_frame) {
    if (table[page_number].valid) {
        *page_fault = 0;
        return table[page_number].frame_number;
    }

    *page_fault = 1;

    if (*allocated_pages >= FRAME_COUNT) {
        fprintf(stderr, "ERROR: Exceeded frame capacity in Task 2 (page %u)\n", page_number);
        exit(1);
    }

    int frame = *next_free_frame;
    table[page_number].valid = 1;
    table[page_number].frame_number = frame;

    (*allocated_pages)++;
    (*next_free_frame)++;

    return frame;
}

void run_task2(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file");
        exit(1);
    }

    PageTableEntry page_table[PAGE_TABLE_SIZE] = {0};
    int next_free_frame = 0;
    int allocated_pages = 0;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        int logical_address = (int) strtoul(line, NULL, 10);
        AddressComponents addr = parse_logical_address(logical_address);

        int page_fault = 0;
        // Use the helper to get or assign a frame for the page
        int frame_number = page_table_lookup(
            page_table, addr.page_number,
            &page_fault, &allocated_pages, &next_free_frame
        );

        int physical_address = frame_number * PAGE_SIZE + addr.offset;

        printf("logical-address=%u,page-number=%u,offset=%u\n",
               logical_address, addr.page_number, addr.offset);

        printf("page-number=%u,page-fault=%d,frame-number=%d,physical-address=%u\n",
               addr.page_number, page_fault, frame_number, physical_address);
    }

    fclose(fp);
}

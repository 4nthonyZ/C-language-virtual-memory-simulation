#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "task1.h"

#define MASK_22_BITS 0x003FFFFF
#define OFFSET_BITS 12

// Extracts page number and offset from a 32-bit logical address.
AddressComponents parse_logical_address(int logical_address) {
    int masked = logical_address & MASK_22_BITS;
    int page_number = masked >> OFFSET_BITS;
    int offset = masked & ((1 << OFFSET_BITS) - 1);

    AddressComponents result = { page_number, offset };
    return result;
}

void run_task1(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        int logical_address = (int) strtoul(line, NULL, 10);
        AddressComponents addr = parse_logical_address(logical_address);

        // Print address breakdown
        printf("logical-address=%u,page-number=%u,offset=%u\n",
               logical_address, addr.page_number, addr.offset);
    }

    fclose(fp);
}

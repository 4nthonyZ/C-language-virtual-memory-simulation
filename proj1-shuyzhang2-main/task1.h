#ifndef TASK1_H
#define TASK1_H

#include <stdint.h>  

typedef struct {
    int page_number;
    int offset;
} AddressComponents;

void run_task1(const char *filename);

AddressComponents parse_logical_address(int logical_address);

#endif

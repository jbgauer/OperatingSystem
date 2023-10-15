/*
 * paging.h - Definitions for paging
*/

#include "types.h"

typedef struct {
    uint32_t s_addr;
} page_t;

page_t page_init();

page_t* paging_init();

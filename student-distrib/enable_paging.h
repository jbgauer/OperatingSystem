/* enable_paging.h - declares assembly functions written in enable_paging.S
 * 
 */
#ifndef _ENABLE_PAGING_H
#define _ENABLE_PAGING_H

extern void enable_paging();
extern void load_page_directory(uint32_t*);

#endif

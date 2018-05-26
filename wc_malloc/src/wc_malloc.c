/*
 wc_malloc.c - Experimental memory performance
 
 Copyright (C) Chris Ison 2018
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * Developer Notes
 *
 * Error Checking:
 * 		Limited checking is done as each check is a potential bottle
 * 		neck. The idea is to strip away as much as possible to gain as
 * 		much performance as possible. Much care should be given in avoiding
 * 		situations where more error checking is needed here.
 *
 * Experimental:
 * 		Because of the experimental nature of this it should not be used
 * 		in critical systems as issues are expected (even if not encountered).
 *
 * Two potential failure points:
 * 		First of these failure points is running out ram to assign more
 * 		chunk data. Second is running out of ram to assign more chunks.
 * 		Both failures will result in NULL being returned when calling
 * 		wc_[malloc|calloc|realloc]. In the case of wc_realloc the data pointed
 * 		to by ptr is unchanged.
 *
 * Zero size equals NULL return:
 * 		With wc_[malloc|calloc|realloc] requesting a size of 0 will return
 * 		a NULL result. With wc_realloc a 0 size request will be treated as
 * 		wc_free(ptr). with wc_calloc a 0 nmem also results in a NULL return.
 *
 * Expected behaviour:
 * 		TODO: void wc_free(void *ptr); releases memory pointed to by ptr back into
 * 		the pool.
 *
 * 		void *wc_malloc(size_t size); returns a pointer to an amount of memory
 * 		allocated equal to size bytes. Returns NULL on failure, or if size is 0.
 *
 * 		void *wc_calloc(size_t nmem, size_t size); returns a pointer to an
 * 		amount of memory allocated equal to nmem * size bytes. Returns NULL
 * 		on failure, or if nmem or size is 0.
 *
 * 		void *wc_realloc(void *ptr, size_t size); returns a pointer to an amount
 * 		of memory allocated equal to size bytes. if ptr != NULL it will change
 * 		the allocated amount to size bytes, and if the return address is different
 * 		will copy contents of ptr to new location and release ptr. On failure
 * 		returns NULL and the data in ptr remains unchanged and accessible.
 * 		If size is 0 it behaves as if wc_free(ptr) were called;
 *
 * 		See each function for each individual internal behavior.
 */

//#define _POSIX_C_SOURCE
#include <errno.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>

#include "wc_malloc.h"

void *base_heap = NULL;
void *heap_end = NULL;
void *soft_heap_end = NULL;

#define MIN_PAGE_GRAB 3

size_t page_size = 4096;

#define _HHEADER_SIZE sizeof(unsigned long)
unsigned long msbL = 0;
unsigned long scan_filter = 0;

int wc_malloc_init() {
	unsigned long new_size = 0;

	msbL = 1L << ((_HHEADER_SIZE * 8L) - 1L);
	scan_filter = msbL - 1L;
	page_size = sysconf(_SC_PAGE_SIZE);
	new_size = page_size * MIN_PAGE_GRAB;
	base_heap = sbrk(new_size);
	heap_end = base_heap + new_size;
	*(unsigned long *)base_heap = new_size;
	soft_heap_end = heap_end;

	return 0;
}

void *find_first_fit(size_t size) {
	int new_pages = 0;
	unsigned long new_size = 0;
	unsigned long scan_size = size + _HHEADER_SIZE;
	unsigned long heap_item_size = 0;
	void *scan_heap = NULL;
	void *tmp_heap_end = NULL;

	scan_heap = base_heap;
	do {
		heap_item_size = *(unsigned long *) scan_heap & scan_filter;
		// is heap item free
		if (*(unsigned long *) scan_heap <= scan_filter) {
			// is heap item big enough?
			if (scan_size <= heap_item_size) {
				*(unsigned long *) scan_heap = (unsigned long) scan_size | msbL;

				// do we have room to split?
				heap_item_size -= scan_size;
				if ((_HHEADER_SIZE * 2) <= heap_item_size) {
					// split it out
					*(unsigned long *)(scan_heap + scan_size) = heap_item_size;
				} else {
					soft_heap_end = scan_heap + scan_size;
				}
				// return writable heap location
				return (scan_heap + _HHEADER_SIZE);
			}
		}
		scan_heap += heap_item_size;
	} while (scan_heap < soft_heap_end);

	// no large enough free space found so lets get space from the system.
	new_pages = (scan_size / page_size) + MIN_PAGE_GRAB;
	new_size = new_pages * page_size;
	tmp_heap_end = sbrk(new_size);

	if (tmp_heap_end == (void *) -1) {
		return NULL;
	}

	*(unsigned long *) (scan_heap + scan_size) = (unsigned long) (new_size
			+ (heap_end - soft_heap_end) - scan_size);
	*(unsigned long *) scan_heap = scan_size | msbL;
	heap_end += new_size;
	soft_heap_end = heap_end;

	return (scan_heap + _HHEADER_SIZE);
}

void wc_free(void *ptr) {
	void *tmp_ptr = NULL;
	long size = 0;
	if ((ptr < base_heap) || (ptr > heap_end)) {
		errno = EFAULT;
		signal(SIGSEGV, SIG_ERR);
		return;
	}
	tmp_ptr = ptr - _HHEADER_SIZE;
	*(long *) tmp_ptr &= scan_filter;
}

void *wc_malloc(size_t size) {
	if (size == 0)
		return NULL;
	return find_first_fit(size);
}

void *wc_realloc(void *ptr, size_t size) {
	void *ret_ptr  = NULL;

	// If size is zero treat as free
	if (size == 0) {
		if (ptr != NULL) {
			wc_free(ptr);
		}
		return NULL;
	} else {
		// feeling lazy, give'm a new address
		ret_ptr = wc_malloc(size);
		if (ptr != NULL) {
			memmove(ret_ptr, ptr, (((long)(ptr - _HHEADER_SIZE)) & scan_filter));
			wc_free(ptr);
		}
		return ret_ptr;
	}
}

void *wc_calloc(size_t nmemb, size_t size) {
	if ((nmemb == 0) || (size == 0))
		return NULL;

	return wc_malloc(nmemb * size);
}


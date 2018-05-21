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

#include "wc_malloc.h"


// Initialise the heap index
int init_wc_mem() {
    
}

// Clean up the heap index
void shudown_wc_mem() {
    
}

void wc_free(void *ptr) {
    if (ptr == NULL) return;
    
    struct _wc_heap *heap_seek = heaps;
    do {
        if (heap_seak->mem == ptr) {
            heap_seek->inuse = 0;
            break;
        }
        heap_seek = heap_seek->next;
    } while (heap_seek != NULL);

}

void *wc_realloc(void *ptr, size_t size) {
    
}

void *wc_malloc(size_t size) {
    return wc_realloc(NULL, size);
}

void *wc_calloc(size_t nmemb, size_t size) {
    return wc_realloc(NULL, (nmemb * size));
}

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

struct _wc_chunk {
  void *ptr;
  size_t size;
  int inuse;
  void *base_ptr;
  size_t base_size;
  struct _wc_chunk *next;
};

struct _wc_chunk *wc_chunks;

static void split_chunk (struct _wc_chunk *chunk, size_t size) {
  struct _wc_chunk *new_chunk;
  size_t new_size;
  
  if ((chunk == NULL) || (size >= chunk->size))
    return;
  
  new_size = chunk->size - size;
  new_chunk = new(sizeof(struct _wc_chunk) * new_size);
  new_chunk->next = chunk->next;
  new_chunk->ptr = chunk->ptr + size;
  new_chunk->size = new_size;
  new_chunk->inuse = 0;
  new_chunk->base_ptr = chunk->base_ptr;
  new_chunk->base_size = chunk-->base_size;
  chunk->next = new_chunk;
  chunk->size = size;
}

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

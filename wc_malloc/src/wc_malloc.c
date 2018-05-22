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

#include <sys/mman.h>
#include <unistd.h>

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

// Only call this if we truely need more ram.
static struct _wc_chunk *_wc_new_chunk (size_t size) {
    // grap ram for pointer
    struct _wc_chunk *chunk = mmap(NULL, sizeof(struct _wc_chunk), PROT_READ | PROT_WRITE, MAP_ANONYMOUS, 0, 0);
   
    // we want enough pages of ram to fit data
    int page_size = sysconf(_SC_PAGESIZE);
    chunk->base_size = ((size / page_size) + 1) * page_size;
    
    // grab ram
    chunk->base_ptr = mmap(NULL, chunk->base_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, 0, 0);
    
    // set up the pointer
    chunk->ptr = chunk->base_ptr;
    chunk->size = chunk->base_size;
    chunk->inuse = 0;
    
    // make pointer first on list
    chunk->next = wc_chunks;
    wc_chunks = chunk;
    return chunk;
}

// joins 2 adjacent "empty" chunks belonging to same base together
static void check_next_chunk(struct _wc_chunk *chunk) {
    // make sure we have something to check;
    struct _wc_chunk *next_chunk = NULL;
    if (chunk == NULL)
        return;
    
    if (chunk->next == NULL)
        return;
    
    next_chunk = chunk->next;
    
    // is the next chunk on the list from the same mmap call?
    if (chunk->base_ptr == next_chunk->base_ptr) {
        //is the next chunk "empty"
        if (!next_chunk->inuse) {
            
            // lets join this together
            chunk->size += next_chunk->size;
            chunk->next = next_chunk->next;
            
            // remove the extra pointer
            munmap(next_chunk, sizeof(struct _wc_chunk));
        }
    }
}

static void split_chunk (struct _wc_chunk *chunk, size_t size) {
  struct _wc_chunk *new_chunk;
  size_t new_size;
  
  if ((chunk == NULL) || (size >= chunk->size))
    return;
  
  new_size = chunk->size - size;
  new_chunk = mmap(NULL, sizeof(struct _wc_chunk), PROT_READ | PROT_WRITE, MAP_ANONYMOUS, 0, 0);
  new_chunk->next = chunk->next;
  new_chunk->ptr = chunk->ptr + size;
  new_chunk->size = new_size;
  new_chunk->inuse = 0;
  new_chunk->base_ptr = chunk->base_ptr;
  new_chunk->base_size = chunk->base_size;
  chunk->next = new_chunk;
  chunk->size = size;
}

struct _wc_chunk *fit_chunk (size_t size) {
    struct _wc_chunk *seek_chunk = wc_chunks;
    
    do {
        if (!seek_chunk->inuse) {
            if (seek_chunk->size >= size) {
                break;
            }
        }
        seek_chunk = seek_chunk->next;
    } while (seek_chunk != NULL);
    
    return seek_chunk;
}

// Initialise the heap index
int init_wc_mem() {
    _wc_new_chunk(sysconf(_SC_PAGESIZE));
    return 0;
}

// Clean up the heap index
void shudown_wc_mem() {
    struct _wc_chunk *seek_chunk = wc_chunks;
    struct _wc_chunk *next_chunk = NULL;
    void *prev_ptr = NULL;
    do {
        next_chunk = seek_chunk->next;
        if (seek_chunk->base_ptr != prev_ptr) {
            prev_ptr = seek_chunk->base_ptr;
            munmap(seek_chunk->base_ptr, seek_chunk->base_ptr);
        }
        munmap(seek_chunk, sizeof(struct _wc_chunk));
        seek_chunk = next_chunk;
    } while (seek_chunk != NULL);
}

// return the chunk data that contains *ptr
static struct _wc_chunk *get_chunk (void *ptr) {
    struct _wc_chunk *chunk_seek = wc_chunks;
    
    if (ptr == NULL)
        return NULL;
    
    do {
        if (chunk_seek->ptr == ptr) {
            return chunk_seek;
        }
        chunk_seek = chunk_seek->next;
    } while (chunk_seek != NULL);
    return NULL;
}

void wc_free(void *ptr) {
    struct _wc_chunk *chunk_seek = wc_chunks;
    struct _wc_chunk *prev_chunk = NULL;
    
    if (ptr == NULL)
        return;
    
    do {
        if (chunk_seek->ptr == ptr) {
            chunk_seek->inuse = 0;
            // check if next chunk from same mmap call and free
            check_next_chunk(chunk_seek);
            // check if previous chunk form same mmap call and free
            check_next_chunk(prev_chunk);
            // no more to do here
            return;
        }
        prev_chunk = chunk_seek;
        chunk_seek = chunk_seek->next;
    } while (chunk_seek != NULL);

}

void *wc_realloc(void *ptr, size_t size) {
    struct _wc_chunk *old_chunk = NULL;
    struct _wc_chunk *chunk = NULL;
    struct _wc_chunk *new_chunk = NULL;
    size_t new_size = 0;
    
    if (!size)
        return NULL;
    
    if (ptr) {
        
        old_chunk = get_chunk(ptr);
        check_next_chunk(old_chunk);
        
    } else {
        chunk = fit_chunk(size);
        if (chunk == NULL) {
            chunk = _wc_new_chunk(size);
        }
    }
    
    if (size < chunk->size) {
        new_size = chunk->size - size;
        new_chunk = mmap(NULL, sizeof(struct _wc_chunk), PROT_READ | PROT_WRITE, MAP_ANONYMOUS, 0, 0);
        new_chunk->next = chunk->next;
        new_chunk->ptr = chunk->ptr + size;
        new_chunk->size = new_size;
        new_chunk->inuse = 0;
        new_chunk->base_ptr = chunk->base_ptr;
        new_chunk->base_size = chunk->base_size;
        chunk->next = new_chunk;
        chunk->size = size;
    }
    chunk->inuse = 1;
    return chunk;
}

void *wc_malloc(size_t size) {
    return wc_realloc(NULL, size);
}

void *wc_calloc(size_t nmemb, size_t size) {
    return wc_realloc(NULL, (nmemb * size));
}

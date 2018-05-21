/*
  wc_malloc.h - Experimental memory performance
 
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

#ifndef wc_malloc_h
#define wc_malloc_h

#include <stdio.h>

extern int init_wc_mem();

extern void *wc_malloc(size_t size);
extern void *wc_realloc(void *ptr, size_t size);
extern void *wc_calloc(size_t nmemb, size_t size);
extern void wc_free(void *ptr);

extern void shudown_wc_mem();

#endif /* wc_malloc_h */

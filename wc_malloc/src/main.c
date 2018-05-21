/*
  main.c - Experimental memory performance
 
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

#include <stdio.h>
#include <time.h>

#include "wc_malloc.h"

void *buffer[10000] = { NULL };

void test_sys_malloc() {
    int i = 0;
    
    printf("Testing malloc/free\r\n");
    
    printf("Start: %li\r\n", time(NULL));
    
    for (i = 0; i < 10000; i++) {
        buffer[i] = malloc(128);
    }
    for (i = 0; i < 10000; i++) {
        free(buffer[i]);
    }
    
    printf("Stop: %li\r\n", time(NULL));
    
}

void test_wc_malloc() {
    int i = 0;
    
    printf("Testing wc_malloc/free\r\n");
    
    printf("Start: %li\r\n", time(NULL));
    
    init_wc_mem();
    for (i = 0; i < 10000; i++) {
        buffer[i] = wc_malloc(128);
    }
    for (i = 0; i < 10000; i++) {
        wc_free(buffer[i]);
    }
    shutdown_wc_mem();
    
    printf("Stop: %li\r\n", time(NULL));
    
}



int main(int argc, char *argv[]) {
    
    test_sys_malloc();
    
    test_wc_malloc();
    
    return 0;
}

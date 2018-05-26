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
#include <sys/time.h>
#include <stdlib.h>
#include <strings.h>

#include "wc_malloc.h"

void *buffer[10000] = { NULL };

void test_sys_malloc() {
    int i = 0;
    
    struct timeval time_before, time_after, time_result;
    
    gettimeofday(&time_before, NULL);
    for (i = 0; i < 10000; i++) {
        buffer[i] = malloc(128);
        bzero(buffer[i], 128);
    }
    gettimeofday(&time_after, NULL);
    timersub(&time_after, &time_before, &time_result);
    printf("Time elapsed: %ld.%06ld\n", (long int)time_result.tv_sec, (long int)time_result.tv_usec);

    for (i = 0; i < 10000; i++) {
        free(buffer[i]);
    }
    timersub(&time_after, &time_before, &time_result);
    printf("Time elapsed: %ld.%06ld\n", (long int)time_result.tv_sec, (long int)time_result.tv_usec);

}

void test_wc_malloc() {
    long int i = 0;
    struct timeval time_before, time_after, time_result;

    gettimeofday(&time_before, NULL);
    wc_malloc_init();
    for (i = 0; i < 10000; i++) {
        buffer[i] = wc_malloc(128);
        bzero(buffer[i], 128);
    }
    gettimeofday(&time_after, NULL);
    timersub(&time_after, &time_before, &time_result);
    printf("Time elapsed: %ld.%06ld\n", (long int)time_result.tv_sec, (long int)time_result.tv_usec);

    for (i = 0; i < 10000; i++) {
        wc_free(buffer[i]);
    }
    gettimeofday(&time_after, NULL);
    timersub(&time_after, &time_before, &time_result);
    printf("Time elapsed: %ld.%06ld\n", (long int)time_result.tv_sec, (long int)time_result.tv_usec);

}



int main(int argc, char *argv[]) {

	for (int i = 0; i < 10; i++) {
//   printf("Testing malloc\r\n")
	test_sys_malloc();
//    printf("Testing wc_malloc\r\n");
    test_wc_malloc();
	}
    return 0;
}

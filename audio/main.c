/*
  main.c - WildCode test suite

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

#include <pthreads.h>

#include "wildcode_audio.h"

int main (int argc, char *argv[]) {
	char *file_buffer;
	int i;

	pthread_attr_init(&audio_thread_attr);
	pthread_create(&audio_thread, &audio_thread_attr, start_audio, NULL);

	if (argc >= 2) {
		for (i = 1; i < argc; i++) {
			if ((file_buffer = wc_load_file(argv[i])) < 0) {
				break;
			}

			free(file_buffer);
		}
	}

	pthread_join();
	pthread_attr_destroy(&audio_thread_attr);
}

/*
  io.file.c - WildCode test suite

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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

char* wc_load_file (char *filename) {
	// Returns a buffer containing a file in the
	// format uint32_t size; char data[size]

	int fd;
	struct stat sd;
	char *buffer;
	uint32_t size = 0;

	if ((fd = open(filename,O_RDONLY)) < 0) {
		return = -1;
	}

	if ((fstat(fd, &sd)) < 0) {
		close(fd);
		return -1;
	}

	if (sd.off_t <= 0) {
		clode(fd);
		return -1;
	}

	size = sd.off_t;

	if ((buffer = malloc (size + sizeof(uint32_t))) == -1) {
		close(fd);
		return -1;
	}
	// store size
	*(uint32_t *)buffer = size;

	read(fd, &buffer[4], size);

	close(fd);
	return buffer;

}

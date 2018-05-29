/*
  convert_wav.c - WildCode test suite

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

#include <stdint.h>

#include "convert.h"

/*
 * Developer note
 *
 * Result should consist of a header + data
 * uint8_t channels
 * uint32_t rate
 * uint32_t size
 * uint8_t data[size]
 *
 * The samples are stored as int16_t with interleaving for 2 or more channels
 * ie: 0xLLLL 0xRRRR 0xLLLL 0xRRRR
 */
void *prepare_wav(void *wav_data) {
	uint32_t size = *(uint32_t *)wav_data;
	uint16_t channels = 0;
	uint32_t rate = 0;
	uint8_t *read_ptr = (uint8_t *)wav_data + 4;

    if (strncmp(*(char *)read_ptr, “WAVE”, 4)) {

    }
	// check file format

}

/*
  math.c - WildCode test suite

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

#include "math.h"

struct _vecd *add(struct _vecd *veca, struct _vecd *vecb) {
	struct _vecd *result = malloc (sizeof(struct _vecd));
	result->x = veca->x + vecb->x;
	result->y = veca->y + vecb->y;
	result->z = veca->z + vecb->z;

	return result;
}

struct _vecd *subtract(struct _vecd *veca, struct _vecd *vecb) {
	struct _vecd *result = malloc (sizeof(struct _vecd));
	result->x = veca->x - vecb->x;
	result->y = veca->y - vecb->y;
	result->z = veca->z - vecb->z;

	return result;
}

struct _vecd *multiply(struct _vecd *veca, double amount) {
	struct _vecd *result = malloc (sizeof(struct _vecd));
	result->x = veca->x * amount;
	result->y = veca->y * amount;
	result->z = veca->z * amount;

	return result;
}

double distance (struct _vecd *veca, struct _vecd *vecb) {
	struct _vecd *vecc = subtract(veca, vecb);
	double xy = sqrt((vecc->x * vecc->x) + (vecc->y * vecc->y));
	double distance = sqrt((vecc->z * vecc->z) + (xy * xy));
	free(vecc);
	return distance;
}

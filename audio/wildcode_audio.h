/*
  wildcode_audio.h - WildCode test suite

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

#ifndef _AUDIO_H
#define _AUDIO_H

#include <pthread.h>

extern pthread_t audio_thread;
extern pthread_attr_t audio_thread_attr;

extern void *start_audio(void *ptr);

#endif // _AUDIO_H

/*
  audio_main.c - WildCode test suite

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

#include <asoundlib.h>

#include "wildcode_audio.h"

pthread_t audio_thread;
pthread_attr_t audio_thread_attr;

void *start_audio(void *ptr) {
	char *device = "default";
	snd_pcm_t *handle;
	snd_pcm_sframes_t frames;

	if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
		printf("Open PCM Error: %s\r\n", snd_strerror(err));
		pthread_exit(err);
	}
	if ((err = snd_pcm_set_params(handle, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 2, 48000, 1, 100000)) < 0) {
		printf("Open PCM Error: %s\r\n", snd_strerror(err));
		pthread_exit(err);
	}



	snd_pcm_close(handle);
	pthread_exit(0);
}



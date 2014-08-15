#include <sndfile.h>
#include <string.h>
#include <stdlib.h>
#include "ebur128.h"

int main(int ac, const char* av[]) {
	SF_INFO file_info;
	SNDFILE* file;
	sf_count_t nr_frames_read;
	ebur128_state* state = NULL;
	double* buffer;
	double loudness;

	if (ac != 2) {
		exit(1); //ERROR CODE 1: Exactly one input file is expected.   
  	}

  	state = malloc((size_t) sizeof(ebur128_state*));
    	file_info.format = SF_FORMAT_RAW | SF_FORMAT_PCM_24 | SF_ENDIAN_BIG;
    	file_info.channels = 2;
    	file_info.samplerate = 48000;
  	file = sf_open(av[1], SFM_READ, &file_info);
	state = ebur128_init((unsigned) file_info.channels, (unsigned) file_info.samplerate, EBUR128_MODE_I);

	buffer = (double*) malloc(state->samplerate * state->channels * sizeof(double));

	while ((nr_frames_read = sf_readf_double(file, buffer, (sf_count_t) state->samplerate))) {
      ebur128_add_frames_double(state, buffer, (size_t) nr_frames_read);
    }

    ebur128_loudness_global(state, &loudness);
    fprintf(stderr, "%.2f\n", loudness);

    free(buffer);
    buffer = NULL;

    if (sf_close(file)) {
    	exit(2); //ERROR CODE 2: File wasn't able to be closed.
    }

    ebur128_destroy(&state);
	free(state);
	return 0; // ERROR CODE 0: No errors!
}

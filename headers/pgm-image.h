#ifndef _IMAGEIO
#define _IMAGEIO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct {
	int width;
	int height;
	unsigned char * buffer;
} PGM_IMAGE;

void pgm_image_free(PGM_IMAGE *img);
PGM_IMAGE *pgm_image_new(int width, int height);
void pgm_image_write(char *filename, PGM_IMAGE *img);
PGM_IMAGE *pgm_image_read(char *filename);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "../headers/pgm-image.h"
#include "../headers/canny.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s <input.pgm> <output.pgm>\n", argv[0]);
        return 1;
    }

    PGM_IMAGE *input_img = pgm_image_read(argv[1]);

    PGM_IMAGE *output_img = canny_edge_detect(input_img);

    pgm_image_write(argv[2], output_img);

    pgm_image_free(input_img);
    pgm_image_free(output_img);

	return 0;
}

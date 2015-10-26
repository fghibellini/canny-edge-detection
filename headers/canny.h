#ifndef _FASTEDGE
#define _FASTEDGE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "pgm-image.h"
#include "gauss.h"

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

typedef enum { VERTICAL, HORIZONTAL, DIAGONAL, ANTIDIAGONAL } DIRECTION;

typedef struct {
    int width;
    int height;
    int *gradients;
    DIRECTION *directions;
} SOBEL_RESULT;

typedef struct {
    int high;
    int low;
} THRESHOLD_PAIR;

PGM_IMAGE *canny_edge_detect(PGM_IMAGE * input_img);
SOBEL_RESULT *calc_sobel(PGM_IMAGE *input_img);
PGM_IMAGE *non_max_suppression(SOBEL_RESULT *sobel);

THRESHOLD_PAIR estimate_threshold(PGM_IMAGE * img);
PGM_IMAGE *hysteresis(PGM_IMAGE *input_img, THRESHOLD_PAIR thresholds);
void trace(int x, int y, int low, PGM_IMAGE *input_img, PGM_IMAGE *output_img);
SOBEL_RESULT *sobel_result_new(int width, int height);
void sobel_result_free(SOBEL_RESULT *sr);

#endif

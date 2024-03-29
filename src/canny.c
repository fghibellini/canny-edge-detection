#include "../headers/canny.h"

PGM_IMAGE *canny_edge_detect(PGM_IMAGE * input_img)
{
    // STEP 1
    PGM_IMAGE *gauss_result = gaussian_noise_reduce(input_img);
    // STEP 2
    SOBEL_RESULT *sobel_result = calc_sobel(gauss_result);
    // STEP 3
    PGM_IMAGE *non_max_suppressed = non_max_suppression(sobel_result);
    // STEP 4
    THRESHOLD_PAIR thresholds = estimate_threshold(non_max_suppressed);
    // STEP 5
    PGM_IMAGE *hysteresis_result = hysteresis(non_max_suppressed, thresholds);

    pgm_image_free(gauss_result);
    sobel_result_free(sobel_result);
    pgm_image_free(non_max_suppressed);

    return hysteresis_result;
}

/**
 * Calculates the result of the Sobel operator (http://en.wikipedia.org/wiki/Sobel_operator),
 * and estimates edge direction angle.
 */
SOBEL_RESULT *calc_sobel(PGM_IMAGE *input_img)
{
    int w = input_img->width;
    int h = input_img->height;
    unsigned char *b = input_img->buffer;

    SOBEL_RESULT *sr = sobel_result_new(w, h);
    int * g = sr->gradients;
    DIRECTION *dir = sr->directions;

    int max_x = w - 3;
    int max_y = w * (h - 3);

    for (int y = 3 * w; y < max_y; y += w)
    {
        for (int x = 3; x < max_x; x++)
        {
            int g_x = (2 * b[x + y + 1] 
                    + b[x + y - w + 1]
                    + b[x + y + w + 1]
                    - 2 * b[x + y - 1] 
                    - b[x + y - w - 1]
                    - b[x + y + w - 1]);
            int g_y = 2 * b[x + y - w] 
                    + b[x + y - w + 1]
                    + b[x + y - w - 1]
                    - 2 * b[x + y + w] 
                    - b[x + y + w + 1]
                    - b[x + y + w - 1];

            g[x + y] = sqrt(g_x * g_x + g_y * g_y);

            int direction;
            if (g_x == 0) {
                direction = VERTICAL;
            } else {
                float g_div = g_y / (float) g_x;
                if (g_div < 0) {
                    if (g_div < -2.41421356237) {
                       direction = HORIZONTAL;
                    } else {
                        if (g_div < -0.414213562373) {
                            direction = ANTIDIAGONAL;
                        } else {
                            direction = VERTICAL;
                        }
                    }
                } else {
                    if (g_div > 2.41421356237) {
                        direction = HORIZONTAL;
                    } else {
                        if (g_div > 0.414213562373) {
                            direction = DIAGONAL;
                        } else {
                            direction = VERTICAL;
                        }
                    }
                }
            }
            dir[x + y] = direction;
        }
    }    

    return sr;
}

PGM_IMAGE *non_max_suppression(SOBEL_RESULT *sobel)
{
    int w = sobel->width;
    int h =  sobel->height;
    DIRECTION *dir = sobel->directions;
    int *g = sobel->gradients;

    PGM_IMAGE *output_image = pgm_image_new(w, h);
    unsigned char *b = output_image->buffer;

    int max_x = w - 1;
    int max_y = w * (h - 1);

    for (int y = 1; y < max_y; y += w)
    {
        for (int x = 1; x < max_x; x++)
        {
            DIRECTION direction = dir[x + y];

            switch (direction) {
                case HORIZONTAL:
                    if (g[x + y] > g[x + y - w] && g[x + y] > g[x + y + w]) {
                        b[x + y] = min(255, g[x + y]);
                    } else {
                        b[x + y] = 0;
                    }
                    break;
                case ANTIDIAGONAL:
                    if (g[x + y] > g[x + y - w - 1] && g[x + y] > g[x + y + w + 1]) {
                        b[x + y] = min(255, g[x + y]);
                    } else {
                        b[x + y] = 0;
                    }
                    break;
                case VERTICAL:
                    if (g[x + y] > g[x + y - 1] && g[x + y] > g[x + y + 1]) {
                        b[x + y] = min(255, g[x + y]);
                    } else {
                        b[x + y] = 0;
                    }
                    break;
                case DIAGONAL:
                    if (g[x + y] > g[x + y - w + 1] && g[x + y] > g[x + y + w - 1]) {
                        b[x + y] = min(255, g[x + y]);
                    } else {
                        b[x + y] = 0;
                    }
                    break;
                default:
                    fprintf(stderr, "ERROR - invalid DIRECTION value\n");
                    exit(2);
            }
        }
    }

    return output_image;
}

/**
 * ESTIMATE_THRESHOLD
 * ------------------
 * estimates hysteresis threshold, assuming that the top X% (as defined by the HIGH_THRESHOLD_PERCENTAGE)
 * of edge pixels with the greatest intesity are true edges and that the low threshold is equal to the quantity
 * of the high threshold plus the total number of 0s at the low end of the histogram divided by 2.
 */
THRESHOLD_PAIR estimate_threshold(PGM_IMAGE * img) {
    int histogram[256];
    for (unsigned char i = 0; i < 255; i++)
    {
        histogram[i] = 0;
    }

    unsigned char *buffer = img->buffer;
    int buffer_size = img->width * img->height;

    for (int i = 0; i < buffer_size; i++) {
        histogram[buffer[i]]++;
    }

    int non_zero_count = buffer_size - histogram[0];
    int pixels = non_zero_count * 0.1;
    int high_cutoff = 0;

    int i = 255;
    while (high_cutoff < pixels) {
        high_cutoff += histogram[i];
        i--;
    }
    int high = i;

    int j = 1;
    while (histogram[j] == 0) {
        j++;
    }
    int low = (high + j) * 0.8;

    THRESHOLD_PAIR res = { high, low };
    return res;
}

PGM_IMAGE *hysteresis(PGM_IMAGE *input_img, THRESHOLD_PAIR thresholds)
{
    int high = thresholds.high;
    int low = thresholds.low;

    int w = input_img->width;
    int h = input_img->height;
    unsigned char* in_buffer = input_img->buffer;

    PGM_IMAGE *output_img = pgm_image_new(w, h);
    unsigned char *out_buffer = output_img->buffer;

    int max = w * h;
    for (int i = 0; i < max; i++)
    {
        out_buffer[i] = 0;
    }

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if (in_buffer[y * w + x] >= high) {
                trace(x, y, low, input_img, output_img);
            }
        }
    }

    return output_img;
}

void trace(int x, int y, int low, PGM_IMAGE *input_img, PGM_IMAGE *output_img)
{
    unsigned char* in_buffer = input_img->buffer;
    unsigned char *out_buffer = output_img->buffer;

    int w = output_img->width;
    int h = output_img->height;

    if (out_buffer[y * w + x] != 0) {
        // don't pass twice on same spot
        return;
    } else {
        out_buffer[y * w + x] = 255;

        int j_min = y < 1 ? 0 : -1;
        int j_max = y > (h-1) ? 0 : 1;
        int i_min = x < 1 ? 0 : -1;
        int i_max = x > (w-1) ? 0 : 1;

        for (int j = j_min; j <= j_max; j++)
        {
            for(int i = i_min; i <= i_max; i++)
            {
                int top = y + j;
                int left = x + i;
                if (in_buffer[top * w + left] >= low) {
                    trace(left, top, low, input_img, output_img);
                }
            }
        }
    }
}

SOBEL_RESULT *sobel_result_new(int width, int height)
{
    SOBEL_RESULT *res = malloc(sizeof(SOBEL_RESULT));
    res->width = width;
    res->height = height;
    res->gradients = malloc(width * height * sizeof(int));
    res->directions = malloc(width * height * sizeof(DIRECTION));
    return res;
}

void sobel_result_free(SOBEL_RESULT *sr)
{
    free(sr->gradients);
    free(sr->directions);
    free(sr);
}

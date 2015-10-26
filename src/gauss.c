
#include "../headers/gauss.h"

/*
	GAUSSIAN_NOISE_ REDUCE
	apply 5x5 Gaussian convolution filter, shrinks the image by 4 pixels in each direction, using Gaussian filter found here:
	http://en.wikipedia.org/wiki/Canny_edge_detector
*/
PGM_IMAGE *gaussian_noise_reduce(PGM_IMAGE * input_img)
{
    int w = input_img->width;
    int h = input_img->height;
    unsigned char *b = input_img->buffer;

    PGM_IMAGE *output_img = pgm_image_new(w, h);
    unsigned char *output_buffer = output_img->buffer;

	int max_x = w - 2;
	int max_y = w * (h - 2);

	for (int y = w * 2; y < max_y; y += w)
    {
		for (int x = 2; x < max_x; x++)
        {
			output_buffer[x + y] = (2 * b[x + y - 2 - w - w] + 
			4 * b[x + y - 1 - w - w] + 
			5 * b[x + y - w - w] + 
			4 * b[x + y + 1 - w - w] + 
			2 * b[x + y + 2 - w - w] + 
			4 * b[x + y - 2 - w] + 
			9 * b[x + y - 1 - w] + 
			12 * b[x + y - w] + 
			9 * b[x + y + 1 - w] + 
			4 * b[x + y + 2 - w] + 
			5 * b[x + y - 2] + 
			12 * b[x + y - 1] + 
			15 * b[x + y] + 
			12 * b[x + y + 1] + 
			5 * b[x + y + 2] + 
			4 * b[x + y - 2 + w] + 
			9 * b[x + y - 1 + w] + 
			12 * b[x + y + w] + 
			9 * b[x + y + 1 + w] + 
			4 * b[x + y + 2 + w] + 
			2 * b[x + y - 2 + w + w] + 
			4 * b[x + y - 1 + w + w] + 
			5 * b[x + y + w + w] + 
			4 * b[x + y + 1 + w + w] + 
			2 * b[x + y + 2 + w + w]) / 159;
		}
	}

    return output_img;
}

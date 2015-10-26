
#include "../headers/pgm-image.h"

void pgm_image_free(PGM_IMAGE *img)
{
    free(img->buffer);
    free(img);
}

PGM_IMAGE *pgm_image_new(int width, int height)
{
	PGM_IMAGE *img = malloc(sizeof(PGM_IMAGE));
	img->width = width;
	img->height = height;
	img->buffer = malloc(width * height * sizeof(unsigned char));
    return img;
}

void pgm_image_write(char *filename, PGM_IMAGE * img) 
{
    FILE *fp = fopen(filename, "w");
    if (fp == 0) {
        fprintf(stderr, "Couldn't open file '%s'.\n", filename);
        exit(1);
    }

	fprintf(fp, "P5\n%d %d\n255\n", img->width, img->height);

	for(int i = 0, ii = img->height * img->width; i < ii; i++)
    {
		fputc(img->buffer[i], fp);
    }

	fclose(fp);
}

PGM_IMAGE *pgm_image_read(char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == 0) {
        fprintf(stderr, "Couldn't open file '%s'.\n", filename);
        exit(1);
    }


	char buff[4];
    int readInt;
    int width, height;

    if (fscanf(fp, "%2s\n", buff) != 1) {
        fprintf(stderr, "Couldn't read magic bytes of file '%s'.", filename);
        exit(1);
    }
    if (strcmp(buff, "P5")) {
        fprintf(stderr, "Invalid magic bytes of file '%s'.", filename);
        exit(1);
    }
    if (fscanf(fp, "%d", &width) != 1) {
        fprintf(stderr, "Couldn't read width in file '%s'.", filename);
        exit(1);
    }
    if (fscanf(fp, "%d", &height) != 1) {
        fprintf(stderr, "Couldn't read height in file '%s'.", filename);
        exit(1);
    }
    if (fscanf(fp, "%d\n", &readInt) != 1) {
        fprintf(stderr, "Couldn't read maxValue in file '%s'.", filename);
        exit(1);
    }
    if (readInt != 255) {
        fprintf(stderr, "Invalid maxValue of pgm image '%s'.", filename);
        exit(1);
    }

    PGM_IMAGE *img = pgm_image_new(width, height);

    for (int i = 0, ii = img->width * img->height; i < ii; i++) {
        int c = fgetc(fp);
        if (c == EOF) {
            fprintf(stderr, "Not enough bytes in file '%s'.", filename);
            exit(1);
        } else {
            img->buffer[i] = (unsigned char) c;
        }
    }

	fclose(fp);

    return img;
}

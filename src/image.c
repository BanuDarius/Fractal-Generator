#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"

// Allocates and initializes a new image with the given width and height
Image *create_image(int width, int height)
{
	Image *img = malloc(sizeof(Image));
	if (!img)
	{
		fprintf(stderr, "Error alocating image\n");
		return NULL;
	}
	img->width = width;
	img->height = height;
	img->pixels = malloc(height * sizeof(Color *));
	if (!img->pixels)
	{
		fprintf(stderr, "Error alocating image\n");
		free(img);
		return NULL;
	}
	for (int i = 0; i < height; i++)
	{
		img->pixels[i] = malloc(width * sizeof(Color));
		if (!img->pixels[i])
		{
			fprintf(stderr, "Error alocating image\n");
			for (int j = 0; j < i; j++)
				free(img->pixels[j]);
			free(img->pixels);
			free(img);
			exit(1);
		}
	}
	return img;
}

void free_image(Image *img)
{
	for (int i = 0; i < img->height; i++)
		free(img->pixels[i]);
	free(img->pixels);
	free(img);
}

// Fills the entire image with black pixels
void blank_image(Image *img)
{
	for (int y = 0; y < img->height; y++)
		for (int x = 0; x < img->width; x++)
			img->pixels[y][x] = (Color){0, 0, 0};
}

// Applies the specified color to pixel (x, y) if the coordinates are within bounds
void color_pixel(Image *img, int x, int y, Color c)
{
	if (x >= 0 && x < img->width && y >= 0 && y < img->height)
		img->pixels[y][x] = c;
}

void fill_triangle(Image *img, int x, int y, int height, Color c)
{
	#pragma omp parallel for
	for (int dy = 0; dy < height; dy++)
	{
		// Iterates over pixels that form the width of the triangle on the current row
		for (int dx = -dy; dx <= dy; dx++)
		{
			int pixel_x = x + dx;
			int pixel_y = y + dy;
			color_pixel(img, pixel_x, pixel_y, c); // colors the current pixel
		}
	}
}

void draw_line(Image *img, int x1, int y1, int x2, int y2, Color c)
{
	int delta_x = abs(x2 - x1);
	int delta_y = abs(y2 - y1);

	int dir_x, dir_y;

	// Determines the step direction along the axes
	if (x1 < x2)
		dir_x = 1;
	else
		dir_x = -1;

	if (y1 < y2)
		dir_y = 1;
	else
		dir_y = -1;

	int err = delta_x - delta_y;

	while (1) // loops until the end point is reached
	{
		color_pixel(img, x1, y1, c);
		if (x1 == x2 && y1 == y2)
			break;

		int err2 = 2 * err;

		// Adjusts error and moves horizontally if needed
		if (err2 > -delta_y)
		{
			err -= delta_y;
			x1 += dir_x;
		}

		// Adjusts error and moves vertically if needed
		if (err2 < delta_x)
		{
			err += delta_x;
			y1 += dir_y;
		}
	}
}

// Fills a square area in the image with the specified color
void fill_square(Image *img, int x, int y, int size, Color c)
{
	// Iterates over each pixel in the square
	#pragma omp parallel for
	for (int dy = 0; dy < size; dy++)
		for (int dx = 0; dx < size; dx++)
		{
			color_pixel(img, x + dx, y + dy, c);
		}
}

// Calculate a scaled size based on the number of iterations and image height.
int calculate_scaled_size(int iterations, int height)
{
	if (iterations == 0)
		return height / 2;
	double scale = height / 2;
	if (scale < 1)
		scale = 1;
	return (int)scale;
}

// Saves the image in plain PPM format (P3) to a file
void save_ppm(const char *filename, Image *img)
{
	FILE *f = fopen(filename, "w");
	if (!f)
	{
		fprintf(stderr, "Couldn't open file\n");
		return;
	}

	// Writes the PPM header: format, width, height, max color value
	fprintf(f, "P3\n%d %d\n255\n", img->width, img->height);

	// Write each pixel's RGB values
	for (int y = 0; y < img->height; y++)
	{
		for (int x = 0; x < img->width; x++)
		{
			Color col = img->pixels[y][x];
			fprintf(f, "%d %d %d ", col.r, col.g, col.b);
		}
		fprintf(f, "\n");
	}
	fclose(f);
}

// Returns an RGB Color struct for a given color name
Color get_color_from_name(const char *name)
{
	if (strcmp(name, "red") == 0)
		return (Color){255, 0, 0};
	if (strcmp(name, "green") == 0)
		return (Color){0, 255, 0};
	if (strcmp(name, "blue") == 0)
		return (Color){0, 0, 255};
	if (strcmp(name, "yellow") == 0)
		return (Color){255, 255, 0};
	if (strcmp(name, "cyan") == 0)
		return (Color){0, 255, 255};
	if (strcmp(name, "magenta") == 0)
		return (Color){255, 0, 255};
	if (strcmp(name, "white") == 0)
		return (Color){255, 255, 255};
	return (Color){255, 255, 255}; // white is the default color
}
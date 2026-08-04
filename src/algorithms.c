#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "algorithms.h"
#include "image.h"

void sierpinski_triangle_rec(Image *img, int x, int y, int height, int iterations, Color c)
{
	if (iterations == 0)
	{
		fill_triangle(img, x, y, height, c); // draws a filled triangle at the current position
		return;
	}

	// Divides into 3 sub-triangles and draws each
	sierpinski_triangle_rec(img, x, y, height / 2, iterations - 1, c);
	sierpinski_triangle_rec(img, x - height / 2, y + height / 2, height / 2, iterations - 1, c);
	sierpinski_triangle_rec(img, x + height / 2, y + height / 2, height / 2, iterations - 1, c);
}

void sierpinski_triangle_chaos(Image *img, int num_points, Color c)
{
	// Defines the three vertices of the main triangle
	double x1 = img->width / 2.0, y1 = 0;
	double x2 = 0, y2 = img->height - 1;
	double x3 = img->width - 1, y3 = img->height - 1;

	// Initializes a random starting point
	double x = rand() % img->width;
	double y = rand() % img->height;
	
	#pragma omp parallel for
	for (int i = 0; i < num_points; i++)
	{
		int r = rand() % 3; // randomly selects one of the three triangle vertices

		// Moves halfway towards the selected vertex
		if (r == 0)
		{
			x = (x + x1) / 2.0;
			y = (y + y1) / 2.0;
		}
		else if (r == 1)
		{
			x = (x + x2) / 2.0;
			y = (y + y2) / 2.0;
		}
		else
		{
			x = (x + x3) / 2.0;
			y = (y + y3) / 2.0;
		}
		color_pixel(img, (int)x, (int)y, c);
	}
}

void sierpinski_carpet(Image *img, int x, int y, int size, int iterations, Color c)
{
	if (iterations == 0)
	{
		fill_square(img, x, y, size, c);
		return;
	}

	int new_size = size / 3;

	// Ends recursion when the square becomes too small to subdivide further
	if (new_size < 1)
		return;
	
	#pragma omp parallel for
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (!(i == 1 && j == 1)) // skips the center square to create the hole in the carpet
			{
				// Determines the starting coordinates of the current sub-square
				int new_x = x + i * new_size;
				int new_y = y + j * new_size;
				sierpinski_carpet(img, new_x, new_y, new_size, iterations - 1, c);
			}
		}
	}
}

void koch_curve(Image *img, double x1, double y1, double x2, double y2, int iterations, Color c, bool flip)
{
	if (iterations == 0)
	{
		draw_line(img, (int)x1, (int)y1, (int)x2, (int)y2, c);
		return;
	}

	double dx = (x2 - x1) / 3.0;
	double dy = (y2 - y1) / 3.0;

	// Points dividing the segment into three equal parts
	double x3 = x1 + dx;
	double y3 = y1 + dy;

	double x5 = x2 - dx;
	double y5 = y2 - dy;

	double sin60 = sin(M_PI / 3.0);
	double cos60 = cos(M_PI / 3.0);

	double x4, y4;

	// Calculates the coordinates of the apex point that creates the characteristic Koch Curve "bump"
	if (flip) // determines the directions of the "bump"
	{
		x4 = x3 + dx * cos60 + dy * sin60;
		y4 = y3 - dx * sin60 + dy * cos60;
	}
	else
	{
		x4 = x3 + dx * cos60 - dy * sin60;
		y4 = y3 + dx * sin60 + dy * cos60;
	}

	// Recursively draws the four smaller Koch Curve segments
	koch_curve(img, x1, y1, x3, y3, iterations - 1, c, flip);
	koch_curve(img, x3, y3, x4, y4, iterations - 1, c, flip);
	koch_curve(img, x4, y4, x5, y5, iterations - 1, c, flip);
	koch_curve(img, x5, y5, x2, y2, iterations - 1, c, flip);
}

void koch_snowflake(Image *img, int center_x, int center_y, int size, int iterations, Color c)
{
	// Calculates half the base length and the height of the equilateral triangle
	double half_size = size / 2.0;
	double height = size * sqrt(3.0) / 2.0;

	// Determines the coordinates of the trianglefs three vertices
	double x1 = center_x - half_size;
	double y1 = center_y + height / 3.0;

	double x2 = center_x + half_size;
	double y2 = center_y + height / 3.0;

	double x3 = center_x;
	double y3 = center_y - 2.0 * height / 3.0;

	// Recursively draws each edge of the equilateral triangle using Koch Curve function
	koch_curve(img, x1, y1, x2, y2, iterations, c, false);
	koch_curve(img, x2, y2, x3, y3, iterations, c, false);
	koch_curve(img, x3, y3, x1, y1, iterations, c, false);
}

void mandelbrot(Image *img, int width, int height, int iterations, Color base_color)
{
	// Defines the bounds of the complex plane for the default Mandelbrot set
	double x_min = -2.0, x_max = 1.0;
	double y_min = -1.5, y_max = 1.5;

	#pragma omp parallel for
	for (int px = 0; px < width; px++)
	{
		for (int py = 0; py < height; py++)
		{
			// Converts pixel coordinates to corresponding point in the complex plane
			double x_scale = x_min + px * (x_max - x_min) / width;
			double y_scale = y_min + py * (y_max - y_min) / height;
			
			double zx = 0.0, zy = 0.0;
			int i = 0;
			
			// Stops iterating if the complex number escapes the radius 2 circle (|z| > 2)
			while (zx * zx + zy * zy <= 4.0 && i < iterations)
			{
				double xtemp = zx * zx - zy * zy + x_scale;
				zy = 2 * zx * zy + y_scale;
				zx = xtemp;
				i++;
			}
			
			// Color the pixel based on whether it belongs to the set
			if (i == iterations)
			{
				color_pixel(img, px, py, base_color); // inside the set
			}
			else
			{
				// Pixel not in Mandelbrot set ? applies grayscale color based on how fast it diverged
				int color_value = (int)(255 * i / (double)iterations);
				Color c = {color_value, color_value, color_value};
				color_pixel(img, px, py, c);
			}
		}
	}
}

// Similar approach to Mandelbrot Set, but iterates z = z^2 + c for fixed c
void julia(Image *img, int width, int height, int iterations, Color base_color, double cx, double cy)
{
	// Defines the bounds of the complex plane for the Julia set
	double xmin = -2.0, xmax = 2.0;
	double ymin = -2.0, ymax = 2.0;
	
	#pragma omp parallel for
	for (int px = 0; px < width; px++)
	{
		for (int py = 0; py < height; py++)
		{
			double zx = xmin + px * (xmax - xmin) / width;
			double zy = ymin + py * (ymax - ymin) / height;
			
			int i = 0;
			
			while (zx * zx + zy * zy <= 4.0 && i < iterations)
			{
				double xtemp = zx * zx - zy * zy + cx;
				zy = 2.0 * zx * zy + cy;
				zx = xtemp;
				i++;
			}
			
			if (i == iterations)
			{
				color_pixel(img, px, py, base_color);
			}
			else
			{
				int color_value = (int)(255 * i / (double)iterations);
				Color c = {color_value, color_value, color_value};
				color_pixel(img, px, py, c);
			}
		}
	}
}
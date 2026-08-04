#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"
#include "structs.h"
#include "algorithms.h"

int main()
{
	int iterations, i;
	char color_name[20];
	char type[50];

	printf("Enter fractal type: ");
	if (scanf("%s", type) != 1)
	{
		fprintf(stderr, "Invalid input\n");
		return 1;
	}

	printf("Enter the number of iterations: ");
	if (scanf("%d", &iterations) != 1 || iterations < 0)
	{
		fprintf(stderr, "Invalid number\n");
		return 1;
	}

	printf("Enter fractal color: ");
	if (scanf("%s", color_name) != 1)
	{
		fprintf(stderr, "Invalid color\n");
		return 1;
	}

	Color color = get_color_from_name(color_name);

	int width;
	printf("Enter width: ");
	i += scanf("%d", &width);
	printf("Enter height: ");
	int height;
	i += scanf("%d", &height);
	Image *img = create_image(width, height);
	blank_image(img);

	if (strcmp(type, "triangle") == 0)
	{
		if (iterations < 10)
		{
			int size = calculate_scaled_size(iterations, height);

			// Starts drawing near the top middle of the image
			int start_x = width / 2;
			int start_y = 10; // offset to avoid drawing at the very top

			sierpinski_triangle_rec(img, start_x, start_y, size, iterations, color);
			printf("Using recursive method for %d iterations\n", iterations);
		}
		else
		{
			int points = iterations * 10000; // adjusts points count based on iterations to preserve fractal detail

			sierpinski_triangle_chaos(img, points, color);
			printf("Using Chaos Game method for %d points\n", points);
		}

		save_ppm("output/triangle.ppm", img);
	}
	else if (strcmp(type, "curve") == 0)
	{
		// Set the start and end points of the Koch curve, 100 pixels away from the left and right edges,
		// and vertically centered in the image
		double x1 = 100.0;
		double y1 = height / 2.0;
		double x2 = width - 100.0;
		double y2 = height / 2.0;

		if (iterations < 13)
			koch_curve(img, x1, y1, x2, y2, iterations, color, true);
		else
			koch_curve(img, x1, y1, x2, y2, 15, color, true);

		save_ppm("output/curve.ppm", img);
	}
	else if (strcmp(type, "carpet") == 0)
	{
		int size = (int)(height * 0.8); // size set to 80% of image height to fit nicely

		// Centers the fractal in the image
		int start_x = (width - size) / 2;
		int start_y = (height - size) / 2;

		if (iterations < 6)
			sierpinski_carpet(img, start_x, start_y, size, iterations, color);
		else
			sierpinski_carpet(img, start_x, start_y, size, 6, color);

		save_ppm("output/carpet.ppm", img);
	}
	else if (strcmp(type, "snowflake") == 0)
	{
		int size = (int)(height * 0.7); // size set to 70% of image height to fit nicely

		// Starts near the middle, moved a bit down to fit better
		int start_x = width / 2;
		int start_y = height / 2 + 50;

		if (iterations < 13)
			koch_snowflake(img, start_x, start_y, size, iterations, color);
		else
			koch_snowflake(img, start_x, start_y, size, 13, color);

		save_ppm("output/snowflake.ppm", img);
	}
	else if (strcmp(type, "mandelbrot") == 0)
	{
		mandelbrot(img, width, height, iterations, color);

		save_ppm("output/mandelbrot.ppm", img);
	}
	else if (strcmp(type, "julia") == 0)
	{
		double cx, cy;
		printf("Enter the real part of c: ");
		i += scanf("%lf", &cx);
		printf("Enter the imaginary part of c: ");
		i += scanf("%lf", &cy);

		julia(img, width, height, iterations, color, cx, cy);

		save_ppm("output/julia.ppm", img);
	}
	else
	{
		fprintf(stderr, "Unknown fractal type\n");
		free_image(img);
		return 1;
	}

	free_image(img);
	return 0;
}

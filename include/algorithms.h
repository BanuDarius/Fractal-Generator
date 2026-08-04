#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "structs.h"

void sierpinski_triangle_rec(Image *img, int x, int y, int height, int iterations, Color c);
void sierpinski_triangle_chaos(Image *img, int num_points, Color c);
void sierpinski_carpet(Image *img, int x, int y, int size, int iterations, Color c);
void koch_curve(Image *img, double x1, double y1, double x2, double y2, int iterations, Color c, bool flip);
void koch_snowflake(Image *img, int center_x, int center_y, int size, int iterations, Color c);
void mandelbrot(Image *img, int width, int height, int iterations, Color base_color);
void julia(Image *img, int width, int height, int iterations, Color base_color, double cx, double cy);

#endif
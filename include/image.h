#ifndef IMAGE_H
#define IMAGE_H

#include "structs.h"

Image *create_image(int width, int height);
void free_image(Image *img);
void blank_image(Image *img);
void color_pixel(Image *img, int x, int y, Color c);
void fill_triangle(Image *img, int x, int y, int height, Color c);
void draw_line(Image *img, int x1, int y1, int x2, int y2, Color c);
void fill_square(Image *img, int x, int y, int size, Color c);
int calculate_scaled_size(int iterations, int height);
void save_ppm(const char *filename, Image *img);
Color get_color_from_name(const char *name);

#endif
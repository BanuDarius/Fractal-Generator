#ifndef STRUCTS_H
#define STRUCTS_H

#define M_PI 3.14159265358979323846

// Structure to represent an RGB color
typedef struct
{
    int r, g, b;
} Color;

// Structure to represent a 2D image made of pixels
typedef struct
{
    int width, height;
    Color **pixels;
} Image;

#endif
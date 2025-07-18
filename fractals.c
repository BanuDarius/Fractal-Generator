#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

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

// Fills a square area in the image with the specified color
void fill_square(Image *img, int x, int y, int size, Color c)
{
    // Iterates over each pixel in the square
    for (int dy = 0; dy < size; dy++)
        for (int dx = 0; dx < size; dx++)
        {
            color_pixel(img, x + dx, y + dy, c);
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

void koch_snowflake(Image *img, int center_x, int center_y, int size, int iterations, Color c)
{
    // Calculates half the base length and the height of the equilateral triangle
    double half_size = size / 2.0;
    double height = size * sqrt(3.0) / 2.0;

    // Determines the coordinates of the triangle’s three vertices
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
                // Pixel not in Mandelbrot set – applies grayscale color based on how fast it diverged
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

int main()
{
    int iterations;
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
    scanf("%d", &width);
    printf("Enter height: ");
    int height;
    scanf("%d", &height);
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

        save_ppm("triangle.ppm", img);
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

        save_ppm("curve.ppm", img);
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

        save_ppm("carpet.ppm", img);
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

        save_ppm("snowflake.ppm", img);
    }
    else if (strcmp(type, "mandelbrot") == 0)
    {
        mandelbrot(img, width, height, iterations, color);

        save_ppm("mandelbrot.ppm", img);
    }
    else if (strcmp(type, "julia") == 0)
    {
        double cx, cy;
        printf("Enter the real part of c: ");
        scanf("%lf", &cx);
        printf("Enter the imaginary part of c: ");
        scanf("%lf", &cy);

        julia(img, width, height, iterations, color, cx, cy);

        save_ppm("julia.ppm", img);
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

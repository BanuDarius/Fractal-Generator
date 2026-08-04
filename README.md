# Fractal Generator

Developed a fractal generator that combines efficient C implementations of mathematical algorithms with an intuitive Python interface built using Gradio. It supports the generation of famous fractals like Sierpinski Triangle, Koch Curve, Mandelbrot Sets and more, rendering them in the PPM (P3) image format.

## Features

- Generates the following fractals:
  - Sierpinski Triangle (recursive and Chaos Game methods)
  - Sierpinski Carpet
  - Koch Curve 
  - Koch Snowflake
  - Mandelbrot Set
  - Julia Set (with customizable complex parameter)

- Supports 7 predefined RGB colors for fractal coloring

- Colors points inside Mandelbrot and Julia Sets with a base color and outside points with grayscale gradients based on iteration count, visually representing escape speed

- Allows custom image dimensions (height and width)

- Allows setting the number of iterations for fractal generation

- Implements iteration limit optimization:
  - To balance performance and visual quality, the program caps the number of iterations for certain fractals because beyond that level the image looks essentially the same
  - The Sierpinski Triangle’s Chaos Game method does not use iteration capping, allowing for more detailed and efficient rendering

- Outputs fractals as PPM image files, which are then loaded and converted by the Python GUI using the Pillow library for seamless visualization within Gradio

- Provides a user-friendly GUI for easy parameter input and visualization


## Installation

### 1. Clone the repository:
   ```bash
   git clone https://github.com/adrianageamanu/Fractal-Generator.git
   cd Fractal-Generator
   ```
### 2. Set up a virtual environment:
   ```bash
   python3 -m venv venv  # Try 'python' if 'python3' is not available
   source venv/bin/activate
   ```
### 3. Install Python dependencies:
   ```bash
   pip install gradio Pillow
   ```


## Usage

### 1. Compile with Make:
   ```bash
   make
   ```
### 2. Run the Python script:
   ```bash
   python3 gui.py  # Try 'python' if 'python3' is not available
   ```
- The Python script runs a local web server that opens a GUI in your browser, having some default parameters set. You can adjust fractal parameters and instantly see the results, all running locally on your machine.


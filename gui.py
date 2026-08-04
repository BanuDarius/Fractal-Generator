import gradio # pyright: ignore[reportMissingImports]
import subprocess
from PIL import Image # pyright: ignore[reportMissingImports]

def generate_fractal(fractal_type, iterations, color, width, height, cx, cy):
    name_map = {
        'Sierpinski Triangle': 'triangle',
        'Koch Curve': 'curve',
        'Koch Snowflake': 'snowflake',
        'Sierpinski Carpet': 'carpet',
        'Mandelbrot Set': 'mandelbrot',
        'Julia Set': 'julia',
    }

    fractal_key = name_map.get(fractal_type, fractal_type.lower())

    input_data = f"{fractal_key}\n{iterations}\n{color}\n{width}\n{height}\n{cx}\n{cy}"
    proc = subprocess.run(
        ['bin/fractals'],
        input=input_data,
        text=True,
        capture_output=True
    )
    print(proc.stdout)

    filename = f"output/{fractal_key}.ppm"
    img = Image.open(filename)
    return img

iface = gradio.Interface(
    fn=generate_fractal,
    inputs=[
        gradio.Dropdown([
            'Sierpinski Triangle',
            'Koch Curve',
            'Koch Snowflake',
            'Sierpinski Carpet',
            'Mandelbrot Set',
            'Julia Set'
        ], label="Fractal Type"),
        gradio.Number(value=0, label="Number of iterations", precision=0),
        gradio.Dropdown(['red', 'green', 'blue', 'yellow', 'cyan', 'magenta', 'white'], label="Color"),
        gradio.Number(value=1024, label="Image Width", precision=0),
        gradio.Number(value=1024, label="Image Height", precision=0),
        gradio.Number(value=-0.8, label="Real part of c (only for Julia Sets)", precision=3),
        gradio.Number(value=0.156, label="Imaginary part of c (only for Julia Sets)", precision=3)
    ],
    outputs="image",
    title="Fractal Generator",
)

iface.launch(pwa=True)

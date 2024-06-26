<img src="imgs/icon.png" width="64" height="64" align="left"></img>
# Snail
A small SVG library for simple graphics rendering and experiments.

## Example
Code below demonstrates how to draw the Snail logo:

```C
#include "snail/snail.h"

void draw_logo(void) {
    // create canvas
    snl_canvas_t canvas = snl_canvas_create(512, 384);
    snl_canvas_preallocate(&canvas, 2048);

    // add gradient
    snl_canvas_add_gradient_linear(
        &canvas, "lg0", 
        SNL_COLOR(32, 183, 157, 255),   // colorA
        SNL_COLOR(152, 250, 174, 255),  // colorB
        0, 100, 100, 100, 0             // offsetA, offsetB, opacityA, opacityB, angle
    );

    // draw background
    snl_canvas_render_rectangle(
        &canvas, 
        SNL_POINT(0, 0), 
        SNL_POINT(canvas.width, canvas.height), 
        13, 
        SNL_APPEARANCE(1, 1, SNL_COLOR_PALEWHITE, 1, SNL_COLOR_PALEWHITE, NULL, NULL)
    );

    // draw snail body
    snl_canvas_render_rectangle(
        &canvas, 
        SNL_POINT(0.03*canvas.width, 2.055*canvas.height/3), 
        SNL_POINT(canvas.width*0.95, 0.9*canvas.height/3.2),
        81, 
        SNL_APPEARANCE(21, 1, SNL_COLOR_NONE, 0, SNL_COLOR_NONE, NULL, "lg0")
    );

    // draw left eye
    snl_canvas_render_rectangle(
        &canvas, 
        SNL_POINT(3.5*canvas.width/5, 1.32*canvas.height/3), 
        SNL_POINT(0.07*canvas.width, 0.2*canvas.height),
        81, 
        SNL_APPEARANCE(0, 1, SNL_COLOR_NONE, 1, SNL_COLOR_NONE, NULL, "lg0")
    );

    // draw right eye
    snl_canvas_render_rectangle(
        &canvas, 
        SNL_POINT(4*canvas.width/5, 1.32*canvas.height/3), 
        SNL_POINT(0.07*canvas.width, 0.2*canvas.height),
        81, 
        SNL_APPEARANCE(0, 1, SNL_COLOR_NONE, 1, SNL_COLOR_NONE, NULL, "lg0")
    );

    // draw text 'made with'
    snl_canvas_render_text_styled(
        &canvas,
        SNL_POINT(0.1*canvas.width, 4.3*canvas.height/5), 
        "made with", 
        SNL_APPEARANCE(0, 1, SNL_COLOR_NONE, 1, SNL_COLOR_NONE, NULL, "lg0"),
        SNL_TEXT_STYLE(45, 0, SNL_FONT_GILL_SANS, SNL_FONT_WEIGHT_BOLD, SNL_FONT_STYLE_ITALIC, SNL_TEXT_NONE)
    );

    // draw text 'Snail'
    snl_canvas_render_text_styled(
        &canvas,
        SNL_POINT(1.17*canvas.width/2, 4.35*canvas.height/5), 
        "Snail", 
        SNL_APPEARANCE(1, 1, SNL_COLOR_BLACK, 1, SNL_COLOR_NONE, NULL, "lg0"),
        SNL_TEXT_STYLE(60, 0, SNL_FONT_LUMINARI, SNL_FONT_WEIGHT_BOLD, SNL_FONT_STYLE_NORMAL, SNL_TEXT_NONE)
    );

    // draw snail shell
    const float circle_radius[3] = { 2.2*canvas.height/7, 1.4*canvas.height/7, 0.8*canvas.height/7 };
    for (size_t i = 0; i < 3; i++) { 
        snl_canvas_render_circle(
            &canvas, SNL_POINT(canvas.width/3, 2.5*canvas.height/7), circle_radius[i], 
            SNL_APPEARANCE(32, i == 2 ? 0 : 1, SNL_COLOR_NONE, i == 2 ? 1 : 0, SNL_COLOR_NONE, NULL, "lg0")
        );
    }

    // save and destroy canvas
    snl_canvas_save(&canvas, "../imgs/logo.svg");
    snl_canvas_destroy(&canvas);
}
```

This produces the SVG below:

<img src="imgs/logo.svg" width="348">

## Building the library
```sh
# clone repo with submodules
$ git clone --recurse-submodules https://github.com/kirillsaidov/snail.git

# build library
$ cd snail/
$ ./build.sh    # linux, macos
$ ./build.bat   # windows
```
You will find the static library inside the `lib/` folder.

## Usage
Create a new project and copy over the neccessary files:
```
|- my_project
    |- third_party
        |- vita  (from 'inc/' folder)
        |- snail (from 'inc/' folder)
    |- lib
        |- libvita.a
        |- libsnail.a
    |- main.c
```
Vita can be found [here](https://github.com/kirillsaidov/vita). Then, to build the project, execute:
```
$ gcc main.c -o main -Ithird_party -Llib -lsnail -lvita
$ ./main
```
Check out the test [Makefile](./tests/Makefile) for more details.

## LICENSE
All code is licensed under the BSL license.


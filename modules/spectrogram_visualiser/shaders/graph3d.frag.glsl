#version 330 core

in vec4 graph_coord;
out vec4 fragColor;
uniform vec4 colour;

void main(void) {
    float factor;
    if (gl_FrontFacing)
    factor = 1.0;
    else
    factor = 0.5;

    fragColor = (graph_coord / 2.0 + 0.5) * factor * colour;
}


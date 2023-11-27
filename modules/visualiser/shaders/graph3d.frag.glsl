#version 330 core

in vec4 graph_coord;
out vec4 fragColor;

uniform sampler1D colourmap;

void main(void) {
    vec4 factor;
    if (gl_FrontFacing)
    factor = vec4(1.0, 1.0, 1.0, 1.0);
    else
    factor = vec4(0.2, 0.2, 0.2, 1.0);

    fragColor = texture(colourmap, (graph_coord.z + 1.0) / 2.0).rgba * factor;
}


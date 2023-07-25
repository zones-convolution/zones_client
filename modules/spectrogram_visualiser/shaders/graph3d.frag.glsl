#version 330 core

in vec4 graph_coord;
out vec4 fragColor;

void main(void) {
    fragColor = graph_coord / 2.0 + 0.5;
}


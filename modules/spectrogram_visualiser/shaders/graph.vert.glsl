#version 330 core

layout (location = 0) in vec2 coord2d;
out vec4 f_color;

float offset_x = 0.0;
float scale_x = 1.0;

void main(void) {
    gl_Position = vec4((coord2d.x + offset_x) * scale_x, coord2d.y, 0, 1);
    f_color = vec4(coord2d.xy / 2.0 + 0.5, 1, 1);
}
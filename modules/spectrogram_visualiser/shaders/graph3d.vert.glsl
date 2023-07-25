#version 330 core

layout (location = 0) in vec2 coord2d;
out vec4 graph_coord;
uniform sampler2D graph_texture;

void main(void) {
    graph_coord = vec4(coord2d, 0, 1);
    graph_coord.z = (texture(graph_texture, graph_coord.xy / 2.0 + 0.5).r);
    gl_Position = vec4(coord2d, graph_coord.z, 1);
}

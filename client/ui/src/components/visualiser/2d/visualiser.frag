in vec4 graphCoord;
out vec4 fragColor;

uniform sampler2D colourMap;
uniform sampler2D render;

uniform float time;


void main(void) {
    float intensity = clamp(
        texture2D(render, vec2(graphCoord.x + 0.5, graphCoord.y + 0.5)).r,
        0.0,
        1.0
    );

    vec4 colour = texture(colourMap, vec2(intensity, 0.0)).rgba;
    fragColor = colour;
}
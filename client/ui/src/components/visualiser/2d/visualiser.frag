in vec4 graphCoord;
out vec4 fragColor;

uniform sampler2D colourMap;
uniform sampler2D render;
uniform sampler2D scale;
uniform float contrast;
uniform float time;
uniform float sensitivity;


void main(void) {
    float sampleY = texture2D(
        scale,
        vec2(
            graphCoord.y + 0.5,
            0.0
        )
    ).r;

    float intensity = clamp(
        texture2D(render, vec2(graphCoord.x + 0.5, sampleY)).r * sensitivity,
        0.0,
        1.0
    );

    if (contrast > 0.0) {
        intensity = log(1.0 + intensity * contrast) / log(1.0 + contrast);
    }

    vec4 colour = texture(colourMap, vec2(intensity, 0.0)).rgba;
    fragColor = colour;
}
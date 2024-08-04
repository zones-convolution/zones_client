uniform sampler2D render;
uniform sampler2D scale;
uniform float contrast;
uniform float sensitivity;

out float intensity;

void main() {
    vec4 graphCoord = vec4(position, 1.0);

    float sampleY = texture2D(
        scale,
        vec2(
            graphCoord.y + 0.5,
            0.0
        )
    ).r;

    intensity = clamp(
        texture2D(render, vec2(graphCoord.x + 0.5, sampleY)).r * sensitivity,
        0.0,
        1.0
    );

    if (contrast > 0.0) {
        intensity = log(1.0 + intensity * contrast) / log(1.0 + contrast);
    }

    graphCoord.z = intensity;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * graphCoord;
}
in float intensity;
uniform sampler2D colourMap;
uniform float sensitivity;

out vec4 fragColor;

void main(void) {
    vec4 factor;
    if (gl_FrontFacing)
    factor = vec4(1.0, 1.0, 1.0, 1.0);
    else
    factor = vec4(0.2, 0.2, 0.2, 1.0);

    vec4 colour = texture(colourMap, vec2(intensity, 0.0)).rgba;

    fragColor = colour * factor;
}
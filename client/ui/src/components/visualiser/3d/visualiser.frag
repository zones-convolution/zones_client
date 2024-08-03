in vec4 graphCoord;
out vec4 fragColor;
uniform sampler2D colourMap;

void main(void) {
    vec4 factor;
    if (gl_FrontFacing)
    factor = vec4(1.0, 1.0, 1.0, 1.0);
    else
    factor = vec4(0.2, 0.2, 0.2, 1.0);

    fragColor = texture(colourMap, vec2((graphCoord.z + 1.0) / 2.0), 0.0).rgba * factor;
}
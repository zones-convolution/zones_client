#version 330 core

in vec4 graph_coord;
out vec4 fragColor;
uniform vec4 colour;

const float baseToMidBound = 0.02;
const float midToTopBound = 0.7;

vec4 getColorFromHeight(float y)
{
    float normalised_y = (y + 1.0) * 0.5;

    vec3 baseColour = vec3(0.050383, 0.029803, 0.527975);
    vec3 midColour = vec3(0.819651, 0.306812, 0.448306);
    vec3 topColour = vec3(0.987621, 0.815978, 0.144363);
    vec3 highlightColour = vec3(0.940015, 0.975158, 0.131326);

    vec3 interpolatedColor;
    if (normalised_y < baseToMidBound)
    {
        interpolatedColor = mix(baseColour, midColour, normalised_y / baseToMidBound);
    }
    else if (normalised_y < midToTopBound)
    {
        interpolatedColor = mix(midColour, topColour, (normalised_y - baseToMidBound) / (midToTopBound - baseToMidBound));
    }
    else
    {
        interpolatedColor = mix(topColour, highlightColour, (normalised_y - midToTopBound) / (1.0 - midToTopBound));
    }

    return vec4(interpolatedColor, 1.0);
}

void main(void) {
    vec4 factor;
    if (gl_FrontFacing)
    factor = vec4(1.0, 1.0, 1.0, 1.0);
    else
    factor = vec4(0.2, 0.2, 0.2, 1.0);

    fragColor = getColorFromHeight(graph_coord.z) * factor * colour;
}


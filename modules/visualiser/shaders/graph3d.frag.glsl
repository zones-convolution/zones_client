#version 330 core

in vec4 graph_coord;
out vec4 fragColor;
uniform vec4 colour;

const float blueToYellowBound = 0.1;
const float yellowToGreenBound = 0.8;

vec4 getColorFromHeight(float y)
{
    vec3 blueColor = vec3(0.0, 0.2, 1.0);
    vec3 yellowColor = vec3(1.0, 1.0, 0.0);
    vec3 greenColor = vec3(0.0, 1.0, 0.0);
    vec3 redColor = vec3(1.0, 0.0, 0.0);

    vec3 interpolatedColor;
    if (y < blueToYellowBound)
    {
        interpolatedColor = mix(blueColor, yellowColor, y / blueToYellowBound);
    }
    else if (y < yellowToGreenBound)
    {
        interpolatedColor = mix(yellowColor, greenColor, (y - blueToYellowBound) / (yellowToGreenBound - blueToYellowBound));
    }
    else
    {
        interpolatedColor = mix(greenColor, redColor, (y - yellowToGreenBound) / (1.0 - yellowToGreenBound));
    }

    return vec4(interpolatedColor, 1.0);
}

void main(void) {
    float factor;
    if (gl_FrontFacing)
    factor = 1.0;
    else
    factor = 0.2;

    fragColor = getColorFromHeight(abs(graph_coord.z)) * factor * colour;
}


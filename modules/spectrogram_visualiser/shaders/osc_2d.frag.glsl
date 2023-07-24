#version 330 core

// Define a 2D vector to store the screen resolution
uniform vec2 resolution;

// Array to store audio sample data
uniform float audioSampleData[256];

// Function to get the amplitude for a given x position on the screen
void getAmplitudeForXPos(in float xPos, out float audioAmplitude)
{
    // Buffer size - 1
    float perfectSamplePosition = 255.0 * xPos / resolution.x;

    // Find the indices of the two closest samples in the audio data
    int leftSampleIndex = int(floor(perfectSamplePosition));
    int rightSampleIndex = int(ceil(perfectSamplePosition));

    // Interpolate between the two samples to get the amplitude at the given x position
    audioAmplitude = mix(audioSampleData[leftSampleIndex], audioSampleData[rightSampleIndex], fract(perfectSamplePosition));
}

// Define a constant for the thickness of the wave
#define THICKNESS 0.02

layout(location = 0) out vec4 fragColor;

float fadeAlphaOnEdges(vec2 position, vec2 size, float percentage) {
    // Calculate the distance from the center of the screen in the X dimension
    float centerX = size.x * 0.5;
    float distanceX = abs(position.x - centerX);

    // Calculate the maximum distance at which alpha should be 1 in the X dimension
    float maxXDistance = size.x * (0.5 - percentage * 0.5);

    // Calculate the alpha value based on the distance in the X dimension
    float alphaX = clamp(1.0 - distanceX / maxXDistance, 0.0, 1.0);

    // Calculate the distance from the center of the screen in the Y dimension
    float centerY = size.y * 0.5;
    float distanceY = abs(position.y - centerY);

    // Calculate the maximum distance at which alpha should be 1 in the Y dimension
    float maxYDistance = size.y * (0.5 - percentage * 0.5);

    // Calculate the alpha value based on the distance in the Y dimension
    float alphaY = clamp(1.0 - distanceY / maxYDistance, 0.0, 1.0);

    // Return the minimum of the two alpha values, so that the alpha value is only 1 if both X and Y distances are within the desired range
    return min(alphaX, alphaY);
}

// Main function that gets called for each pixel on the screen
void main()
{
    // Calculate the normalized y-coordinate of the current pixel
    float y_cord = gl_FragCoord.y / resolution.y;

    // Variable to store the audio amplitude at the current x position
    float amplitude = 0.0;
    // Get the audio amplitude for the current x position
    getAmplitudeForXPos(gl_FragCoord.x, amplitude);

    // Center and reduce the wave amplitude
    amplitude = 0.5 - amplitude * 0.8;

    // Calculate the intensity of the wave at the current pixel
    float intensity = THICKNESS / abs(amplitude - y_cord);
    vec4 colour = vec4(1.0, 1.0, 1.0, 1.0);
    float alpha = fadeAlphaOnEdges(gl_FragCoord.xy, resolution, 0.02);

    // Set the color of the current pixel based on the wave radius
    fragColor = intensity * colour;
}
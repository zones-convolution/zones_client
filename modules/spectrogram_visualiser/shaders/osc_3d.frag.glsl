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
    amplitude = 0.5 - amplitude / 2.5;

    // Calculate the radius of the wave at the current pixel
    float radius = abs(THICKNESS / (amplitude - y_cord));

    // Set the color of the current pixel based on the wave radius
    gl_FragColor = vec4(radius - abs(radius * 0.2), radius - abs(radius * 0.2), radius - abs(radius * 0.2), 1.0);
}
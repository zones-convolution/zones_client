uniform vec2  resolution;
uniform float audioSampleData[256];

void getAmplitudeForXPos (in float xPos, out float audioAmplitude)
{
    // Buffer size - 1
    float perfectSamplePosition = 255.0 * xPos / resolution.x;
    int leftSampleIndex = int (floor (perfectSamplePosition));
    int rightSampleIndex = int (ceil (perfectSamplePosition));
    audioAmplitude = mix (audioSampleData[leftSampleIndex], audioSampleData[rightSampleIndex], fract (perfectSamplePosition));
}

#define THICKNESS 0.02
void main()
{
    float y = gl_FragCoord.y / resolution.y;
    float amplitude = 0.0;
    getAmplitudeForXPos (gl_FragCoord.x, amplitude);
    // Centers & Reduces Wave Amplitude
    amplitude = 0.5 - amplitude / 2.5;
    float r = abs (THICKNESS / (amplitude-y));

    gl_FragColor = vec4 (r - abs (r * 0.2), r - abs (r * 0.2), r - abs (r * 0.2), 1.0);
}
// The value that was output by the vertex shader.
// This MUST have the exact same name that we used in the vertex shader.
varying vec4 fragColour;

void main()
{
    // Set the fragment's colour to the attribute passed in by the
    // vertex shader.
    gl_FragColor = fragColour;
}
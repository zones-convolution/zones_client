// Input attributes.
varying vec4 position;
varying vec4 sourceColour;

// Output to fragment shader.
varying vec4 fragColour;

void main()
{
    // Set the position to the attribute we passed in.
    gl_Position = position;

    // Set the frag colour to the attribute we passed in.
    // This value will be interpolated for us before the fragment
    // shader so the colours we defined ateach vertex will create a
    // gradient across the shape.
    fragColour = sourceColour;
}
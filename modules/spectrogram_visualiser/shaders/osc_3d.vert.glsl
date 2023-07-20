// Define the attribute variable 'position' as a 3D vector.
// This attribute will be provided externally when rendering the vertices.
attribute vec3 position;

// The main function that gets called for each vertex during rendering.
void main()
{
    // Set the position of the vertex in the homogeneous clip space.
    // The 'position' attribute is used as the (x, y, z) coordinates of the vertex,
    // and the fourth component '1.0' is the homogeneous coordinate 'w'.
    // The homogeneous coordinates are required for perspective transformation.
    gl_Position = vec4(position, 1.0);
}
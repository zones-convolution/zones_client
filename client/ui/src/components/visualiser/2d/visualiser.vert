uniform sampler2D graphTexture;
out vec4 graphCoord;

void main() {
    graphCoord = vec4(position, 1.0);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * graphCoord;
}
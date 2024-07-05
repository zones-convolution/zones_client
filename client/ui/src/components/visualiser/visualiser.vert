uniform sampler2D graphTexture;
out vec4 graphCoord;

void main() {
    graphCoord = vec4(position, 1.0);
    //    graphCoord.z = sin(position.x) * sin(position.y);
    graphCoord.z = texture(graphTexture, graphCoord.xy / 2.0 + 0.5).r * 2.0f - 1.0;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * graphCoord;
}
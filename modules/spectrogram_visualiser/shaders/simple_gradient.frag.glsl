void main()
{
    vec4 colour1 = vec4 (1.0, 0.4, 0.6, 1.0);
    vec4 colour2 = vec4 (0.0, 0.8, 0.6, 1.0);
    float alpha = pixelPos.x / 1000.0;
    gl_FragColor = pixelAlpha * mix (colour1, colour2, alpha);
}
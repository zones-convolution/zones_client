void main()
{
    vec4 colour1 = vec4 (0.1, 0.1, 0.9, 1.0);
    vec4 colour2 = vec4 (0.0, 0.8, 0.6, 1.0);
    float distance = distance (pixelPos, vec2 (600.0, 500.0));

    float innerRadius = 200.0;
    float outerRadius = 210.0;

    if (distance < innerRadius)
    gl_FragColor = colour1;
    else if (distance > outerRadius)
    gl_FragColor = colour2;
    else
    gl_FragColor = mix (colour1, colour2, (distance - innerRadius) / (outerRadius - innerRadius));

    gl_FragColor *= pixelAlpha;
}
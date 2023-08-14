VARYING vec2 bounds;

void MAIN()
{
    VERTEX = texture(height, UV0).rgb;
    vec2 desc = vec2(int(xDesc),int(zDesc));
    bounds = abs(desc - UV0);
    POSITION = MODELVIEWPROJECTION_MATRIX * vec4(VERTEX, 1.0);
}

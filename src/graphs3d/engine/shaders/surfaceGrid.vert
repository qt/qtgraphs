VARYING vec3 pos;
VARYING float disc;
void MAIN()
{
    vec2 UV = UV0 * (vertices / range);
    vec3 v = texture(height, UV).xyz;
    pos = v;
    disc = 0;
    if (fill && v.y > graphHeight)
        disc = 1;

    VERTEX = v;
    POSITION = MODELVIEWPROJECTION_MATRIX * vec4(VERTEX, 1.0);
}

void MAIN()
{
    VERTEX = texture(height, UV0).rgb;
    POSITION = MODELVIEWPROJECTION_MATRIX * vec4(VERTEX, 1.0);
}

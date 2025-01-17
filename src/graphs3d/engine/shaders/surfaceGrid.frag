vec4 color;
VARYING vec3 pos;
VARYING float disc;

void MAIN()
{
    if (abs(pos.y) > graphHeight)
        discard;
    if (disc > 0)
        discard;

    color = gridColor;
}

void POST_PROCESS()
{
    COLOR_SUM = color;
}



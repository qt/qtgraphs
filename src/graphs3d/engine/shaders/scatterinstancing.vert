VARYING vec3 pos;
VARYING vec3 vColor;

void MAIN()
{
    pos = VERTEX;
    vec2 gradientUV = vec2(INSTANCE_DATA.x, 0.0);
    vColor = texture(custex, gradientUV).xyz;
    POSITION = INSTANCE_MODELVIEWPROJECTION_MATRIX * vec4(VERTEX, 1.0);
}

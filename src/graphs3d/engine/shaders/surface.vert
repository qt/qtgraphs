VARYING vec3 pos;
VARYING vec2 UV;

void MAIN()
{
    UV = UV0 * (vertCount / size);

    float xStep = xDiff;
    float yStep = yDiff;
    if (flipU)
        xStep *= -1;
    if (flipV)
        yStep *= -1;

    vec2 uvR = UV + vec2(xStep, 0.0);
    if (UV.x > (1.0 - xStep) || UV.x + xStep <= 0)
        uvR = UV - vec2(xStep, 0.0);
    vec2 uvU = UV + vec2(0.0, yStep);
    if (UV.y > (1.0 - yStep) || UV.y + yStep <= 0)
        uvU = UV - vec2(0.0, yStep);
    vec3 v1 = texture(height, UV).rgb;
    vec3 v2 = texture(height, uvR).rgb;
    vec3 v3 = texture(height, uvU).rgb;
    vec3 v21 = v2 - v1;
    vec3 v31 = v3 - v1;

    if (UV.x > (1.0 - xStep) || UV.x + xStep < 0)
        v21 = v1 - v2;
    if (UV.y > (1.0 - yStep) || UV.y + yStep < 0)
        v31 = v1 - v3;

    vec3 n = cross(v21, v31);


    NORMAL = n;
    VERTEX = v1;

    pos = VERTEX;
    vec4 pos = MODELVIEWPROJECTION_MATRIX * vec4(VERTEX, 1.0);
    pos.z += 0.000001;
    POSITION = pos;
}

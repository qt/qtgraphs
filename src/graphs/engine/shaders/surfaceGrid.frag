VARYING vec2 bounds;

void checkBounds() {

    float xBoundsMin = step(-0.01,(bounds.x - rangeMin.x));
    float yBoundsMin = step(-0.01,(bounds.y - rangeMin.y));
    float xBoundsMax = step(-0.01,(rangeMax.x - bounds.x));
    float yBoundsMax = step(-0.01,(rangeMax.y - bounds.y));

    float total = xBoundsMin * yBoundsMin * xBoundsMax * yBoundsMax;
    if(total <= 0)
        discard;
}
void MAIN()
{
    checkBounds();
    BASE_COLOR = gridColor;
}

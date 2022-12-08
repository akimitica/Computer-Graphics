// y = k*x + n

void lineAlg(int x0, int y0, int x1, int y1, COLORREF color)
{
    float k = (y1 - y0) / (x1 - x0);
    float n = y1 - k * x1;
    for (int x = x0; x < x1; ++x)
    {
        float y = k * x + n;
        WritePixel(x, (int)y, color);
    }
}

void DDAAlg(int x0, int y0, int x1, int y1, COLORREF color)
{
    float k = (y1 - y0) / (x1 - x0);
    int y = y0;
    for (int x = x0; x <= x1; x++, y += k)
        WritePixel(x, int(y), color);
}

void Bresanham(int x0, int y0, int x1, int y1, COLORREF color)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int d = 2 * dy - dx;
    int incr1 = 2 * dy;
    int incr2 = 2 * (dy - dx);

    int x = x0;
    int y = y0;
    int xend = x1;
    if (x0>x1)
    {
        x = x1; 
        y = y1; 
        xend = x0;
    }

    WritePixel(pDC, x, y, color);

    while(x < xend)
    {
        x++;
        if (d < 0)
            d+= incr1;
        else
        {
            y++;
            d+=incr2;
        }
        WritePixel(pDC, x, y, color);
    }
}
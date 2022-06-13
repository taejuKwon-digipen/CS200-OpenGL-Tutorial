// Name       : Taeju Kwon
// Assignment : Line Rasterization
// Course     : cs200
// Term & Year: Fall & 2021

#include "draw_line.h"

namespace CS200
{
    void draw_line(int x0, int y0, int x1, int y1, Color color, SetPixel setpixel)
    {
        // TODO
        //Note that the line starts from (x0, y0) and ends at (x1, y1).
        //Use setpixel function to set a pixel with given(x, y) positionand color.
        //void setpixel(int column, int row, Color color)
        //Color values are provided so you can just write :
        //setpixel(x, y, color);

        int dx = 0;
        int dy = 0;
        int d = 0; //선 기울기
        int E = 0; //위
        int NE = 0; //아래
        color = { 1,1,1 };

        if (x0 == x1)
        {
        	while( y0 < y1)
        	{
				setpixel(y0, y1, color);
                y0++;
                y1++;
        	}
        }
        if (y0 == y1)
        {
            while (x0 < x1)
            {
                setpixel(x0, x1, color);
                x0++;
                x1++;
            }
        }

        if (x0 > x1 && y0 < y1) //제  2사분면
        {
            dx = x1 - x0;
            dy = y1 - y0;

            if (dx > dy)
            {
                d = (2 * dy) - dx;
                E = 2 * dy;
                NE = 2 * (dy - dx);

                while (x0 > x1)
                {
                    if (d <= 0)
                    {
                        d += E;
                        x0--;
                    }
                    else
                    {
                        d += NE;
                        x0--;
                        y0++;
                    }
                    setpixel(x0, y0, color);
                }
            }
            else
            {
                d = (2 * dy) - dx;
                E = 2 * dy;
                NE = 2 * (dy - dx);

                while (y0 < y1)
                {
                    if (d <= 0)
                    {
                        d += E;
                        y0++;
                    }
                    else
                    {
                        d += NE;
                        x0--;
                        y0++;
                    }
                    setpixel(x0, y0, color);
                }
            }
        }

        if (x0 > x1 && y0 > y1) //제 3사분면
        {
            dx = x1 - x0;
            dy = y1 - y0;

            if (dx < dy) // octant 5
            {
                E = dx * 2;
                NE = 2 * (dx - dy);
                d = (dy * 2) - (1 / 2 * dx);
                while (x0 > x1)
                {
                    if (d <= 0)
                    {
                        d -= E;
                        x0--;
                    }
                    else
                    {
                        d -= NE;
                        x0--;
                        y0--;
                    }
                    setpixel(x0, y0, color);
                }

            }
            else // octant 4
            {
                E = dx * 2;
                NE = 2 * (dx - dy);
                d = (dy * 2) - (1 / 2 * dx);

                while (y0 > y1)
                {
                    if (d <= 0)
                    {
                        d -= E;
                        y0--;
                    }
                    else
                    {
                        d -= NE;
                        x0--;
                        y0--;
                    }
                    setpixel(x0, y0, color);
                }
            }
        }

        if (x0 < x1 && y0 < y1) // 제 1사분면
        {
            dx = x1 - x0;
            dy = y1 - y0;
            if (dx > dy)
            {
                d = (2 * dy) - dx;
                E = 2 * dy;
                NE = 2 * (dy - dx);

                while (x0 < x1)
                {
                    if (d <= 0)
                    {
                        d += E;
                        x0++;
                    }
                    else
                    {
                        d += NE;
                        x0++;
                        y0++;
                    }
                    setpixel(x0, y0, color);
                }
            }
            else
            {
                d = (2 * dy) - dx;
                E = 2 * dy;
                NE = 2 * (dy - dx);

                while (y0 < y1)
                {
                    if (d <= 0)
                    {
                        d += E;
                        y0++;
                    }
                    else
                    {
                        d += NE;
                        x0++;
                        y0++;
                    }
                    setpixel(x0, y0, color);
                }
            }
        }
        if (x0< x1 && y0 > y1) //제 4사분면
        {
            dx = x1 - x0;
            dy = y1 - y0;

            if (dx < dy)
            {

                E = dx * 2;
                NE = 2 * (dx - dy);
                d = (dy * 2) - (1 / 2 * dx);

                while (y0 < y1)
                {
                    if (d <= 0)
                    {
                        d += E;
                        y0--;
                        setpixel(x0, y0, color);
                    }
                    else
                    {
                        d += NE;
                        x0++;
                        y0--;
                        setpixel(x0, y0, color);
                    }
                }

            }
            else
            {
                E = dx * 2;
                NE = 2 * (dx - dy);
                d = (dy * 2) - (1 / 2 * dx);

                while (x0 > x1)
                {
                    if (d <= 0)
                    {
                        d += E;
                        x0++;
                    }
                    else
                    {
                        d += NE;
                        x0++;
                        y0--;
                    }
                    setpixel(x0, y0, color);
                }

            }
        }



    }
}
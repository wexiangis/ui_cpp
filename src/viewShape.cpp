
#include "viewShape.h"

//递归填充
inline void recursion(unsigned char *map[], int x, int y, int xMax, int yMax, char dir)
{
    if(map[y][x])
        return;
    map[y][x] = 0xFF;
    //
    switch(dir)
    {
        case 'u':
            if(y > 0) recursion(map, x, y-1, xMax, yMax, 'u');
            // if(y < yMax) recursion(map, x, y+1, xMax, yMax, 'd');
            if(x > 0) recursion(map, x-1, y, xMax, yMax, 'l');
            if(x < xMax) recursion(map, x+1, y, xMax, yMax, 'r');
            break;
        case 'd':
            // if(y > 0) recursion(map, x, y-1, xMax, yMax, 'u');
            if(y < yMax) recursion(map, x, y+1, xMax, yMax, 'd');
            if(x > 0) recursion(map, x-1, y, xMax, yMax, 'l');
            if(x < xMax) recursion(map, x+1, y, xMax, yMax, 'r');
            break;
        case 'l':
            if(y > 0) recursion(map, x, y-1, xMax, yMax, 'u');
            if(y < yMax) recursion(map, x, y+1, xMax, yMax, 'd');
            if(x > 0) recursion(map, x-1, y, xMax, yMax, 'l');
            // if(x < xMax) recursion(map, x+1, y, xMax, yMax, 'r');
            break;
        case 'r':
            if(y > 0) recursion(map, x, y-1, xMax, yMax, 'u');
            if(y < yMax) recursion(map, x, y+1, xMax, yMax, 'd');
            // if(x > 0) recursion(map, x-1, y, xMax, yMax, 'l');
            if(x < xMax) recursion(map, x+1, y, xMax, yMax, 'r');
            break;
        case 0:
            if(y > 0) recursion(map, x, y-1, xMax, yMax, 'u');
            if(y < yMax) recursion(map, x, y+1, xMax, yMax, 'd');
            if(x > 0) recursion(map, x-1, y, xMax, yMax, 'l');
            if(x < xMax) recursion(map, x+1, y, xMax, yMax, 'r');
            break;
    }
}

Polygon::Polygon(int line)
{
    if(line < 0)
        return;
    //
    LINE = line;
    if(LINE == 0)
        return;
    //
    DOT = new int[(LINE+1)*2];
    //
    if(LINE == 1)
        ;
}

Polygon::Polygon(int line, int *xy)
{
    
}

Polygon::~Polygon()
{
    if(DOT)
        delete[] DOT;
}


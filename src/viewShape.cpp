
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

//功能: 指定起止坐标, 返回两点间画线的Y坐标数组
//参数: xStart, yStart, xEnd, yEnd : 起止坐标
//    dotX, dotY : Y坐标数组起始地址, 需要自己先分配好内存再传入
//返回: 点数
int _getDotFromLine(int xStart, int yStart, int xEnd, int yEnd, int *dotX, int *dotY)
{
	unsigned short t; 
	int xerr = 0, yerr = 0;
	int delta_x, delta_y;
	int distance; 
	int incx, incy, xCount, yCount; 
	//
	delta_x = xEnd - xStart; //计算坐标增量 
	delta_y = yEnd - yStart; 
	xCount = xStart; 
	yCount = yStart; 
	//
	if(delta_x > 0)
	    incx = 1; //设置单步方向 
	else if(delta_x == 0)
	    incx = 0;//垂直线 
	else 
	{
	    incx = -1;
	    delta_x = -delta_x;
	} 
	//
	if(delta_y > 0)
	    incy = 1; 
	else if(delta_y == 0)
	    incy = 0;//水平线 
	else
	{
	    incy = -1;
	    delta_y = -delta_y;
	} 
	//
	if(delta_x > delta_y)
	    distance = delta_x; //选取基本增量坐标轴 
	else 
	    distance = delta_y; 
	//
	for(t = 0; t <= distance + 1; t++)//画线输出 
	{
        *dotX++ = xCount;
        *dotY++ = yCount;
        
		xerr += delta_x ; 
		yerr += delta_y ; 
		if(xerr > distance) 
		{ 
			xerr -= distance; 
			xCount += incx; 
		} 
		if(yerr > distance) 
		{ 
			yerr -= distance; 
			yCount += incy; 
		} 
	}
    return distance + 2;
}


Polygon::Polygon(int line)
{
    if(line < 0)
        return;
    //
    LINE = line;
    //
    if(LINE == 0)
        return;
    //
    DOT = new int[(LINE+1)*2];
    //
    if(LINE == 1)
    {
        DOT[0] = -1;
        DOT[1] = 0;
        DOT[2] = 1;
        DOT[3] = 0;
    }
    else if(LINE == 2)
    {
        DOT[0] = -10;
        DOT[1] = 0;
        DOT[2] = 0;
        DOT[3] = 17;
        DOT[4] = 10;
        DOT[5] = 0;
    }
    else if(LINE == 3)
    {
        ;
    }
    else if(LINE == 4)
    {
        ;
    }
    else if(LINE == 5)
    {
        ;
    }
    else if(LINE == 6)
    {
        ;
    }
}

Polygon::Polygon(int line, int *xy)
{
    
}

Polygon::~Polygon()
{
    if(DOT)
        delete[] DOT;
}



#include "viewShape.h"
#include <math.h>

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

const double VS_PI = 3.14159265358979323846;

Polygon::Polygon(int line = 0)
{
    if(line < 1)//圆
    {
        LINE = 0;
        DOT = 0;
        return;
    }
    //
    LINE = line;
    DOT = new int[(LINE+1)*2];
    //
    if(LINE == 1)//直线
    {
        DOT[0] = -1;
        DOT[1] = 0;
        DOT[2] = 1;
        DOT[3] = 0;
    }
    else if(LINE == 2)//折线
    {
        DOT[0] = -9;
        DOT[1] = -5;
        DOT[2] = 0;
        DOT[3] = 10;
        DOT[4] = 9;
        DOT[5] = -5;
    }
    else//多边形
    {
        int radius = 100;
        double degreeCount, degreeStart;//按角度旋转 找到各定点坐标
        //
        if(LINE%2)//单数
            degreeStart = VS_PI/2;
        else//双数
            degreeStart = VS_PI/2 - VS_PI*2/LINE/2;
        //
        for(int i = 0, j = 0; i < LINE; i++)
        {
            degreeCount = degreeStart - VS_PI*2*i/LINE;
            //计算X,Y坐标
            DOT[j] = round(radius*cos(degreeCount));
            DOT[j+1] = round(radius*sin(degreeCount));
            //
            j += 2;
        }
    }
}

Polygon::Polygon(int line, int *xy)
{
    if(line < 0 || !xy)
    {
        LINE = 0;
        return;
    }
    //
    LINE = line;
    //
    if(LINE == 0)//圆
        return;
    //
    DOT = new int[(LINE+1)*2];
    //
    for(int i = 0, j = 0; i < LINE; i++)
    {
        DOT[j] = xy[j];
        j += 1;
        DOT[j] = xy[j];
        j += 1;
    }
}

Polygon::~Polygon()
{
    if(DOT)
        delete[] DOT;
}

unsigned char* Polygon::get_grid(int rad, int rad2, int angle, int degree, unsigned char *grid, int *gw, int *gh, unsigned char weight)
{
    int RAD = rad;
    //画圆相关
    int circle_a, circle_b;
    int circle_di;
    int circle_rad = RAD;
    int circle_size = rad2 > RAD ? 0 : (RAD - rad2);
    //栅格图相关
    int xSize = RAD*2+1, ySize = xSize;
    int xMid = RAD+1, yMid = xMid;
    unsigned char *mem;
    unsigned char **memBUff;
    //栅格图指针准备
    if(grid)
    {
        mem = grid;
        //
        if(gw && (*gw < xSize || *gw < ySize)){
            RAD = (*gw/2)-1;
            circle_rad = RAD;
        }else if(gh && (*gh < xSize || *gh < ySize)){
            RAD = (*gh/2)-1;
            circle_rad = RAD;
        }
        //
        if(gw){
            xSize = *gw;
            xMid = xSize/2;
        }
        if(gh){
            ySize = *gh;
            yMid = ySize/2;
        }
    }
    else
        mem = new unsigned char[xSize*ySize];
    memBUff = new unsigned char*[ySize];
    for(int i = 0, j = 0; i < ySize; i++)
    {
        memBUff[i] = &mem[j];
        j += xSize;
    }
    //画扇形,扇圆环
    if(angle > 0 && angle < 360)
    {
        //扇形
        int sectorBuff[RAD+1][2];
        int sectorCount;
        double degreeStart, degreeEnd;
        //
        degreeStart = (degree%360) - (angle%360)/2;
        if(degreeStart < 0)
            degreeStart += 360;
        else if(degreeStart >= 360)
            degreeStart -= 360;
        degreeEnd = degreeStart + angle - 1;
        //
        for(;circle_rad > RAD - circle_size && circle_rad > 0; circle_rad--)
        {
            circle_a = 0;
            circle_b = circle_rad;
            circle_di = 3 - (circle_rad<<1);
            sectorCount = 0;
            while(circle_a <= circle_b)
            {
                sectorBuff[sectorCount][0] = circle_a;
                sectorBuff[sectorCount][1] = circle_b;
                sectorCount += 1;
                //
                circle_a++;
                //使用Bresenham算法画圆
                if(circle_di < 0)
                    circle_di += 4*circle_a + 6;
                else
                {
                    circle_di += 10 + 4*(circle_a - circle_b);
                    circle_b--;
                }
            }
            //
            if(!sectorCount)
                continue;
            //
            double degreeDivOf45 = 45/(double)sectorCount, degreeCount;
            int temp, block, blockCount;//degreeStart 所在线(总共8*sectorCount / 所在块(总共8块 / 所在线计数
            if(degreeStart < 0)
                temp = round((360+degreeStart)/degreeDivOf45);
            else
                temp = round(degreeStart/degreeDivOf45);
            block = temp/sectorCount;
            blockCount = temp%sectorCount;
            //
            // for(degreeCount = temp*degreeDivOf45;
            for(degreeCount = degreeStart;
                degreeCount <= degreeEnd;
                degreeCount+=degreeDivOf45)
            {
                switch(block)
                {
                    case 0:
                        memBUff[yMid - sectorBuff[blockCount][1]][xMid + sectorBuff[blockCount][0]] = weight;
                        memBUff[yMid - sectorBuff[blockCount][1] + 1][xMid + sectorBuff[blockCount][0]] = weight;
                        break;
                    case 1:
                        memBUff[yMid - sectorBuff[sectorCount-blockCount-1][0]][xMid + sectorBuff[sectorCount-blockCount-1][1]] = weight;
                        memBUff[yMid - sectorBuff[sectorCount-blockCount-1][0]][xMid + sectorBuff[sectorCount-blockCount-1][1] - 1] = weight;
                        break;
                    case 2:
                        memBUff[yMid + sectorBuff[blockCount][0]][xMid + sectorBuff[blockCount][1]] = weight;
                        memBUff[yMid + sectorBuff[blockCount][0]][xMid + sectorBuff[blockCount][1] - 1] = weight;
                        break;
                    case 3:
                        memBUff[yMid + sectorBuff[sectorCount-blockCount-1][1]][xMid + sectorBuff[sectorCount-blockCount-1][0]] = weight;
                        memBUff[yMid + sectorBuff[sectorCount-blockCount-1][1] - 1][xMid + sectorBuff[sectorCount-blockCount-1][0]] = weight;
                        break;
                    case 4:
                        memBUff[yMid + sectorBuff[blockCount][1]][xMid - sectorBuff[blockCount][0]] = weight;
                        memBUff[yMid + sectorBuff[blockCount][1] - 1][xMid - sectorBuff[blockCount][0]] = weight;
                        break;
                    case 5:
                        memBUff[yMid + sectorBuff[sectorCount-blockCount-1][0]][xMid - sectorBuff[sectorCount-blockCount-1][1]] = weight;
                        memBUff[yMid + sectorBuff[sectorCount-blockCount-1][0]][xMid - sectorBuff[sectorCount-blockCount-1][1] + 1] = weight;
                        break;
                    case 6:
                        memBUff[yMid - sectorBuff[blockCount][0]][xMid - sectorBuff[blockCount][1]] = weight;
                        memBUff[yMid - sectorBuff[blockCount][0]][xMid - sectorBuff[blockCount][1] + 1] = weight;
                        break;
                    case 7:
                        memBUff[yMid - sectorBuff[sectorCount-blockCount-1][1]][xMid - sectorBuff[sectorCount-blockCount-1][0]] = weight;
                        memBUff[yMid - sectorBuff[sectorCount-blockCount-1][1] + 1][xMid - sectorBuff[sectorCount-blockCount-1][0]] = weight;
                        break;
                }
                //
                if(++blockCount == sectorCount)
                {
                    blockCount = 0;
                    if(++block == 8)
                        block = 0;
                }
            }
        }
    }
    //画圆
    else
    {
        for(;circle_rad > RAD - circle_size && circle_rad > 0; circle_rad--)
        {
            circle_a = 0;
            circle_b = circle_rad;
            circle_di = 3 - (circle_rad<<1);
            while(circle_a <= circle_b)
            {
                //1
                memBUff[yMid - circle_b][xMid + circle_a] = weight;
                memBUff[yMid - circle_b + 1][xMid + circle_a] = weight;
                //2
                memBUff[yMid - circle_a][xMid + circle_b] = weight;
                memBUff[yMid - circle_a][xMid + circle_b - 1] = weight;
                //3
                memBUff[yMid + circle_a][xMid + circle_b] = weight;
                memBUff[yMid + circle_a][xMid + circle_b - 1] = weight;
                //4
                memBUff[yMid + circle_b][xMid + circle_a] = weight;
                memBUff[yMid + circle_b - 1][xMid + circle_a] = weight;
                //5
                memBUff[yMid + circle_b][xMid - circle_a] = weight;
                memBUff[yMid + circle_b - 1][xMid - circle_a] = weight;
                //6
                memBUff[yMid + circle_a][xMid - circle_b] = weight;
                memBUff[yMid + circle_a][xMid - circle_b + 1] = weight;
                //7
                memBUff[yMid - circle_a][xMid - circle_b] = weight;
                memBUff[yMid - circle_a][xMid - circle_b + 1] = weight;
                //8
                memBUff[yMid - circle_b][xMid - circle_a] = weight;
                memBUff[yMid - circle_b + 1][xMid - circle_a] = weight;
                //
                circle_a++;
                //使用Bresenham算法画圆
                if(circle_di < 0)
                    circle_di += 4*circle_a + 6;
                else
                {
                    circle_di += 10 + 4*(circle_a - circle_b);
                    circle_b--;
                }
            }
        }
    }
    //
    if(gw) *gw = xSize;
    if(gh) *gh = ySize;
    //
    delete[] memBUff;
    //
    return mem;
}

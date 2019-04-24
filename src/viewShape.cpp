
#include "viewShape.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

const double VS_PI = 3.14159265358979323846;

unsigned char* _ellipse(int radX, int radY, int lineSize, int angle, int degree, unsigned char weight, unsigned char *grid, int *gw, int *gh, int *xyCentre = NULL)
{
    int RAD = radX > radY ? radX : radY;
    char mode = radX > radY ? 0 : 1;
    double multi = mode ? (double)radX/RAD : (double)radY/RAD;
    //画圆相关
    int circle_a, circle_b;
    int circle_di;
    int circle_rad = RAD;
    int circle_size = lineSize;
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
        }
        if(gh && (*gh < xSize || *gh < ySize)){
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
        //
        if(xyCentre){
            xMid = xyCentre[0];
            yMid = xyCentre[1];
        }
    }
    else
        mem = new unsigned char[xSize*ySize];
    //
    memBUff = new unsigned char*[ySize];
    for(int i = 0, j = 0; i < ySize; i++){
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
                if(mode)//缩放X轴
                {
                    switch(block)
                    {
                        case 0:
                            memBUff[yMid - sectorBuff[blockCount][1]][xMid + (int)(sectorBuff[blockCount][0]*multi)] = weight;
                            memBUff[yMid - sectorBuff[blockCount][1] + 1][xMid + (int)(sectorBuff[blockCount][0]*multi)] = weight;
                            break;
                        case 1:
                            memBUff[yMid - sectorBuff[sectorCount-blockCount-1][0]][xMid + (int)(sectorBuff[sectorCount-blockCount-1][1]*multi)] = weight;
                            memBUff[yMid - sectorBuff[sectorCount-blockCount-1][0]][xMid + (int)(sectorBuff[sectorCount-blockCount-1][1]*multi) - 1] = weight;
                            break;
                        case 2:
                            memBUff[yMid + sectorBuff[blockCount][0]][xMid + (int)(sectorBuff[blockCount][1]*multi)] = weight;
                            memBUff[yMid + sectorBuff[blockCount][0]][xMid + (int)(sectorBuff[blockCount][1]*multi) - 1] = weight;
                            break;
                        case 3:
                            memBUff[yMid + sectorBuff[sectorCount-blockCount-1][1]][xMid + (int)(sectorBuff[sectorCount-blockCount-1][0]*multi)] = weight;
                            memBUff[yMid + sectorBuff[sectorCount-blockCount-1][1] - 1][xMid + (int)(sectorBuff[sectorCount-blockCount-1][0]*multi)] = weight;
                            break;
                        case 4:
                            memBUff[yMid + sectorBuff[blockCount][1]][xMid - (int)(sectorBuff[blockCount][0]*multi)] = weight;
                            memBUff[yMid + sectorBuff[blockCount][1] - 1][xMid - (int)(sectorBuff[blockCount][0]*multi)] = weight;
                            break;
                        case 5:
                            memBUff[yMid + sectorBuff[sectorCount-blockCount-1][0]][xMid - (int)(sectorBuff[sectorCount-blockCount-1][1]*multi)] = weight;
                            memBUff[yMid + sectorBuff[sectorCount-blockCount-1][0]][xMid - (int)(sectorBuff[sectorCount-blockCount-1][1]*multi) + 1] = weight;
                            break;
                        case 6:
                            memBUff[yMid - sectorBuff[blockCount][0]][xMid - (int)(sectorBuff[blockCount][1]*multi)] = weight;
                            memBUff[yMid - sectorBuff[blockCount][0]][xMid - (int)(sectorBuff[blockCount][1]*multi) + 1] = weight;
                            break;
                        case 7:
                            memBUff[yMid - sectorBuff[sectorCount-blockCount-1][1]][xMid - (int)(sectorBuff[sectorCount-blockCount-1][0]*multi)] = weight;
                            memBUff[yMid - sectorBuff[sectorCount-blockCount-1][1] + 1][xMid - (int)(sectorBuff[sectorCount-blockCount-1][0]*multi)] = weight;
                            break;
                    }
                }
                else//缩放Y轴
                {
                    switch(block)
                    {
                        case 0:
                            memBUff[yMid - (int)(sectorBuff[blockCount][1]*multi)][xMid + sectorBuff[blockCount][0]] = weight;
                            memBUff[yMid - (int)(sectorBuff[blockCount][1]*multi) + 1][xMid + sectorBuff[blockCount][0]] = weight;
                            break;
                        case 1:
                            memBUff[yMid - (int)(sectorBuff[sectorCount-blockCount-1][0]*multi)][xMid + sectorBuff[sectorCount-blockCount-1][1]] = weight;
                            memBUff[yMid - (int)(sectorBuff[sectorCount-blockCount-1][0]*multi)][xMid + sectorBuff[sectorCount-blockCount-1][1] - 1] = weight;
                            break;
                        case 2:
                            memBUff[yMid + (int)(sectorBuff[blockCount][0]*multi)][xMid + sectorBuff[blockCount][1]] = weight;
                            memBUff[yMid + (int)(sectorBuff[blockCount][0]*multi)][xMid + sectorBuff[blockCount][1] - 1] = weight;
                            break;
                        case 3:
                            memBUff[yMid + (int)(sectorBuff[sectorCount-blockCount-1][1]*multi)][xMid + sectorBuff[sectorCount-blockCount-1][0]] = weight;
                            memBUff[yMid + (int)(sectorBuff[sectorCount-blockCount-1][1]*multi) - 1][xMid + sectorBuff[sectorCount-blockCount-1][0]] = weight;
                            break;
                        case 4:
                            memBUff[yMid + (int)(sectorBuff[blockCount][1]*multi)][xMid - sectorBuff[blockCount][0]] = weight;
                            memBUff[yMid + (int)(sectorBuff[blockCount][1]*multi) - 1][xMid - sectorBuff[blockCount][0]] = weight;
                            break;
                        case 5:
                            memBUff[yMid + (int)(sectorBuff[sectorCount-blockCount-1][0]*multi)][xMid - sectorBuff[sectorCount-blockCount-1][1]] = weight;
                            memBUff[yMid + (int)(sectorBuff[sectorCount-blockCount-1][0]*multi)][xMid - sectorBuff[sectorCount-blockCount-1][1] + 1] = weight;
                            break;
                        case 6:
                            memBUff[yMid - (int)(sectorBuff[blockCount][0]*multi)][xMid - sectorBuff[blockCount][1]] = weight;
                            memBUff[yMid - (int)(sectorBuff[blockCount][0]*multi)][xMid - sectorBuff[blockCount][1] + 1] = weight;
                            break;
                        case 7:
                            memBUff[yMid - (int)(sectorBuff[sectorCount-blockCount-1][1]*multi)][xMid - sectorBuff[sectorCount-blockCount-1][0]] = weight;
                            memBUff[yMid - (int)(sectorBuff[sectorCount-blockCount-1][1]*multi) + 1][xMid - sectorBuff[sectorCount-blockCount-1][0]] = weight;
                            break;
                    }
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
                if(mode)//缩放X轴
                {
                    //1
                    memBUff[yMid - circle_b][xMid + (int)(circle_a*multi)] = weight;
                    memBUff[yMid - circle_b + 1][xMid + (int)(circle_a*multi)] = weight;
                    //2
                    memBUff[yMid - circle_a][xMid + (int)(circle_b*multi)] = weight;
                    memBUff[yMid - circle_a][xMid + (int)(circle_b*multi) - 1] = weight;
                    //3
                    memBUff[yMid + circle_a][xMid + (int)(circle_b*multi)] = weight;
                    memBUff[yMid + circle_a][xMid + (int)(circle_b*multi) - 1] = weight;
                    //4
                    memBUff[yMid + circle_b][xMid + (int)(circle_a*multi)] = weight;
                    memBUff[yMid + circle_b - 1][xMid + (int)(circle_a*multi)] = weight;
                    //5
                    memBUff[yMid + circle_b][xMid - (int)(circle_a*multi)] = weight;
                    memBUff[yMid + circle_b - 1][xMid - (int)(circle_a*multi)] = weight;
                    //6
                    memBUff[yMid + circle_a][xMid - (int)(circle_b*multi)] = weight;
                    memBUff[yMid + circle_a][xMid - (int)(circle_b*multi) + 1] = weight;
                    //7
                    memBUff[yMid - circle_a][xMid - (int)(circle_b*multi)] = weight;
                    memBUff[yMid - circle_a][xMid - (int)(circle_b*multi) + 1] = weight;
                    //8
                    memBUff[yMid - circle_b][xMid - (int)(circle_a*multi)] = weight;
                    memBUff[yMid - circle_b + 1][xMid - (int)(circle_a*multi)] = weight;
                }
                else//缩放Y轴
                {
                    //1
                    memBUff[yMid - (int)(circle_b*multi)][xMid + circle_a] = weight;
                    memBUff[yMid - (int)(circle_b*multi) + 1][xMid + circle_a] = weight;
                    //2
                    memBUff[yMid - (int)(circle_a*multi)][xMid + circle_b] = weight;
                    memBUff[yMid - (int)(circle_a*multi)][xMid + circle_b - 1] = weight;
                    //3
                    memBUff[yMid + (int)(circle_a*multi)][xMid + circle_b] = weight;
                    memBUff[yMid + (int)(circle_a*multi)][xMid + circle_b - 1] = weight;
                    //4
                    memBUff[yMid + (int)(circle_b*multi)][xMid + circle_a] = weight;
                    memBUff[yMid + (int)(circle_b*multi) - 1][xMid + circle_a] = weight;
                    //5
                    memBUff[yMid + (int)(circle_b*multi)][xMid - circle_a] = weight;
                    memBUff[yMid + (int)(circle_b*multi) - 1][xMid - circle_a] = weight;
                    //6
                    memBUff[yMid + (int)(circle_a*multi)][xMid - circle_b] = weight;
                    memBUff[yMid + (int)(circle_a*multi)][xMid - circle_b + 1] = weight;
                    //7
                    memBUff[yMid - (int)(circle_a*multi)][xMid - circle_b] = weight;
                    memBUff[yMid - (int)(circle_a*multi)][xMid - circle_b + 1] = weight;
                    //8
                    memBUff[yMid - (int)(circle_b*multi)][xMid - circle_a] = weight;
                    memBUff[yMid - (int)(circle_b*multi) + 1][xMid - circle_a] = weight;
                }
                
                //
                circle_a++;
                //使用Bresenham算法画圆
                if(circle_di < 0)
                    circle_di += 4*circle_a + 6;
                else{
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

unsigned char* _circle(int rad, int rad2, int angle, int degree, unsigned char weight, unsigned char *grid, int *gw, int *gh, int *xyCentre = NULL)
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
        }
        if(gh && (*gh < xSize || *gh < ySize)){
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
        //
        if(xyCentre){
            xMid = xyCentre[0];
            yMid = xyCentre[1];
        }
    }
    else
        mem = new unsigned char[xSize*ySize];
    //
    memBUff = new unsigned char*[ySize];
    for(int i = 0, j = 0; i < ySize; i++){
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
                else{
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

//递归填充 注意: xMax, yMax 是x,y可以到达的最大值
inline void _recursion_u(unsigned char *map[], int x, int y, int xMax, int yMax, unsigned char tag);
inline void _recursion_d(unsigned char *map[], int x, int y, int xMax, int yMax, unsigned char tag);
inline void _recursion_l(unsigned char *map[], int x, int y, int xMax, int yMax, unsigned char tag);
inline void _recursion_r(unsigned char *map[], int x, int y, int xMax, int yMax, unsigned char tag);
inline void _recursion_u(unsigned char *map[], int x, int y, int xMax, int yMax, unsigned char tag)
{
    if(map[y][x] != tag)
        return;
    map[y][x] = 0;
    if(y > 0) _recursion_u(map, x, y-1, xMax, yMax, tag);
    // if(y < yMax) _recursion_d(map, x, y+1, xMax, yMax, tag);
    if(x > 0) _recursion_l(map, x-1, y, xMax, yMax, tag);
    if(x < xMax) _recursion_r(map, x+1, y, xMax, yMax, tag);
}
inline void _recursion_d(unsigned char *map[], int x, int y, int xMax, int yMax, unsigned char tag)
{
    if(map[y][x] != tag)
        return;
    map[y][x] = 0;
    // if(y > 0) _recursion_u(map, x, y-1, xMax, yMax, tag);
    if(y < yMax) _recursion_d(map, x, y+1, xMax, yMax, tag);
    if(x > 0) _recursion_l(map, x-1, y, xMax, yMax, tag);
    if(x < xMax) _recursion_r(map, x+1, y, xMax, yMax, tag);
}
inline void _recursion_l(unsigned char *map[], int x, int y, int xMax, int yMax, unsigned char tag)
{
    if(map[y][x] != tag)
        return;
    map[y][x] = 0;
    if(y > 0) _recursion_u(map, x, y-1, xMax, yMax, tag);
    if(y < yMax) _recursion_d(map, x, y+1, xMax, yMax, tag);
    if(x > 0) _recursion_l(map, x-1, y, xMax, yMax, tag);
    // if(x < xMax) _recursion_r(map, x+1, y, xMax, yMax, tag);
}
inline void _recursion_r(unsigned char *map[], int x, int y, int xMax, int yMax, unsigned char tag)
{
    if(map[y][x] != tag)
        return;
    map[y][x] = 0;
    if(y > 0) _recursion_u(map, x, y-1, xMax, yMax, tag);
    if(y < yMax) _recursion_d(map, x, y+1, xMax, yMax, tag);
    // if(x > 0) _recursion_l(map, x-1, y, xMax, yMax, tag);
    if(x < xMax) _recursion_r(map, x+1, y, xMax, yMax, tag);
}
void recursion(unsigned char *map[], int x, int y, int xMax, int yMax, unsigned char tag)
{
    if(map[y][x] != tag)
        return;
    map[y][x] = 0;
    if(y > 0) _recursion_u(map, x, y-1, xMax, yMax, tag);
    if(y < yMax) _recursion_d(map, x, y+1, xMax, yMax, tag);
    if(x > 0) _recursion_l(map, x-1, y, xMax, yMax, tag);
    if(x < xMax) _recursion_r(map, x+1, y, xMax, yMax, tag);
}

//-------------------- 接口封装 --------------------

Polygon::Polygon(int line = 0)
{
    if(line < 3)//圆
    {
        LINE = 0;
        DOT_LEN = 0;
        DOT = 0;
        WIDTH = 1;
        HEIGHT = 1;
        return;
    }
    //
    LINE = line;
    DOT_LEN = LINE*2;
    DOT = new int[DOT_LEN];
    //多边形
    int radius = 100;
    double degreeCount, degreeStart;//按角度旋转 找到各定点坐标
    int maxX = 0, maxY = 0, minX = 0, minY = 0;
    //
    if(LINE%2)//单数
        degreeStart = VS_PI/2;
    else//双数
        degreeStart = VS_PI/2 - VS_PI*2/LINE/2;
    //
    int i, j, cc, half = LINE/2 + LINE%2;
    //计算右半边
    for(i = cc = 0; cc < half; cc++)
    {
        degreeCount = degreeStart - VS_PI*2*cc/LINE;
        //计算X,Y坐标
        DOT[i] = abs(round(radius*cos(degreeCount)));
        //
        if(DOT[i] > maxX){
            maxX = DOT[i];
            minX = -maxX;
        }
        //
        i += 1;
        //
        DOT[i] = round(radius*sin(degreeCount));
        //
        if(DOT[i] < minY)
            minY = DOT[i];
        else if(DOT[i] > maxY)
            maxY = DOT[i];
        //
        i += 1;
        //根据象限修正符号
        // printf("R: X/%d, Y/%d\n", DOT[i-2], DOT[i-1]);
    }
    //轴对称
    for(j = i; cc < LINE; cc++)
    {
        DOT[i++] = -DOT[j-2];
        DOT[i++] = DOT[j-1];
        j -= 2;
        // printf("L: X/%d, Y/%d\n", DOT[i-2], DOT[i-1]);
    }
    //坐标归正
    for(int i = 0; i < DOT_LEN;)
    {
        DOT[i] -= minX;//x坐标右推至正值范围
        i++;
        DOT[i] = -(DOT[i] - maxY);//y坐标上推至正值范围 然后大小翻转
        i++;
        // printf("X/%d, Y/%d\n", DOT[i-2], DOT[i-1]);
    }
    //
    WIDTH = maxX - minX + 1;
    HEIGHT = maxY - minY + 1;
    //
    // printf("init: line/%d, width/%d, height/%d, dot_len/%d [%d-%d %d-%d]\n", 
    //     LINE, WIDTH, HEIGHT, DOT_LEN, minX, maxX, minY, maxY);
}

Polygon::Polygon(int line, int *xy)
{
    if(line < 3 || !xy)//圆
    {
        LINE = 0;
        DOT_LEN = 0;
        DOT = 0;
        WIDTH = 1;
        HEIGHT = 1;
        return;
    }
    //
    LINE = line;
    DOT_LEN = LINE*2;
    DOT = new int[DOT_LEN];
    //
    int maxX = xy[0], maxY = xy[1], minX = xy[0], minY = xy[1];
    //
    for(int i = 0; i < DOT_LEN;)
    {
        DOT[i] = xy[i];
        //
        if(DOT[i] < minX)
            minX = DOT[i];
        else if(DOT[i] > maxX)
            maxX = DOT[i];
        //
        i += 1;
        //
        DOT[i] = xy[i];
        //
        if(DOT[i] < minY)
            minY = DOT[i];
        else if(DOT[i] > maxY)
            maxY = DOT[i];
        //
        i += 1;
        // printf("A: X/%d, Y/%d\n", DOT[i-2], DOT[i-1]);
    }
    //多余裁掉
    if(minX > 0){
        for(int i = 0; i < DOT_LEN; i+=2)
            DOT[i] -= minX;
        maxX -= minX;
        minX = 0;
    }
    if(minY > 0){
        for(int i = 1; i < DOT_LEN; i+=2)
            DOT[i] -= minY;
        maxY -= minY;
        minY = 0;
    }
    //坐标归正
    for(int i = 0; i < DOT_LEN;)
    {
        DOT[i] -= minX;//x坐标右推至正值范围
        i++;
        DOT[i] = -(DOT[i] - maxY);//大小翻转
        i++;
        // printf("X/%d, Y/%d\n", DOT[i-2], DOT[i-1]);
    }
    //
    WIDTH = maxX - minX + 1;
    HEIGHT = maxY - minY + 1;
    //
    // printf("init: line/%d, width/%d, height/%d, dot_len/%d [%d-%d %d-%d]\n", 
    //     LINE, WIDTH, HEIGHT, DOT_LEN, minX, maxX, minY, maxY);
}

Polygon::~Polygon()
{
    if(DOT)
        delete[] DOT;
}

unsigned char* Polygon::get_circle(int rad, int rad2, int angle, int degree, unsigned char weight, unsigned char *grid, int *gw, int *gh, int *xyCentre = NULL)
{
    return _circle(rad, rad2, angle, degree, weight, grid, gw, gh, xyCentre);
}

unsigned char* Polygon::get_ellipse(int radX, int radY, int lineSize, int angle, int degree, unsigned char weight, unsigned char *grid, int *gw, int *gh, int *xyCentre = NULL)
{
    return _ellipse(radX, radY, lineSize, angle, degree, weight, grid, gw, gh, xyCentre);
}

unsigned char* Polygon::get_polygon(int w, int h, int lineSize, unsigned char weight)
{
    if(DOT_LEN == 0 || w < 1 || h < 1)
        return NULL;
    //
    int W = w, H = h;
    int memSize = W*H;
    unsigned char *mem = NULL;
    unsigned char **memBUff = NULL;
    int *dot = NULL, *dotP;
    //
    if(lineSize > W || lineSize > H)
        return NULL;
    //grid初始化
    mem = new unsigned char[memSize];
    memBUff = new unsigned char*[H];
    for(int i = 0, j = 0; i < H; i++){
        memBUff[i] = &mem[j];
        j += W;
    }
    dot = new int[DOT_LEN];
    //----- 画线 -----
    if(lineSize > 0)
    {
        //缩放端点坐标
        double zoomX = (double)(W-lineSize*2)/WIDTH;
        double zoomY = (double)(H-lineSize*2)/HEIGHT;
        for(int i = 0, HW = WIDTH/2, HH = HEIGHT/2; i < DOT_LEN;){
            if(DOT[i] > HW)
                dot[i] = W - 1 - (WIDTH-DOT[i]-1)*zoomX - lineSize;//过半部分 缩放
            else
                dot[i] = DOT[i]*zoomX + lineSize;//未过半部分 缩放
            i += 1;
            if(DOT[i] > HH)
                dot[i] = H - 1 - (HEIGHT-DOT[i]-1)*zoomY - lineSize;//过半部分 缩放
            else
                dot[i] = DOT[i]*zoomY + lineSize;//未过半部分 缩放
            i += 1;
            // printf("w/%d/%.2lf, h/%d/%.2lf  x/%d, y/%d\n", W, zoomX, H, zoomY, dot[i-2], dot[i-1]);
        }
        //在栅格图上连线
        int xLoad[W+H+1], yLoad[W+H+1], loadCount;
        dotP = &dot[DOT_LEN-2];
        for(int i = 0; i < DOT_LEN; i+=2){
            //获取直线坐标
            loadCount = _getDotFromLine(dotP[0], dotP[1], dot[i], dot[i+1], xLoad, yLoad);
            dotP = &dot[i];
            //画点
            for(int j = 0; j < loadCount; j++)
                memBUff[yLoad[j]][xLoad[j]] = weight;
        }
    }
    //----- 填充 -----
    else
    {
        memset(mem, weight, memSize);
        //缩放端点坐标
        double zoomX = (double)W/WIDTH;
        double zoomY = (double)H/HEIGHT;
        for(int i = 0, HW = WIDTH/2, HH = HEIGHT/2; i < DOT_LEN;){
            if(DOT[i] > HW)
                dot[i] = W - 1 - (WIDTH-DOT[i]-1)*zoomX;//过半部分 缩放
            else
                dot[i] = DOT[i]*zoomX;//未过半部分 缩放
            i += 1;
            if(DOT[i] > HH)
                dot[i] = H - 1 - (HEIGHT-DOT[i]-1)*zoomY;//过半部分 缩放
            else
                dot[i] = DOT[i]*zoomY;//未过半部分 缩放
            i += 1;
            // printf("w/%d/%.2lf, h/%d/%.2lf  x/%d, y/%d\n", W, zoomX, H, zoomY, dot[i-2], dot[i-1]);
        }
        //在栅格图上连线
        int xLoad[W+H+1], yLoad[W+H+1], loadCount;
        dotP = &dot[DOT_LEN-2];
        for(int i = 0; i < DOT_LEN; i+=2){
            //获取直线坐标
            loadCount = _getDotFromLine(dotP[0], dotP[1], dot[i], dot[i+1], xLoad, yLoad);
            dotP = &dot[i];
            //画点
            for(int j = 0; j < loadCount; j++)
                memBUff[yLoad[j]][xLoad[j]] = weight-1;
        }
        //填充
        for(int i = 0, j; i < H; i++){
            for(j = 0; j < W; j++){
                if(memBUff[i][j] == weight)
                    memBUff[i][j] = 0;
                else
                    break;
            }
            for(j = W-1; j >= 0; j--){
                if(memBUff[i][j] == weight)
                    memBUff[i][j] = 0;
                else
                    break;
            }
        }
        for(int i = 0, j; i < W; i++){
            for(j = 0; j < H; j++){
                if(memBUff[j][i] == weight)
                    memBUff[j][i] = 0;
                else
                    break;
            }
            for(j = H-1; j >= 0; j--){
                if(memBUff[j][i] == weight)
                    memBUff[j][i] = 0;
                else
                    break;
            }
        }
    }

    //
    delete[] memBUff;
    delete[] dot;
    //
    return mem;
}

void _recursion_part(unsigned char **memBUff, int w, int h, int divH, unsigned char weight)
{
    unsigned char **mp;
    int cY, cX, check = 1;
    int temp;
    //
    recursion(memBUff, 0, 0, w, divH-1, weight);
    while(check)
    {
        //由上至下
        for(cY = divH, check = 0; cY < h; cY+= divH)
        {
            mp = memBUff+cY;
            //
            if(cY + divH < h)
                temp = divH - 1;
            else
                temp = h - cY - 1;
            //
            for(cX = 0; cX < w; cX++)
            {
                if(memBUff[cY][cX] == weight && memBUff[cY-1][cX] == 0)
                    recursion(mp, cX, 0, w-1, temp, weight);
                else if(memBUff[cY][cX] == 0 && memBUff[cY-1][cX] == weight)
                    check += 1;
            }
        }
        //由下至上
        if(check)
        {
            for(cY = cY - divH, check = 0; cY > 0; cY-=divH)
            {
                if(cY == divH)
                    temp = 0;
                else
                    temp = divH;
                //
                mp = memBUff+cY-temp;
                //
                for(cX = 0; cX < w; cX++)
                {
                    if(memBUff[cY][cX] == 0 && memBUff[cY-1][cX] == weight)
                        recursion(mp, cX, temp-1, w-1, divH, weight);
                    else if(memBUff[cY][cX] == weight && memBUff[cY-1][cX] == 0)
                        check += 1;
                }
            }
        }
    }
}

unsigned char* Polygon::get_polygon2(int w, int h, unsigned char weight)
{
    if(DOT_LEN == 0 || w < 1 || h < 1)
        return NULL;
    //
    int W = w, H = h;
    int memSize = W*H;
    unsigned char *mem = NULL;
    unsigned char **memBUff = NULL;
    int *dot = NULL, *dotP;
    //grid初始化
    memBUff = new unsigned char*[H+2];
    for(int i = 0; i < H+2; i++){
        memBUff[i] = new unsigned char[W+2];
        memset(memBUff[i], weight, W+2);
    }
    //
    dot = new int[DOT_LEN];
    //缩放端点坐标
    double zoomX = (double)W/WIDTH;
    double zoomY = (double)H/HEIGHT;
    for(int i = 0, HW = WIDTH/2, HH = HEIGHT/2; i < DOT_LEN;){
        if(DOT[i] > HW)
            dot[i] = W - (WIDTH-DOT[i]-1)*zoomX;//过半部分 缩放
        else
            dot[i] = DOT[i]*zoomX + 1;//未过半部分 缩放
        i += 1;
        if(DOT[i] > HH)
            dot[i] = H - (HEIGHT-DOT[i]-1)*zoomY;//过半部分 缩放
        else
            dot[i] = DOT[i]*zoomY + 1;//未过半部分 缩放
        i += 1;
        // printf("w/%d/%.2lf, h/%d/%.2lf  x/%d, y/%d\n", W, zoomX, H, zoomY, dot[i-2], dot[i-1]);
    }
    //在栅格图上连线
    int xLoad[W+H+1], yLoad[W+H+1], loadCount;
    dotP = &dot[DOT_LEN-2];
    for(int i = 0; i < DOT_LEN; i+=2){
        //获取直线坐标
        loadCount = _getDotFromLine(dotP[0], dotP[1], dot[i], dot[i+1], xLoad, yLoad);
        dotP = &dot[i];
        //画点
        for(int j = 0; j < loadCount; j++)
            memBUff[yLoad[j]][xLoad[j]] = weight-1;
    }
    //
#define recursion_max 20000
    //分块递归填充
    if(memSize > recursion_max)
    {
        if(recursion_max > W+2)
            _recursion_part(memBUff, W+2, H+2, recursion_max/(W+2), weight);
        else
            _recursion_part(memBUff, W+2, H+2, 1, weight);
    }
    //递归填充
    else
        recursion(memBUff, 0, 0, W+1, H+1, weight);
    //
    mem = new unsigned char[W*H];
    int ccp = 0;
    delete[] memBUff[ccp++];
    for(int i = 0; i < memSize;){
        memcpy(&mem[i], memBUff[ccp], W);
        delete[] memBUff[ccp++];
        i += W;
    }
    delete[] memBUff[ccp];
    //
    delete[] memBUff;
    delete[] dot;
    //
    return mem;
}


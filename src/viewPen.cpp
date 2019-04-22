#include "viewPen.h"

//用完记得 delete[]
unsigned char* _intRGB_to_charRGB(int color)
{
    unsigned char *ret = new unsigned char[3];
    ret[0] = (unsigned char)((color&0xFF0000)>>16);
    ret[1] = (unsigned char)((color&0x00FF00)>>8);
    ret[2] = (unsigned char)(color&0x0000FF);
    return ret;
}

ViewPen::ViewPen(int displayDir = 0):
    XSTART(VIEW_X_START),
    XEND(VIEW_X_END),
    YSTART(VIEW_Y_START),
    YEND(VIEW_Y_END),
    WSIZE(VIEW_PERW){
    AS = amoled_malloc(displayDir, VIEW_X_SIZE, VIEW_Y_SIZE, VIEW_PERW);
}

ViewPen::~ViewPen()
{
    amoled_free(AS);
}

void ViewPen::set_dir(int displayDir = 0)
{
    amoled_displayDir(displayDir);
}

void ViewPen::set_mode(ViewPen_Mode mode)
{
    amoled_modeSet(mode);
}

void ViewPen::set_bright(unsigned char bright)
{
    amoled_brightSet(bright);
}

void ViewPen::output()
{
    amoled_print_en();
}

void ViewPen::clear(int color = 0)
{
    amoled_print_clear(color);
}

void ViewPen::print(int x, int y, int color)
{
    amoled_print_dot(x, y, color);
}

void ViewPen::print(int x, int y, int color, float alpha)
{
    amoled_print_dot2(x, y, color, alpha);
}

//范围控制
// xS,yS 为传入的数组坐标计数
// mxS,myS 为屏幕坐标计数
// xL,yL 为实际在X轴,Y轴的绘制长度
#define XY_RAIL()   \
int xS, yS, mxS, myS, xL, yL;\
if(xStart < XSTART){\
    mxS = XSTART;\
    xS = XSTART - xStart;\
}else{\
    mxS = xStart;\
    xS = 0;\
}if(yStart < YSTART){\
    myS = YSTART;\
    yS = YSTART - yStart;\
}else{\
    myS = yStart;\
    yS = 0;\
}if(xSize - xS + mxS - 2 < XEND)\
    xL = xSize;\
else\
    xL = XEND - mxS + 1;\
if(ySize - yS + myS - 2 < YEND)\
    yL = ySize;\
else\
    yL = YEND - myS + 1;\

void ViewPen::print_rgb(unsigned char *rgb, int xStart, int yStart, int xSize, int ySize, float alpha = 0)
{
    if(!rgb || alpha >= 1 ||
        xStart > XEND || yStart > YEND ||
        xStart+xSize <= XSTART || yStart+ySize <= YSTART)
        return;
    //得到 xS, yS, mxS, myS, xL, yL
    XY_RAIL();
    //
    unsigned char *line;
    int i, j, cc, mxS2, cc2, cc2p = xSize*3;
    cc2 = yS*cc2p + xS*3;
    //
    if(alpha == 0)
    {
        for(i = 0; i < yL; i++, myS++)
        {
            line = &rgb[cc2];
            for(j = 0, mxS2 = mxS; j < xL; j++, mxS2++)
            {
                cc = j*3;
#if(AMOLED_RGB_MODE == 1)
                AS->data.backupMap[myS][mxS2][2] = line[cc];
                AS->data.backupMap[myS][mxS2][1] = line[cc+1];
                AS->data.backupMap[myS][mxS2][0] = line[cc+2];
#else
                AS->data.backupMap[myS][mxS2][0] = line[cc];
                AS->data.backupMap[myS][mxS2][1] = line[cc+1];
                AS->data.backupMap[myS][mxS2][2] = line[cc+2];
#endif
            }
            cc2 += cc2p;
        }
    }
    else
    {
        float alpha2 = 1-alpha;
        for(i = 0; i < yL; i++, myS++)
        {
            line = &rgb[cc2];
            for(j = 0, mxS2 = mxS; j < xL; j++, mxS2++)
            {
                cc = j*3;
#if(AMOLED_RGB_MODE == 1)
                AS->data.backupMap[myS][mxS2][2] = 
                    AS->data.backupMap[myS][mxS2][2]*alpha + line[cc]*alpha2;
                AS->data.backupMap[myS][mxS2][1] = 
                    AS->data.backupMap[myS][mxS2][1]*alpha + line[cc+1]*alpha2;
                AS->data.backupMap[myS][mxS2][0] = 
                    AS->data.backupMap[myS][mxS2][0]*alpha + line[cc+2]*alpha2;
#else
                AS->data.backupMap[myS][mxS2][0] = 
                    AS->data.backupMap[myS][mxS2][0]*alpha + line[cc]*alpha2;
                AS->data.backupMap[myS][mxS2][1] = 
                    AS->data.backupMap[myS][mxS2][1]*alpha + line[cc+1]*alpha2;
                AS->data.backupMap[myS][mxS2][2] = 
                    AS->data.backupMap[myS][mxS2][2]*alpha + line[cc+2]*alpha2;
#endif
            }
            cc2 += cc2p;
        }
    }
}

void ViewPen::print_map(unsigned char ***map, int xStart, int yStart, int xSize, int ySize, float alpha = 0)
{
    if(!map || alpha >= 1 ||
        xStart > XEND || yStart > YEND ||
        xStart+xSize <= XSTART || yStart+ySize <= YSTART)
        return;
    //得到 xS, yS, mxS, myS, xL, yL
    XY_RAIL();
    //
    int xC, yC, mxC, myC;
    int xCEnd = xS + xL, yCEnd = yS + yL; 
    //
    if(alpha == 0)
    {
        for(yC = yS, myC = myS; yC < yCEnd; yC++, myC++)
        {
            for(xC = xS, mxC = mxS; xC < xCEnd; xC++, mxC++)
            {
                if(map[yC][xC])
                {
#if(AMOLED_RGB_MODE == 1)
                    AS->data.backupMap[myC][mxC][2] = map[yC][xC][0];
                    AS->data.backupMap[myC][mxC][1] = map[yC][xC][1];
                    AS->data.backupMap[myC][mxC][0] = map[yC][xC][2];
#else
                    AS->data.backupMap[myC][mxC][0] = map[yC][xC][0];
                    AS->data.backupMap[myC][mxC][1] = map[yC][xC][1];
                    AS->data.backupMap[myC][mxC][2] = map[yC][xC][2];
#endif
                }
            }
        }
    }
    else
    {
        float alpha2 = 1-alpha;
        for(yC = yS, myC = myS; yC < yCEnd; yC++, myC++)
        {
            for(xC = xS, mxC = mxS; xC < xCEnd; xC++, mxC++)
            {
                if(map[yC][xC])
                {
#if(AMOLED_RGB_MODE == 1)
                    AS->data.backupMap[myC][mxC][2] = 
                        AS->data.backupMap[myC][mxC][2]*alpha + map[yC][xC][0]*alpha2;
                    AS->data.backupMap[myC][mxC][1] = 
                        AS->data.backupMap[myC][mxC][1]*alpha + map[yC][xC][1]*alpha2;
                    AS->data.backupMap[myC][mxC][0] = 
                        AS->data.backupMap[myC][mxC][0]*alpha + map[yC][xC][2]*alpha2;
#else
                    AS->data.backupMap[myC][mxC][0] = 
                        AS->data.backupMap[myC][mxC][0]*alpha + map[yC][xC][0]*alpha2;
                    AS->data.backupMap[myC][mxC][1] = 
                        AS->data.backupMap[myC][mxC][1]*alpha + map[yC][xC][1]*alpha2;
                    AS->data.backupMap[myC][mxC][2] = 
                        AS->data.backupMap[myC][mxC][2]*alpha + map[yC][xC][2]*alpha2;
#endif
                }
            }
        }
    }
}

void ViewPen::print_grid(unsigned char *grid, int color, int xStart, int yStart, int xSize, int ySize, float alpha = 0)
{
    if(!grid || alpha >= 1 ||
        xStart > XEND || yStart > YEND ||
        xStart+xSize <= XSTART || yStart+ySize <= YSTART)
        return;
    //得到 xS, yS, mxS, myS, xL, yL
    XY_RAIL();
    //
    unsigned char *col = _intRGB_to_charRGB(color);
    int yC, mxC, myC;
    int yCEnd = yS + yL;
    int i, cc = yS*xSize + xS;
    unsigned char *line;
    //
    if(alpha == 0)
    {
        for(yC = yS, myC = myS; yC < yCEnd; yC++, myC++)
        {
            line = &grid[cc];
            for(i = 0, mxC = mxS; i < xL; i++, mxC++)
            {
                if(line[i])
                {
#if(AMOLED_RGB_MODE == 1)
                    AS->data.backupMap[myC][mxC][2] = col[0];
                    AS->data.backupMap[myC][mxC][1] = col[1];
                    AS->data.backupMap[myC][mxC][0] = col[2];
#else
                    AS->data.backupMap[myC][mxC][0] = col[0];
                    AS->data.backupMap[myC][mxC][1] = col[1];
                    AS->data.backupMap[myC][mxC][2] = col[2];
#endif
                }
            }
            cc += xSize;
        }
    }
    else
    {
        float alpha2 = 1-alpha;
        for(yC = yS, myC = myS; yC < yCEnd; yC++, myC++)
        {
            line = &grid[cc];
            for(i = 0, mxC = mxS; i < xL; i++, mxC++)
            {
                if(line[i])
                {
#if(AMOLED_RGB_MODE == 1)
                    AS->data.backupMap[myC][mxC][2] = 
                        AS->data.backupMap[myC][mxC][2]*alpha + col[0]*alpha2;
                    AS->data.backupMap[myC][mxC][1] = 
                        AS->data.backupMap[myC][mxC][1]*alpha + col[1]*alpha2;
                    AS->data.backupMap[myC][mxC][0] = 
                        AS->data.backupMap[myC][mxC][0]*alpha + col[2]*alpha2;
#else
                    AS->data.backupMap[myC][mxC][0] = 
                        AS->data.backupMap[myC][mxC][0]*alpha + col[0]*alpha2;
                    AS->data.backupMap[myC][mxC][1] = 
                        AS->data.backupMap[myC][mxC][1]*alpha + col[1]*alpha2;
                    AS->data.backupMap[myC][mxC][2] = 
                        AS->data.backupMap[myC][mxC][2]*alpha + col[2]*alpha2;
#endif
                }
            }
            cc += xSize;
        }
    }
    //
    delete[] col;
}

void ViewPen::print_grid2(unsigned char *grid, int color, int xStart, int yStart, int xSize, int ySize, float alpha = 0)
{
    if(!grid || alpha >= 1 ||
        xStart > XEND || yStart > YEND ||
        xStart+xSize <= XSTART || yStart+ySize <= YSTART)
        return;
    //得到 xS, yS, mxS, myS, xL, yL
    XY_RAIL();
    //
    unsigned char *col = _intRGB_to_charRGB(color);
    int yC, mxC, myC;
    int yCEnd = yS + yL;
    int i, cc = yS*xSize + xS;
    unsigned char *line;
    //
    float weight;
    unsigned char colBak[3] = {0};
    int result, j;
    //
    if(alpha == 0)
    {
        for(yC = yS, myC = myS; yC < yCEnd; yC++, myC++)
        {
            line = &grid[cc];
            for(i = 0, mxC = mxS; i < xL; i++, mxC++)
            {
                if(!line[i])
                    continue;
                else if(line[i] == 255)
                {
#if(AMOLED_RGB_MODE == 1)
                    AS->data.backupMap[myC][mxC][2] = col[0];
                    AS->data.backupMap[myC][mxC][1] = col[1];
                    AS->data.backupMap[myC][mxC][0] = col[2];
#else
                    AS->data.backupMap[myC][mxC][0] = col[0];
                    AS->data.backupMap[myC][mxC][1] = col[1];
                    AS->data.backupMap[myC][mxC][2] = col[2];
#endif
                }
                else
                {
                    weight = (float)line[i]/255;
                    for(j = 0; j < 3; j++)
                    {
                        result = col[j]*weight;
                        if(result > 0xFF)
                            colBak[j] = 0xFF;
                        else if(result < 0)
                            colBak[j] = 0;
                        else
                            colBak[j] = result;
                    }
#if(AMOLED_RGB_MODE == 1)
                    AS->data.backupMap[myC][mxC][2] = colBak[0];
                    AS->data.backupMap[myC][mxC][1] = colBak[1];
                    AS->data.backupMap[myC][mxC][0] = colBak[2];
#else
                    AS->data.backupMap[myC][mxC][0] = colBak[0];
                    AS->data.backupMap[myC][mxC][1] = colBak[1];
                    AS->data.backupMap[myC][mxC][2] = colBak[2];
#endif
                }
            }
            cc += xSize;
        }
    }
    else
    {
        float alpha2 = 1-alpha;
        for(yC = yS, myC = myS; yC < yCEnd; yC++, myC++)
        {
            line = &grid[cc];
            for(i = 0, mxC = mxS; i < xL; i++, mxC++)
            {
                if(!line[i])
                    continue;
                else if(line[i] == 255)
                {
#if(AMOLED_RGB_MODE == 1)
                    AS->data.backupMap[myC][mxC][2] = 
                        AS->data.backupMap[myC][mxC][2]*alpha + col[0]*alpha2;
                    AS->data.backupMap[myC][mxC][1] = 
                        AS->data.backupMap[myC][mxC][1]*alpha + col[1]*alpha2;
                    AS->data.backupMap[myC][mxC][0] = 
                        AS->data.backupMap[myC][mxC][0]*alpha + col[2]*alpha2;
#else
                    AS->data.backupMap[myC][mxC][0] = 
                        AS->data.backupMap[myC][mxC][0]*alpha + col[0]*alpha2;
                    AS->data.backupMap[myC][mxC][1] = 
                        AS->data.backupMap[myC][mxC][1]*alpha + col[1]*alpha2;
                    AS->data.backupMap[myC][mxC][2] = 
                        AS->data.backupMap[myC][mxC][2]*alpha + col[2]*alpha2;
#endif
                }
                else
                {
                    weight = (float)line[i]/255;
                    for(j = 0; j < 3; j++)
                    {
                        result = col[j]*weight;
                        if(result > 0xFF)
                            colBak[j] = 0xFF;
                        else if(result < 0)
                            colBak[j] = 0;
                        else
                            colBak[j] = result;
                    }
#if(AMOLED_RGB_MODE == 1)
                    AS->data.backupMap[myC][mxC][2] = 
                        AS->data.backupMap[myC][mxC][2]*alpha + colBak[0]*alpha2;
                    AS->data.backupMap[myC][mxC][1] = 
                        AS->data.backupMap[myC][mxC][1]*alpha + colBak[1]*alpha2;
                    AS->data.backupMap[myC][mxC][0] = 
                        AS->data.backupMap[myC][mxC][0]*alpha + colBak[2]*alpha2;
#else
                    AS->data.backupMap[myC][mxC][0] = 
                        AS->data.backupMap[myC][mxC][0]*alpha + colBak[0]*alpha2;
                    AS->data.backupMap[myC][mxC][1] = 
                        AS->data.backupMap[myC][mxC][1]*alpha + colBak[1]*alpha2;
                    AS->data.backupMap[myC][mxC][2] = 
                        AS->data.backupMap[myC][mxC][2]*alpha + colBak[2]*alpha2;
#endif
                }
            }
            cc += xSize;
        }
    }
    //
    delete[] col;
}

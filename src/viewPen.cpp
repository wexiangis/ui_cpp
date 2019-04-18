#include <stdlib.h>
#include <string.h>
#include "viewPen.h"

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

void ViewPen::refresh()
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

void ViewPen::print_map(int xStart, int yStart, int xSize, int ySize, unsigned char *rgb, float alpha = 0)
{
    if(!rgb || alpha == 1 ||
        xStart > XEND || yStart > YEND ||
        xStart+xSize <= XSTART || yStart+ySize <= YSTART)
        return;
    //
    int xS, yS, mxS, myS;
    int xL, yL;
    //
    if(xStart < XSTART)
    {
        mxS = XSTART;
        xS = XSTART - xStart;
    }else
    {
        mxS = xStart;
        xS = 0;
    }
    if(yStart < YSTART)
    {
        myS = YSTART;
        yS = YSTART - yStart;
    }else
    {
        myS = yStart;
        yS = 0;
    }
    //
    if(xSize - xS + mxS - 2 < XEND)
        xL = xSize;
    else
        xL = XEND - mxS + 1;
    if(ySize - yS + myS - 2 < YEND)
        yL = ySize;
    else
        yL = YEND - myS + 1;
    //
    if(alpha == 0)
    {
        for(int i = 0; i < yL; i++)
            memcpy(AS->data.backupMap[myS+i][mxS], &rgb[(yS+i)*xSize*3 + xS*3], xL*3);
    }
    else if(alpha < 1)//等于1就不用画了
    {
        unsigned char *line;
        int xE = xS + xL, yE = yS + yL;
        int i, j, cc, mxS2;
        for(i = 0; i < yL; i++, myS++)
        {
            line = &rgb[(yS+i)*xSize*3 + xS*3];
            for(j = 0, mxS2 = mxS; j < xL; j++, mxS2++)
            {
                cc = j*3;
#if(AMOLED_RGB_MODE == 1)
                AS->data.backupMap[myS][mxS2][0] = 
                    AS->data.backupMap[myS][mxS2][0]*alpha + line[cc]*(1-alpha);
                AS->data.backupMap[myS][mxS2][1] = 
                    AS->data.backupMap[myS][mxS2][1]*alpha + line[cc+1]*(1-alpha);
                AS->data.backupMap[myS][mxS2][2] = 
                    AS->data.backupMap[myS][mxS2][2]*alpha + line[cc+2]*(1-alpha);
#else
                AS->data.backupMap[myS][mxS2][2] = 
                    AS->data.backupMap[myS][mxS2][2]*alpha + line[cc]*(1-alpha);
                AS->data.backupMap[myS][mxS2][1] = 
                    AS->data.backupMap[myS][mxS2][1]*alpha + line[cc+1]*(1-alpha);
                AS->data.backupMap[myS][mxS2][0] = 
                    AS->data.backupMap[myS][mxS2][0]*alpha + line[cc+2]*(1-alpha);
#endif
            }
        }
    }
}

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

void ViewPen::print_map(int xStart, int yStart, int xEnd, int yEnd, unsigned char *rgb)
{
    if(!rgb ||
        xStart > XEND || yStart > YEND ||
        xEnd < XSTART || yEnd < YSTART)
        return;
}

void ViewPen::print_map(int xStart, int yStart, int xEnd, int yEnd, unsigned char *rgb, float alpha)
{
    if(!rgb ||
        xStart > XEND || yStart > YEND ||
        xEnd < XSTART || yEnd < YSTART)
        return;
}

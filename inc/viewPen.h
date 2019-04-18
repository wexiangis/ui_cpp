#ifndef _VIEWPEN_H_
#define _VIEWPEN_H_

#include "amoled.h"

class ViewPen
{
public:

    typedef enum{
        VPM_INIT = 0,
        VPM_LIGHT_UP = 1,
        VPM_LIGHT_DOWN = 2,
        VPM_WAKEUP = 3,
        VPM_SLEEP = 4,
        VPM_POWER_OFF = 5,
        VPM_REFRESH = 6,
    }ViewPen_Mode;

    //----- 构造函数 -----
    explicit ViewPen(int displayDir);
    ~ViewPen();

    //----- 设置 -----
    //重新设置屏幕方向
    void set_dir(int displayDir);
    //特殊功能设置
    void set_mode(ViewPen_Mode mode);
    //设置亮度 0~255
    void set_bright(unsigned char bright);

    //----- 绘图 -----
    //使能输出
    void output();
    //清屏
    void clear(int color);
    //画点 alpha 透明度0~1 0表示不透明 1表示完全透明
    void print(int x, int y, int color);
    void print(int x, int y, int color, float alpha);
    //面填充 alpha 透明度0~1 0表示不透明 1表示完全透明
    void print_rgb(unsigned char *rgb, int xStart, int yStart, int xSize, int ySize, float alpha);
    //面填充 map[y][x] = NULL 时跳过该点
    void print_map(unsigned char ***map, int xStart, int yStart, int xSize, int ySize, float alpha);
    //栅格图纯色填充 grid[x] = 0/1
    void print_grid(unsigned char *grid, int color, int xStart, int yStart, int xSize, int ySize, float alpha);
    //栅格图纯色填充 grid[x] = 0~255 代表颜色权重
    void print_grid2(unsigned char *grid, int color, int xStart, int yStart, int xSize, int ySize, float alpha);

    //----- 其它 -----

private:

    Amoled_Struct *AS;
    int XSTART, XEND;
    int YSTART, YEND;
    int WSIZE;
};

#endif

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
    void refresh();
    //清屏
    void clear(int color);
    //画点 alpha 透明度0~1 0表示不透明 1表示完全透明
    void print(int x, int y, int color);
    void print(int x, int y, int color, float alpha);
    //面填充
    void print_map(int xStart, int yStart, int xSize, int ySize, unsigned char *rgb, float alpha);

    //----- 其它 -----
    

private:

    Amoled_Struct *AS;
    int XSTART, XEND;
    int YSTART, YEND;
    int WSIZE;
};

#endif

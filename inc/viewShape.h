#ifndef _VIEWSHAPE_H_
#define _VIEWSHAPE_H_

//形状 栅格图制造者
class Polygon
{
public:

    //----- 构造函数 -----
    //line = 0/圆 1/直线 2/折线 >=3/多边形
    explicit Polygon(int line);//正多边形
    Polygon(int line, int *xy);//xy[] = {x1, y1, x2, y2, ...};
    ~Polygon();

    //----- 获取多边形数据 -----
    //获取圆形: rad > 0, 其它 = 0
    //获取圆环: rad, rad2 > 0 且 rad > rad2, 其它 = 0
    //获取扇形,分段圆环: angle, degree > 0; angle/开角角度 degree/旋转角度
    unsigned char *get_grid(int rad, int rad2, int angle, int degree, int &xErr, int &yErr);
    //获取直线,折线,多边形
    unsigned char *get_grid(int w, int h, int lineSize, int &xErr, int &yErr);
    //获取直线,折线,多边形
    unsigned char *get_mem(int w, int h, int color, int lineSize, int &xErr, int &yErr);
    
    //----- 基本参数 -----
    int line();

private:

    int LINE;//边数
    int *DOT;//多边形各端点坐标(相对于内接矩形

};

#endif

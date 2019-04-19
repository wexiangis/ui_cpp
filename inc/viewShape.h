#ifndef _VIEWSHAPE_H_
#define _VIEWSHAPE_H_

//形状 栅格图制造者
class ViewShape
{
public:

    //----- 构造函数 -----
    explicit ViewShape(int line, int x1, int y1, ...);//
    ~ViewShape();

    unsigned char *get_grid();
    
    //----- 基本参数 -----
    int width();
    int height();
    int line();

private:

    int WIDTH, HEIGHT;//外接矩形的长,宽
    int LINE;//边数
    int *DOT;//多边形各端点坐标(相对于内接矩形

};

#endif

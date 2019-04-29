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

    void set(int line);
    void set(int line, int *xy);

    //----- 获取多边形数据 -----
    //获取圆形: rad > 0, 其它 = 0
    //获取圆环: rad, rad2 > 0 且 rad > rad2, 其它 = 0
    //获取扇形,分段圆环: angle, degree > 0; angle/开角角度 degree/旋转角度
    unsigned char* get_circle(int rad, int rad2, 
        int angle, int degree, unsigned char weight, 
        unsigned char *grid, int *gw, int *gh, int *xyCentre);
    unsigned char* get_ellipse(int radX, int radY, 
        int lineSize, int angle, int degree, unsigned char weight, 
        unsigned char *grid, int *gw, int *gh, int *xyCentre);
    
    //获取直线,折线,多边形
    unsigned char* get_polygon(int w, int h, int lineSize, unsigned char weight);
    unsigned char* get_polygon_origin(int &w, int &h, int lineSize, unsigned char weight);

    //复杂多边形的填充
    unsigned char* get_polygon2(int w, int h, unsigned char weight);
    unsigned char* get_polygon2_origin(int &w, int &h, unsigned char weight);

    //矩形
    unsigned char* get_rect(int w, int h, int rad, int lineSize, unsigned char weight);

    //----- 基本参数 -----
    int line();

private:

    int LINE;//边数
    int DOT_LEN;//DOT长度 例如:{x1,y1,x2,y3} DOT_LEN=4
    int *DOT;//多边形各端点坐标(相对于内接矩形
    int WIDTH, HEIGHT;//外接矩形的宽高
};

class ViewShape : public Polygon
{
public:

    typedef enum{
        VST_NULL = 0,
        VST_BUTTON,
        VST_SWITCH,
        VST_SCROLL_BAR,
        VST_PROCESS_BAR,
        VST_CIRCLE,
        VST_SECTOR,
    }ViewShape_Type;

private:

};

#endif

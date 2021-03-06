#ifndef _VIEWPICTURE_H_
#define _VIEWPICTURE_H_

//注意事项:
//  禁止对象拷贝(内有指针内存分配)

class ViewPicture
{
public:

    //----- 构造函数 -----
    explicit ViewPicture(const char *picPath);
    ~ViewPicture();

    //----- 修改原图数据 -----
    void set_replaceColor(int *distColot, int *repColor, int count);
    void set_wsight(float weight, int *distColot, int count);

    //----- 采集原图数据 -----
    int get_color(int xy[2]);
    int get_color(int x, int y);

    //----- 获取 rgb 数据指针 (用完记得free) -----
    unsigned char* get_mem(int w, int h, //指定width,height 获得缩放后的rgb图片数据
        int *distColot, //指定目标颜色的数组
        int *repColor,  //指定替换颜色的数组
        int count,      //颜色个数
        float weight);  //系数加权: color*weight // 0,1/不起用 >0/启用

    //----- 获取 map[y][x][3] 指针 (用完记得 release_map) -----
    unsigned char*** get_map(int w, int h, //指定width,height 获得缩放后的图片数据
        int *alphaColor, //指定透明色数组(目标指针将为NULL,例如 map[Y][X]=NULL
        int count);      //透明色个数
    unsigned char*** get_map(unsigned char *pic, int width, int height, int pw);
    unsigned char*** get_map(unsigned char *pic, int picWidth, int picHeight, int mapWidth, int mapHeight, int pw);
    void release_map(unsigned char ***map, int w, int h);

    //----- 其它 -----
    //刷新图片数据,重新从文件读入
    void refresh();
    // 注意: 当图片大小发生改变时,将重新生成 MEM 和 MAP,之前 get_map() 获取的指针访问将是危险的 !!
    
    //----- 基本参数 -----
    char ready();//返回 0/异常(比如图片无法正常加载) 1/正常
    int width();
    int height();
    int wSize();
    int memSize();
    char* picPath();
    unsigned char* mem();

private:

    //原始数据
    int WIDTH, HEIGHT, WSIZE, MEMSIZE;//原始大小 WSIZE/每像素字节
    unsigned char *MEM;//原始数据
    unsigned char ***MAP;
    char *PICPATH;
    char READY;

};

#endif

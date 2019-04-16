#ifndef _VIEWPICTURE_H_
#define _VIEWPICTURE_H_

class ViewPicture{

public:

    int width, height, wSize;//当前大小 wSize/每像素字节
    unsigned char *mem;
    unsigned char ***map;//map[y][x][wSize]快速定位

    //----- 构造函数 -----
    explicit ViewPicture(char *picPath);
    ~ViewPicture();

    //----- 设置缩放值 -----
    void set_width(int w);
    void set_height(int height);
    void set_size(int w, int h);

    //----- 设置像素属性 -----
    void set_alpha_color(int distColot);//设置透明色 可以设置多个
    void set_replace_color(int distColot, int repColor);//设置替换色 可以设置多个

    //----- 获取原始数据指针 (用完不要free) -----
    unsigned char *get_mem();
    unsigned char *get_mem(int w, int h);//指定width,height 缩放处理后返回

    //----- 获取map[y][x][wSize]指针 (用完不要free) -----
    unsigned char ***get_map();
    unsigned char ***get_map(int w, int h);//指定width,height 缩放处理后返回

    //----- 其它 -----
    void recover();//恢复原始数据 前面的所有设置作废
    void recover_pixel();//恢复像素设置
    void recover_size();//恢复缩放设置,用回原来的大小
    void refresh();//刷新图片数据(重新从文件读入)

private:

    int WIDTH, HEIGHT, WSIZE;//原始大小 WSIZE/每像素字节
    unsigned char *MEM;//原始数据

};

#endif

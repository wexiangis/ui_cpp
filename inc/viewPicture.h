#ifndef _VIEWPICTURE_H_
#define _VIEWPICTURE_H_

#include <iostream>

//注意事项:
//  禁止对象拷贝(内有指针内存分配)

class ViewPicture{

public:

    //----- 构造函数 -----
    explicit ViewPicture(const char *picPath);
    ~ViewPicture();

    //----- 设置像素属性 -----
    void set_alpha_color(int distColot);//设置透明色 可以设置多个
    void set_replace_color(int distColot, int repColor);//设置替换色 可以设置多个

    //----- 获取原始数据指针 (用完记得free) -----
    unsigned char* get_mem(int w, int h);//指定width,height 获得缩放后的图片数据

    //----- 获取 map[y][x][3] 指针 (用完记得 release_map) -----
    unsigned char*** get_map(int w, int h);//指定width,height 获得缩放后的图片数据
    void release_map(unsigned char ***map, int w, int h);

    //----- 其它 -----
    void refresh();//刷新图片数据(重新从文件读入)
    void refresh(const char *picPath);//刷新图片数据(重新从文件读入) 更换图片
    char isReady();//返回 0/异常(比如图片无法正常加载) 1/正常

private:

    //原始数据
    int WIDTH, HEIGHT, WSIZE, MEMSIZE;//原始大小 WSIZE/每像素字节
    unsigned char *MEM;//原始数据
    char *PICPATH;

    char READY;

};

#endif

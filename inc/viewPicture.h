#ifndef _VIEWPICTURE_H_
#define _VIEWPICTURE_H_

#include <iostream>

//注意事项:
//  禁止对象拷贝(内有指针内存分配)

class ViewPicture{

public:

    //----- 构造函数 -----
    explicit ViewPicture(char *picPath);
    ~ViewPicture();

    //----- 基本参数 -----
    int width(){return Width;}
    int height(){return Height;}
    int wSize(){return WSIZE;}
    int memSize(){return MemSize;}
    unsigned char* mem(){return Mem;}
    unsigned char*** map(){return Map;}

    //----- 设置缩放值 -----
    void set_width(int w);
    void set_height(int h);
    void set_size(int w, int h);

    //----- 设置像素属性 -----
    void set_alpha_color(int distColot);//设置透明色 可以设置多个
    void set_replace_color(int distColot, int repColor);//设置替换色 可以设置多个

    //----- 获取原始数据指针 (用完不要free) -----
    unsigned char* get_mem(int w, int h);//指定width,height 缩放处理后返回

    //----- 获取map[y][x][wSize]指针 (用完不要free) -----
    unsigned char*** get_map(int w, int h);//指定width,height 缩放处理后返回

    //----- 其它 -----
    void refresh();//刷新图片数据(重新从文件读入)
    void refresh(char *picPath);//刷新图片数据(重新从文件读入) 更换图片
    char isReady();//返回 0/异常(比如图片无法正常加载) 1/正常

private:

    //用户修改后的图片数据
    int Width, Height, MemSize;
    unsigned char *Mem;
    unsigned char ***Map;//map[y][x][wSize]快速定位

    //原始数据
    int WIDTH, HEIGHT, WSIZE, MEMSIZE;//原始大小 WSIZE/每像素字节
    unsigned char *MEM;//原始数据
    char *PICPATH;

    char READY;

};

#endif

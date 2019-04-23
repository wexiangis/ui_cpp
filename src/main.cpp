#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "viewPen.h"
#include "viewPicture.h"
#include "viewShape.h"
#include <iostream>

int clock_count()
{
    static int last = 0, th = 0;
    int current = clock();
    int result = current-last;
    std::cout<<"clock "<<th<<" : "<<result<<std::endl;
    last = current;
    th += 1;
    return result;
}

unsigned char* build_grid(int w, int h)
{
    unsigned char *ret = new unsigned char[w*h];
    int xC, yC, cc;
    unsigned char weight = 0;

    for(yC = cc = 0; yC < h; yC++)
    {
        for(xC = weight = 0; xC < w; xC++)
        {
            ret[cc++] = weight++;
        }
    }

    return ret;
}

#define TEST_MODE 2

#if(TEST_MODE == 2)

#include <stdlib.h>
#include <string.h>

//
int main(int argc, char **argv)
{
    int rad = 20, w = VIEW_X_SIZE, h = VIEW_Y_SIZE, degree = 0, angle = 360-30, degree2;

    int gridSize = w*h;
    unsigned char *grid = new unsigned char[gridSize];

    Polygon pg(0);

    degree2 = degree%360;
    rad = 20;
    for(int i = 0; i < 20; i++)
    {
        pg.get_grid(rad, rad-10, angle, degree2, grid, &w, &h, 0xFF);
        rad += 20; degree2 += 60;
    }
    degree -= 30;

    ViewPen pen(0);

    while(1)
    {
        pen.clear(0);
        pen.print_grid(grid, 0xFF0000, 0, 0, w, h, 0);
        pen.output();
        usleep(200000);

        memset(grid, 0, gridSize);

        degree2 = degree%360;
        rad = 20;
        for(int i = 0; i < 20; i++)
        {
            pg.get_grid(rad, rad-10, angle, degree2, grid, &w, &h, 0xFF);
            rad += 20; degree2 += 60;
        }
        degree -= 30;
    }

    return 0;
}

#elif(TEST_MODE == 1)

//
int main(int argc, char **argv)
{
    // ViewPicture pic("./test.jpg");
    ViewPicture pic("./res/bmp/test.bmp");
    // ViewPicture pic("./res/jpg/test.jpg");
    if(!pic.ready())
    {
        std::cout<<"pic init err !"<<std::endl;
        return -1;
    }

    int resW = 600, resH = 400;

    unsigned char *res = pic.get_mem(resW, resH, 
        new int[5] {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFFFF, 0x000000}, 
        new int[5] {0xFFFFFF, 0xC0C0C0, 0x808080, 0x606060, 0x202020}, 
        0, 1);
    unsigned char ***res2 = pic.get_map(resW, resH, new int[1] {0xFF0000}, 0);
    unsigned char *grid = build_grid(resW, resH);

    ViewPen pen(0);
    pen.clear(0);

    while(1)
    {
        clock_count();
        std::cout<<"\n";

        pen.print_rgb(res, 0, 0, resW, resH, 0);
        clock_count();
        pen.print_map(res2, 50, 50, resW, resH, 0);
        clock_count();
        pen.print_grid(grid, 0xFF0000, 100, 100, resW, resH, 0);
        clock_count();
        pen.print_grid2(grid, 0xFF0000, 150, 150, resW, resH, 0);
        clock_count();
        std::cout<<"\n";

        pen.print_rgb(res, 0, 0+200, resW, resH, 0.5);
        clock_count();
        pen.print_map(res2, 50, 50+200, resW, resH, 0.5);
        clock_count();
        pen.print_grid(grid, 0x0000FF, 100, 100+200, resW, resH, 0.5);
        clock_count();
        pen.print_grid2(grid, 0x0000FF, 150, 150+200, resW, resH, 0.5);
        clock_count();
        std::cout<<"\n";

        // pen.print_rgb(res, 0, 0, resW, resH, 0);
        // pic.refresh();

        pen.output();
        sleep(1);
    }

    return 0;
}

#elif(TEST_MODE == 0)
//
int main(int argc, char **argv)
{

    AnyType<string> vt13("vt13", new string[5]{"cdf", "234", "jjj", "kkk", "ddd"}, 5);

    cout<<"class name: "<<vt13.name()<<endl;

    cout<<vt13.get()<<", "<<vt13.get(1)<<endl;

    string *ret = vt13.getAll();
    cout<<ret[0]<<", "<<ret[1]<<", "<<ret[2]<<endl;
    delete[] ret;

    vt13.del(2);
    vt13.insert("bbb", 2);

    string *ret2 = vt13.getN(1, 3);
    cout<<ret2[0]<<", "<<ret2[1]<<", "<<ret2[2]<<endl;
    delete[] ret2;

    return 0;
}

#endif

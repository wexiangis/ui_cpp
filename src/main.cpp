#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "viewPen.h"
#include "viewPicture.h"
#include "viewShape.h"
#include "viewType.h"
#include <iostream>

int clock_count(int tag)
{
    static int last = 0, th = 0;
    int current = clock();
    int result = current-last;
    std::cout<<'['<<tag<<"] clock "<<th<<" : "<<result<<std::endl;
    last = current;
    th += 1;
    return result;
}

#define TEST_MODE 2

#if(TEST_MODE == 4)

//
int main(int argc, char **argv)
{


    return 0;
}

#elif(TEST_MODE == 3)

#include <stdlib.h>
#include <string.h>

//
int main(int argc, char **argv)
{
    int w = VIEW_X_SIZE, h = VIEW_Y_SIZE;
    // int w = 100, h = 100;

    // Polygon pg( 3 );

    // Polygon pg( 20 , new int[20*2] {
    //     -2,3, 2,3, 1,5, 5,5, 5,1, 3,2, 3,-2, 5,-1, 5,-5, 1,-5, 
    //     2,-3, -2,-3, -1,-5, -5,-5, -5,-1, -3,-2, -3,2, -5,1, -5,5, -1,5} );

    // Polygon pg( 49 , new int[49*2] {
    //     0,-1, 1,0, 2,-1, 3,0, 4,-1, 5,0, 6,-1, 7,0, 7,1, 6,0, 
    //     5,1, 4,0, 3,1, 2,0, 1,1, 0,0, 0,1, 1,2, 2,1, 3,2, 
    //     4,1, 5,2, 6,1, 7,2, 8,1, 8,0, 7,-1, 6,-2, 5,-1, 4,-2, 
    //     3,-1, 2,-2, 1,-1, 1,-2, 2,-3, 3,-2, 4,-3, 5,-2, 6,-3, 7,-2, 
    //     8,-1, 8,-2, 6,-4, 5,-3, 4,-4, 3,-3, 2,-4, 1,-3, 0,-2
    // });
    
    Polygon pg( 20 , new int[20*2] {
        0,0, -1,0, -1,2, 2,2, 2,-2, -3,-2, -3,4, 4,4, 4,-4, -5,-4,
        -5,4, -4,4, -4,-3, 3,-3, 3,3, -2,3, -2,-1, 1,-1, 1,1, 0,1
    });
    
    // Polygon pg( 20 , new int[20*2] {
    //     0,0, 1,0, 1,2, -2,2, -2,-2, 3,-2, 3,4, -4,4, -4,-4, 5,-4,
    //     5,4, 4,4, 4,-3, -3,-3, -3,3, 2,3, 2,-1, -1,-1, -1,1, 0,1
    // });

    unsigned char *grid = pg.get_polygon(w-80, h-80, 5, 0xFF);//画线
    // unsigned char *grid = pg.get_polygon(w-80, h-80, 0, 0xFF);//普通图形填充
    // unsigned char *grid = pg.get_polygon2(w-80, h-80, 0xFF);//复杂凹多边形填充
    // unsigned char *grid = pg.get_rect(w-80, h-80, 100, 50, 0xFF);

    unsigned char *grid2 = pg.get_rect(w, h, 50, 10, 0xFF);

    ViewPen pen(0);
    pen.clear(0);
    
    pen.print_grid(grid, 0xFF0000, 40, 40, w-80, h-80, 0);
    pen.print_grid(grid2, 0xFF0000, 0, 0, w, h, 0);

    pen.output();
    usleep(10000);

    return 0;
}

#elif(TEST_MODE == 2)

#include <stdlib.h>
#include <string.h>

//
int main(int argc, char **argv)
{
    int rad = 20, w = VIEW_X_SIZE, h = VIEW_Y_SIZE, degree = 0, angle = 360-60, degree2;

    int gridSize = w*h;
    unsigned char *grid = new unsigned char[gridSize];

    Polygon pg(0);

    ViewPen pen(0);

    while(1)
    {
        memset(grid, 0, gridSize);

        degree2 = degree%360;
        rad = 20;
        for(int i = 0; i < 18; i++)
        {
            // pg.get_circle(rad, rad-5-(i%2)*10, angle, degree2, 0xFF, grid, &w, &h, NULL);
            if(i%2)
                pg.get_ellipse(rad, rad/2, 10, angle, degree2, 0xFF, grid, &w, &h, NULL);
            else
                pg.get_ellipse(rad, rad/2, 10, angle, degree2, 0xFF, grid, &w, &h, NULL);
            rad += 20; degree2 += 60;
        }
        degree -= 30;

        pen.clear(0);
        pen.print_grid(grid, 0xFF0000, 0, 0, w, h, 0);
        pen.output();
        usleep(200000);
    }

    return 0;
}

#elif(TEST_MODE == 1)

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
        clock_count(0);
        std::cout<<"\n";

        pen.print_rgb(res, 0, 0, resW, resH, 0);
        clock_count(1);
        pen.print_map(res2, 50, 50, resW, resH, 0);
        clock_count(2);
        pen.print_grid(grid, 0xFF0000, 100, 100, resW, resH, 0);
        clock_count(3);
        pen.print_grid2(grid, 0xFF0000, 150, 150, resW, resH, 0);
        clock_count(4);
        std::cout<<"\n";

        pen.print_rgb(res, 0, 0+200, resW, resH, 0.5);
        clock_count(5);
        pen.print_map(res2, 50, 50+200, resW, resH, 0.5);
        clock_count(6);
        pen.print_grid(grid, 0x0000FF, 100, 100+200, resW, resH, 0.5);
        clock_count(7);
        pen.print_grid2(grid, 0x0000FF, 150, 150+200, resW, resH, 0.5);
        clock_count(8);
        std::cout<<"\n";

        // pen.print_rgb(res, 0, 0, resW, resH, 0);
        // pic.refresh();

        pen.output();
        sleep(1);
    }

    return 0;
}

#endif

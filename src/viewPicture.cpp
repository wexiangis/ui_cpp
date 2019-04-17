#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "viewPicture.h"
#include "bmp.h"
#include "jpeg.h"

//
unsigned char ***_mapInit(unsigned char *pic, int width, int height, int pw)
{
    unsigned char ***map;
    int xC, yC;
    //
    if(pic == NULL || width < 1 || height < 1 || pw < 1)
        return NULL;
    //
    map = (unsigned char ***)calloc(height+1, sizeof(unsigned char **));
    for(yC = 0; yC < height; yC++)
    {
        map[yC] = (unsigned char **)calloc(width+1, sizeof(unsigned char *));
        for(xC = 0; xC < width; xC++)
            map[yC][xC] = &pic[yC*width*pw + xC*pw];
    }
    //
    return map;
}

//
unsigned char ***_mapInit(unsigned char *pic, int picWidth, int picHeight, int mapWidth, int mapHeight, int pw)
{
    unsigned char ***map;
    int xC, yC, pxC, pyC;
    float pxCf, pyCf, xPow, yPow;
    //
    if(pic == NULL || mapWidth < 1 || mapHeight < 1 || pw < 1 || picWidth < 1 || picHeight < 1)
        return NULL;
    //
    xPow = picWidth/mapWidth;
    yPow = picHeight/mapHeight;
    //
    map = (unsigned char ***)calloc(mapHeight+1, sizeof(unsigned char **));
    for(yC = pyCf = 0; yC < mapHeight; yC++, pyCf+=yPow)
    {
        map[yC] = (unsigned char **)calloc(mapWidth+1, sizeof(unsigned char *));
        for(xC = pxCf = 0, pyC = pyCf; xC < mapWidth; xC++, pxCf+=xPow)
        {
            pxC = pxCf;
            map[yC][xC] = &pic[pyC*picWidth*pw + pxC*pw];
        }
    }
    //
    return map;
}

//
void _mapRelease(unsigned char ***map, int width, int height)
{
    int i;
    //
    if(map == NULL)
        return;
    //
    for(i = 0; i < height; i++)
        free(map[i]);
    free(map);
}

ViewPicture::ViewPicture(char *picPath)
{
    if(picPath == NULL)
        return;
    //备份图片路径
    int pathLen = strlen(picPath);
    PICPATH = (char *)calloc(pathLen+1, 1);
    strcpy(PICPATH, picPath);
    //读取图片
    if(access(picPath, F_OK) == 0)
    {
        if(strstr(picPath, ".bmp") || strstr(picPath, ".BMP"))
            MEM = bmp_get(PICPATH, &MEMSIZE, &WIDTH, &HEIGHT, &WSIZE);
        else
            MEM = dejpeg(PICPATH,  &MEMSIZE, &WIDTH, &HEIGHT, &WSIZE);
        //
        if(!MEM)
            return;
        //

        //就绪
        READY = 1;
    }
}

ViewPicture::~ViewPicture()
{
    if(Mem) free(Mem);
    if(Map) _mapRelease(Map, Width, Height);
    if(MEM) free(MEM);
    if(PICPATH) free(PICPATH);
    READY = 0;
}

void ViewPicture::set_width(int w)
{
    if(w == Width)
        return;
    //
}

void ViewPicture::set_height(int h)
{
    
}

void ViewPicture::set_size(int w, int h)
{
    
}

void ViewPicture::set_alpha_color(int distColot)
{
    
}

void ViewPicture::set_replace_color(int distColot, int repColor)
{
    
}

unsigned char* ViewPicture::get_mem(int w, int h)
{
    return NULL;
}

unsigned char*** ViewPicture::get_map(int w, int h)
{
    return NULL;
}

void ViewPicture::refresh()
{
    
}

void ViewPicture::refresh(char *picPath)
{
    
}

char ViewPicture::isReady()
{
    return READY;
}


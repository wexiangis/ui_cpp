#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "viewPicture.h"
#include "bmp.h"
#include "jpeg.h"

//
unsigned char*** _mapInit(unsigned char *pic, int width, int height, int pw)
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
unsigned char*** _mapInit(unsigned char *pic, int picWidth, int picHeight, int mapWidth, int mapHeight, int pw)
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
    if(map == NULL)
        return;
    for(i = 0; i < height; i++)
        free(map[i]);
    free(map);
}

//用完记得free()
unsigned char* _intRGB_to_charRGB(int *color, int count)
{
    unsigned char *ret = (unsigned char *)calloc(count*3, sizeof(unsigned char));
    for(int i = 0, j = 0; i < count; i++)
    {
        ret[j++] = (unsigned char)((color[i]&0xFF0000)>>16);
        ret[j++] = (unsigned char)((color[i]&0x00FF00)>>8);
        ret[j++] = (unsigned char)(color[i]&0x0000FF);
    }
    return ret;
}

int _charRGB_to_intRGB(unsigned char *color)
{
    // return ((color[0]<<16) | (color[1]<<8) | color[0]);
    int ret = color[0];
    ret = (ret<<8) | color[1];
    ret = (ret<<8) | color[2];
    return ret;
}

//
ViewPicture::ViewPicture(const char *picPath)
{
    if(picPath == NULL)
        return;
    READY = 0;
    //备份图片路径
    int pathLen = strlen(picPath);
    PICPATH = (char *)calloc(pathLen+1, sizeof(char));
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
        MAP = _mapInit(MEM, WIDTH, HEIGHT, WSIZE);
        //就绪
        READY = 1;
    }
}

ViewPicture::~ViewPicture()
{
    READY = 0;
    if(MAP) _mapRelease(MAP, WIDTH, HEIGHT);
    if(MEM) free(MEM);
    if(PICPATH) free(PICPATH);
}

void ViewPicture::refresh()
{
    unsigned char *memTemp = NULL;
    int ms = 0, w = 0, h = 0, ws = 0;
    //读取图片
    if(access(PICPATH, F_OK) == 0)
    {
        if(strstr(PICPATH, ".bmp") || strstr(PICPATH, ".BMP"))
            memTemp = bmp_get(PICPATH, &ms, &w, &h, &ws);
        else
            memTemp = dejpeg(PICPATH,  &ms, &w, &h, &ws);
    }
    //
    if(memTemp)
    {
        READY = 0;
        //
        if(ms == MEMSIZE && w == WIDTH && h == HEIGHT && ws == WSIZE)
        {
            memcpy(MEM, memTemp, ms);
            free(memTemp);
        }
        else
        {
            if(MAP)
                _mapRelease(MAP, WIDTH, HEIGHT);
            if(MEM)
                free(MEM);
            //
            MEMSIZE = ms;
            WIDTH = w;
            HEIGHT = h;
            WSIZE = ws;
            //
            MEM = memTemp;
            MAP = _mapInit(MEM, WIDTH, HEIGHT, WSIZE);
        }
        //就绪
        READY = 1;
    }
}

void ViewPicture::set_replaceColor(int *distColot, int *repColor, int count)
{
    if(!READY || !distColot || !repColor || !count)
        return;
    unsigned char *rgbDist = _intRGB_to_charRGB(distColot, count);
    unsigned char *rgbRep = _intRGB_to_charRGB(repColor, count);
    int i, j, cc;
    //
    for(i = 0; i < MEMSIZE;)
    {
        for(j = cc = 0; j < count; j++)
        {
            if(MEM[i] == rgbDist[cc] && 
                MEM[i+1] == rgbDist[cc+1] &&
                MEM[i+2] == rgbDist[cc+2])
            {
                MEM[i] = rgbRep[cc];
                MEM[i+1] = rgbRep[cc+1];
                MEM[i+2] = rgbRep[cc+2];
                break;//命中一次就跳出 不再和其它颜色比对
            }
            cc += 3;
        }
        i += 3;
    }
    //
    free(rgbDist);
    free(rgbRep);
}

void ViewPicture::set_wsight(float weight, int *distColot, int count = 0)
{
    if(!READY || weight == 0 || weight == 1)
        return;
    if(count > 0 && distColot)
    {
        unsigned char *rgbDist = _intRGB_to_charRGB(distColot, count);
        int result, i, j, k, cc;
        for(i = 0; i < MEMSIZE;)
        {
            for(j = cc = 0; j < count; j++)
            {
                if(MEM[i] == rgbDist[cc] && 
                    MEM[i+1] == rgbDist[cc+1] &&
                    MEM[i+2] == rgbDist[cc+2])
                {
                    for(k = 0; k < 3; k++)
                    {
                        if(!MEM[i+k])//数值为0时无法加权
                            MEM[i+k] = 1;
                        //
                        result = MEM[i+k]*weight;
                        if(result > 0xFF)
                            MEM[i+k] = 0xFF;
                        else if(result < 0)
                            MEM[i+k] = 0;
                        else
                            MEM[i+k] = result;
                    }
                    break;//命中一次就跳出 不再和其它颜色比对
                }
                cc += 3;
            }
            i += 3;
        }
        //
        free(rgbDist);
    }
    else
    {
        int result, i, j;
        for(i = 0; i < MEMSIZE;)
        {
            for(j = 0; j < 3; j++, i++)
            {
                if(!MEM[i])//数值为0时无法加权
                    MEM[i] = 1;
                //
                result = MEM[i]*weight;
                if(result > 0xFF)
                    MEM[i] = 0xFF;
                else if(result < 0)
                    MEM[i] = 0;
                else
                    MEM[i] = result;
            }
        }
    }
}

int ViewPicture::get_color(int xy[2])
{
    if(!READY || !xy)
        return 0;
    return _charRGB_to_intRGB(MAP[xy[1]][xy[0]]);
}

int ViewPicture::get_color(int x, int y)
{
    if(!READY || x < 0 || x > WIDTH-1 || y < 0 || y > HEIGHT-1)
        return 0;
    return _charRGB_to_intRGB(MAP[y][x]);
}

unsigned char* ViewPicture::get_mem(int w, int h,
    int *distColot = NULL,
    int *repColor = NULL,
    int count = 0,
    float weight = 0)
{
    if(!READY || w < 1 || h < 1)
        return NULL;
    //
    int mem_size = w*h*3;
    unsigned char *mem = (unsigned char *)calloc(mem_size+1, sizeof(unsigned char));
    int xC, yC, pxC, pyC, i; // xC,yC 为 mem 的位置计数; pxC,pyC 为 MAP 的位置计数
    float pxCf, pyCf, xPow, yPow;
    //
    xPow = (float)WIDTH/w;
    yPow = (float)HEIGHT/h;//
    //
    for(i = yC = 0, pyCf = 0; yC < h; yC++, pyCf+=yPow)
    {
        pyC = pyCf;//浮点转int
        for(xC = 0, pxCf = 0; xC < w; xC++, pxCf+=xPow)
        {
            pxC = pxCf;//浮点转int
            mem[i++] = MAP[pyC][pxC][0];
            mem[i++] = MAP[pyC][pxC][1];
            mem[i++] = MAP[pyC][pxC][2];
        }
    }
    //颜色处理
    if(count > 0 && distColot && repColor)
    {
        unsigned char *rgbDist = _intRGB_to_charRGB(distColot, count);
        unsigned char *rgbRep = _intRGB_to_charRGB(repColor, count);
        int j, cc;
        //
        for(i = 0; i < mem_size;)
        {
            for(j = cc = 0; j < count; j++)
            {
                if(mem[i] == rgbDist[cc] && 
                    mem[i+1] == rgbDist[cc+1] &&
                    mem[i+2] == rgbDist[cc+2])
                {
                    mem[i] = rgbRep[cc];
                    mem[i+1] = rgbRep[cc+1];
                    mem[i+2] = rgbRep[cc+2];
                    break;//命中一次就跳出 不再和其它颜色比对
                }
                cc += 3;
            }
            i += 3;
        }
        //
        free(rgbDist);
        free(rgbRep);
    }
    //权重处理
    if(weight && weight != 1)
    {
        int result, j;
        for(i = 0; i < mem_size;)
        {
            for(j = 0; j < 3; j++, i++)
            {
                //数值为0时无法加权
                if(!mem[i])
                    mem[i] = 1;
                //
                result = mem[i]*weight;
                if(result > 0xFF)
                    mem[i] = 0xFF;
                else if(result < 0)
                    mem[i] = 0;
                else
                    mem[i] = result;
            }
        }
    }
    //
    return mem;
}

unsigned char*** ViewPicture::get_map(unsigned char *pic, int width, int height, int pw)
{
    return _mapInit(pic, width, height, pw);
}

unsigned char*** ViewPicture::get_map(unsigned char *pic, int picWidth, int picHeight, int mapWidth, int mapHeight, int pw)
{
    return _mapInit(pic, picWidth, picHeight, mapWidth, mapHeight, pw);
}

unsigned char*** ViewPicture::get_map(int w, int h,
    int *alphaColor = NULL,
    int count = 0)
{
    if(!READY || w < 1 || h < 1)
        return NULL;
    //
    unsigned char ***map = _mapInit(MEM, WIDTH, HEIGHT, w, h, WSIZE);
    //透明色处理
    if(map && count > 0 && alphaColor)
    {
        unsigned char *alphaRGB = _intRGB_to_charRGB(alphaColor, count);
        int xC, yC;
        int i, cc;
        for(xC = 0; xC < w; xC++)
        {
            for(yC = 0; yC < h; yC++)
            {
                for(i = cc = 0; i < count; i++)
                {
                    if(map[yC][xC][0] == alphaRGB[cc] &&
                        map[yC][xC][1] == alphaRGB[cc+1] &&
                        map[yC][xC][2] == alphaRGB[cc+2])
                    {
                        map[yC][xC] = NULL;
                        break;
                    }
                    cc += 3;
                }
            }
        }
        free(alphaRGB);
    }
    //
    return map;
}

void ViewPicture::release_map(unsigned char ***map, int w, int h)
{
    _mapRelease(map, w, h);
}

char ViewPicture::ready(){return READY;}
int ViewPicture::width(){return WIDTH;}
int ViewPicture::height(){return HEIGHT;}
int ViewPicture::wSize(){return WSIZE;}
int ViewPicture::memSize(){return MEMSIZE;}
char* ViewPicture::picPath(){return PICPATH;}
unsigned char* ViewPicture::mem(){return MEM;}

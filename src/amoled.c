
#include "amoled.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <linux/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdint.h>
#include <getopt.h>
#include <linux/spi/spidev.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdarg.h>

#if(!VIEW_CONFIG_MODE)
#include "bmp.h"
#include "jpeg.h"
#endif

//---------- 驱动接口 ----------

//============== LCD基本绘图方法 =============
#define	AMOLED_WMODE    _IOW('Q', 0x31, int)
#define	AMOLED_XYSET    _IOW('Q', 0x32, int)
#define	AMOLED_DRAW     _IOW('Q', 0x33, int)
#define	AMOLED_BRIGHT   _IOW('Q', 0x37, int)
//=============== dma设置方法 ===============
#define	AMOLED_MODE_SET	    _IOW('Q', 0x34, int)
#define	AMOLED_PERW_SET	    _IOW('Q', 0x35, int)
#define	AMOLED_SPEED_SET    _IOW('Q', 0x36, int)
//=============== 专用绘图方法 ================
#define	AMOLED_FB_INIT	    _IOW('Q', 0x41, int)       

//-------------------- 基本接口, 不可重入 --------------------

#if(!VIEW_CONFIG_MODE && VIEW_CONFIG_FRAME)
static char outputBmp[64] = "frameOutput/p";
extern void screen_string_addNum(char *str, int strMemSize);
#endif
//
static void _amoled_draw(Amoled_Struct *as)
{
#if(!VIEW_CONFIG_MODE)
#if(VIEW_CONFIG_FRAME)
    char bmpFile[64] = {0};
    screen_string_addNum(outputBmp, 64);
    // BMP
    // snprintf(bmpFile, 64, "%s.bmp", outputBmp);
    // bmp_create(bmpFile, as->data.data, as->base.xSize, as->base.ySize, as->base.perW);
    // JPG
    snprintf(bmpFile, 64, "%s.jpg", outputBmp);
    enjpeg(bmpFile, as->data.data, as->base.xSize, as->base.ySize, as->base.perW);
#endif
    // bmp_create("test.bmp", as->data.data, as->base.xSize, as->base.ySize, as->base.perW);
    enjpeg("test.jpg", as->data.data, as->base.xSize, as->base.ySize, as->base.perW);
#else
#if(AMOLED_USE_FRAMEBUFFER)
    memcpy(as->base.hware.fb, as->data.data, as->base.size);
#else
    ioctl(as->base.hware.fdTemp, AMOLED_DRAW, (void *)as->data.data);
#endif  //#if(AMOLED_USE_FRAMEBUFFER)
#endif
}

//-------------------- draw --------------------
//
void amoled_draw(Amoled_Struct *as)
{
    _amoled_draw(as);
}

//-------------------- Map init --------------------

void _amoled_mapInit(Amoled_Struct *as, unsigned char *data, amoledMapY **dataMap)
{
    int xC, yC, i, xC2, yC2;
    amoledMapY *map;
    //
    if(as->base.xSize != as->base.ySize)
    {
        if(as->base.xSize > as->base.ySize)
            as->data.map_xSize = as->data.map_ySize = as->base.xSize;
        else
            as->data.map_xSize = as->data.map_ySize = as->base.ySize;
    }
    else
    {
        as->data.map_xSize = as->base.xSize;
        as->data.map_ySize = as->base.ySize;
    }
    *dataMap = map = (amoledMapY *)calloc(as->data.map_ySize, sizeof(amoledMapY));
    //dataMap 指针准备
    if(as->base.displayDir == 0)
    {
        for(yC = 0; yC < as->data.map_ySize; yC++)
        {
            map[yC] = (amoledMapX *)calloc(as->data.map_xSize, sizeof(amoledMapX));
            for(xC = 0; xC < as->data.map_xSize; xC++)
            {
                if(yC < as->base.ySize && xC < as->base.xSize)  //范围内
                    map[yC][xC] = 
                        &data[yC*as->base.xSize*as->base.perW + xC*as->base.perW];
                else  //范围外
                    map[yC][xC] = &data[0];
            }
        }
    }
    else if(as->base.displayDir == 1)
    {
        for(yC = 0; yC < as->data.map_xSize; yC++)
        {
            map[yC] = (amoledMapX *)calloc(as->data.map_xSize, sizeof(amoledMapX));
            for(xC = 0; xC < as->data.map_ySize; xC++)
            {
                if(yC < as->base.xSize && xC < as->base.ySize)  //范围内
                    map[yC][xC] = 
                        &data[(xC)*as->base.xSize*as->base.perW + (as->base.xSize - yC - 1)*as->base.perW];
                else  //范围外
                    map[yC][xC] = &data[0];
            }
        }
    }
    else if(as->base.displayDir == 2)
    {
        for(yC = 0; yC < as->data.map_ySize; yC++)
        {
            map[yC] = (amoledMapX *)calloc(as->data.map_xSize, sizeof(amoledMapX));
            for(xC = 0; xC < as->data.map_xSize; xC++)
            {
                if(yC < as->base.ySize && xC < as->base.xSize)  //范围内
                    map[yC][xC] = 
                        &data[(as->base.ySize - yC - 1)*as->base.xSize*as->base.perW + 
                        (as->base.xSize - xC)*as->base.perW];
                else  //范围外
                    map[yC][xC] = &data[0];
            }
        }
    }
    else if(as->base.displayDir == 3)
    {
        for(yC = 0; yC < as->data.map_xSize; yC++)
        {
            map[yC] = (amoledMapX *)calloc(as->data.map_xSize, sizeof(amoledMapX));
            for(xC = 0; xC < as->data.map_ySize; xC++)
            {
                if(yC < as->base.xSize && xC < as->base.ySize)  //范围内
                    map[yC][xC] = 
                        &data[(as->base.ySize - xC - 1)*as->base.xSize*as->base.perW + (yC)*as->base.perW];
                else  //范围外
                    map[yC][xC] = &data[0];
            }
        }
    }
}

void amoled_mapInit(Amoled_Struct *as)
{
    as->data.data = (unsigned char *)calloc(as->base.size, sizeof(unsigned char));
    as->data.backup = (unsigned char *)calloc(as->base.size, sizeof(unsigned char));
    //内存池Map
    _amoled_mapInit(as, as->data.data, &as->data.dataMap);
    _amoled_mapInit(as, as->data.backup, &as->data.backupMap);
}

void _amoled_mapRelease(Amoled_Struct *as, amoledMapY **dataMap)
{
    int yC;
    amoledMapY *map;
    //
    map = *dataMap;
    //dataMap 指针准备
    if(as->base.displayDir == 0 || as->base.displayDir == 2)
    {
        for(yC = 0; yC < as->data.map_ySize; yC++)
            free(map[yC]);
    }
    else if(as->base.displayDir == 1 || as->base.displayDir == 3)
    {
        for(yC = 0; yC < as->data.map_xSize; yC++)
            free(map[yC]);
    }
    //
    free(map);
    *dataMap = NULL;
}

void amoled_mapRelease(Amoled_Struct *as)
{
    //内存池Map
    _amoled_mapRelease(as, &as->data.dataMap);
    _amoled_mapRelease(as, &as->data.backupMap);

    free(as->data.data);
    free(as->data.backup);

    as->data.data = NULL;
    as->data.backup = NULL;
}

//-------------------- 初始化 --------------------
//
int amoled_hardware_init(Amoled_Struct *as)
{
#if(!VIEW_CONFIG_MODE)
    return 0;
#else
    Amoled_Hardware *hware = &as->base.hware;

#if(AMOLED_USE_FRAMEBUFFER != 2)
    //生成fb节点
	hware->fdTemp = open(AMOLED_DEV_PATH_TEMP, O_RDWR);
	if(hware->fdTemp < 0)
	{	
		fprintf(stderr, "open lcdspi devPathTemp %s error!\n", AMOLED_DEV_PATH_TEMP);
	   // return -1;
	   hware->fdTemp = open(AMOLED_DEV_PATH_TEMP2, O_RDWR);
	    if(hware->fdTemp < 0)
	    {	
		    fprintf(stderr, "open lcdspi devPathTemp %s error!\n", AMOLED_DEV_PATH_TEMP2);
	        return -1;
	    }
	}
#endif

#if(AMOLED_USE_FRAMEBUFFER == 1)
    if (ioctl(hware->fdTemp, AMOLED_FB_INIT, NULL) == -1)
	{
        fprintf(stderr, "can't init frame buffer\r\n");
		close(hware->fdTemp);
		return -1;
	}
	close(hware->fdTemp);
	hware->fdTemp = 0;
#endif

#if(AMOLED_USE_FRAMEBUFFER)
	//获取fb映射
	hware->fd = open(AMOLED_DEV_PATH_FB, O_RDWR);
	if(hware->fd < 0)
	{	
		fprintf(stderr, "open lcdspi devPath %s error!\r\n", AMOLED_DEV_PATH_FB);
	    return -1;
	}
	//
    ioctl(hware->fd, FBIOGET_VSCREENINFO, &hware->fbSinf);
    printf("frame buffer info : %d x %d, %dbyte/%dbpp\r\n", 
        hware->fbSinf.xres, hware->fbSinf.yres, hware->fbSinf.bits_per_pixel/8, hware->fbSinf.bits_per_pixel);
    printf("frame buffer request : %d x %d, %dbyte/%dbpp\r\n", 
        as->base.xSize, as->base.ySize, as->base.perW, as->base.perW*8);
	if(hware->fbSinf.xres != as->base.xSize || 
       hware->fbSinf.yres != as->base.ySize || 
       hware->fbSinf.bits_per_pixel != as->base.perW*8)
	{
        as->base.xSize = hware->fbSinf.xres;
        as->base.ySize = hware->fbSinf.yres;
        as->base.perW = hware->fbSinf.bits_per_pixel/8;
        as->base.size = as->base.xSize*as->base.ySize*as->base.perW;
	    // close(hware->fd);
	    // return -1;
	}
	//
    hware->fb = (unsigned char *)mmap(0, as->base.size, PROT_READ|PROT_WRITE, MAP_SHARED, hware->fd, 0);
#else
    hware->fd = 0;
    hware->fb = 0;
#endif
    //
    return 0;
#endif
}

//自己保留总指针
Amoled_Struct *amoled_struct;

//
Amoled_Struct *amoled_malloc(int displayDir, int xSize, int ySize, int perW)
{
    Amoled_Struct *as;
    //
    if(xSize <=0 || ySize <= 0 || perW <= 0)
        return NULL;
    //基本参数设置
    as = (Amoled_Struct *)calloc(1, sizeof(Amoled_Struct));
    //displayDir
    if(displayDir <= 0 || displayDir > 3)
        as->base.displayDir = 0;
    else
        as->base.displayDir = displayDir;
    //
    as->base.xSize = xSize;
    as->base.ySize = ySize;
    as->base.perW = perW;
    as->base.size = as->base.xSize*as->base.ySize*as->base.perW;
    if(amoled_hardware_init(as) < 0)   //驱动初始化
    {
        free(as);
        return NULL;
    }
    //
    amoled_mapInit(as);
    //默认自动备份
    as->base.autoBackup = 1;
    //自己保留总指针
    amoled_struct = as;
    //
    return as;
}
//
void amoled_free(Amoled_Struct *as)
{
    if(as == NULL)
        return ;
    //fb close
    if(as->base.hware.fb)
    {
        munmap(as->base.hware.fb, as->base.size);
        close(as->base.hware.fd);
    }
    //fd close
    if(as->base.hware.fdTemp)
        close(as->base.hware.fdTemp);
    //
    amoled_mapRelease(as);
    //
    free(as);
    //as = NULL;
}

//-------------------- 刷新, 可擦除掉未保存至备份的数据 --------------------

void amoled_refresh(Amoled_Struct *as)
{
    //备份数据  覆盖  现有数据
    memcpy(as->data.data, as->data.backup, as->base.size);
}
//
void amoled_refresh_draw(Amoled_Struct *as)
{
    if(memcmp(as->data.data, as->data.backup, as->base.size)) //得不偿失
    {
        //备份数据  覆盖  现有数据
        memcpy(as->data.data, as->data.backup, as->base.size);

        //抗锯齿
#if(VIEW_CONFIG_ANTI_ALIASING)
        int xC = 0, yC = 0, xS = as->base.xSize, yS = as->base.ySize;
        int i;
        unsigned int temp1, temp2;
        for(yC = 1; yC < yS - 1; yC++)
        {
            for(xC = 1; xC < xS - 1; xC++)
            {
                for(i = 0; i < as->base.perW; i++)
                {
                    temp1 = as->data.backupMap[yC][xC][i]; 
                    temp2 = (as->data.backupMap[yC][xC+1][i] + as->data.backupMap[yC][xC-1][i] + 
                            as->data.backupMap[yC+1][xC][i] + as->data.backupMap[yC-1][xC][i])>>2;
                    as->data.dataMap[yC][xC][i] = (temp1>>1) + (temp2>>1);
                }
            }
        }
#endif
        amoled_draw(as);
    }
}
//
void amoled_backup(Amoled_Struct *as)
{
    //现有数据  覆盖  备份数据
    memcpy(as->data.backup, as->data.data, as->base.size);
}

//-------------------- 提供给view绘图的专用接口 --------------------

#define  XY_LOCAL_ERR(x,y)  ((x<0 || x>amoled_struct->data.map_xSize-1 || y<0 || y>amoled_struct->data.map_ySize-1) ? 0 : 1)

void amoled_print_dot(int x, int y, int data)
{
    // if(amoled_struct == NULL)
    //     return;
    if(XY_LOCAL_ERR(x, y))
    {
    //
#if(AMOLED_RGB_MODE == 1)
        amoled_struct->data.backupMap[y][x][0] = (unsigned char)(data&0xFF);
        data >>= 8;
        amoled_struct->data.backupMap[y][x][1] = (unsigned char)(data&0xFF);
        data >>= 8;
        amoled_struct->data.backupMap[y][x][2] = (unsigned char)(data&0xFF);
#else
        amoled_struct->data.backupMap[y][x][2] = (unsigned char)(data&0xFF);
        data >>= 8;
        amoled_struct->data.backupMap[y][x][1] = (unsigned char)(data&0xFF);
        data >>= 8;
        amoled_struct->data.backupMap[y][x][0] = (unsigned char)(data&0xFF);
#endif
    }
}

void amoled_print_dot2(int x, int y, int data, float alpha)
{
    // if(amoled_struct == NULL)
    //     return;
    if(XY_LOCAL_ERR(x, y))
    {
#if(AMOLED_RGB_MODE == 1)
        amoled_struct->data.backupMap[y][x][0] = 
            amoled_struct->data.backupMap[y][x][0]*alpha + ((unsigned char)(data&0xFF))*(1-alpha);
        data >>= 8;
        amoled_struct->data.backupMap[y][x][1] = 
            amoled_struct->data.backupMap[y][x][1]*alpha + ((unsigned char)(data&0xFF))*(1-alpha);
        data >>= 8;
        amoled_struct->data.backupMap[y][x][2] = 
            amoled_struct->data.backupMap[y][x][2]*alpha + ((unsigned char)(data&0xFF))*(1-alpha);
#else
        amoled_struct->data.backupMap[y][x][2] = 
            amoled_struct->data.backupMap[y][x][2]*alpha + ((unsigned char)(data&0xFF))*(1-alpha);
        data >>= 8;
        amoled_struct->data.backupMap[y][x][1] = 
            amoled_struct->data.backupMap[y][x][1]*alpha + ((unsigned char)(data&0xFF))*(1-alpha);
        data >>= 8;
        amoled_struct->data.backupMap[y][x][0] = 
            amoled_struct->data.backupMap[y][x][0]*alpha + ((unsigned char)(data&0xFF))*(1-alpha);
#endif
    }
}

void amoled_print_en(void)
{
    // if(amoled_struct == NULL)
    //     return;
    //备份数据覆盖现有数据并输出
    amoled_refresh_draw(amoled_struct);
}

void amoled_displayDir(int displayDir)
{
    if(amoled_struct == NULL || amoled_struct->base.displayDir == displayDir)
        return ;
    //内存池Map
    _amoled_mapRelease(amoled_struct, &amoled_struct->data.dataMap);
    _amoled_mapRelease(amoled_struct, &amoled_struct->data.backupMap);
    //displayDir
    if(displayDir <= 0 || displayDir > 3)
        amoled_struct->base.displayDir = 0;
    else
        amoled_struct->base.displayDir = displayDir;
    //内存池Map
    _amoled_mapInit(amoled_struct, amoled_struct->data.data, &amoled_struct->data.dataMap);
    _amoled_mapInit(amoled_struct, amoled_struct->data.backup, &amoled_struct->data.backupMap);
}

// 0/初始化(亮屏), 1/亮屏, 2/灭屏, 3/wakeup, 4/sleep, 5/powerOff, 6/刷新屏幕
void amoled_modeSet(int mode)
{
#if(VIEW_CONFIG_MODE && !AMOLED_USE_FRAMEBUFFER)
    long m = mode;
    if(amoled_struct)
        ioctl(amoled_struct->base.hware.fdTemp, AMOLED_WMODE, (void *)m);
#else
    if(amoled_struct && (mode == 2 || mode == 4 || mode == 5))
    {
        amoled_print_clear(0);
        amoled_print_en();
    }
#endif
}

// 0 ~ 0xFF
void amoled_brightSet(unsigned char bright)
{
#if(VIEW_CONFIG_MODE && !AMOLED_USE_FRAMEBUFFER)
    long m = bright;
    if(amoled_struct)
        ioctl(amoled_struct->base.hware.fdTemp, AMOLED_BRIGHT, (void *)m);
#endif
}

//
void amoled_print_clear(int value)
{
    int xC = 0, yC = 0;
    unsigned char R = (unsigned char)((value&0xFF0000)>>16);
    unsigned char G = (unsigned char)((value&0x00FF00)>>8);
    unsigned char B = (unsigned char)(value&0x0000FF);
    for(yC = 0; yC < amoled_struct->base.ySize; yC++)
    {
        for(xC = 0; xC < amoled_struct->base.xSize; xC++)
        {
#if(AMOLED_RGB_MODE == 1)
            amoled_struct->data.backupMap[yC][xC][0] = B;
            amoled_struct->data.backupMap[yC][xC][1] = G;
            amoled_struct->data.backupMap[yC][xC][2] = R;
#else
            amoled_struct->data.backupMap[yC][xC][0] = R;
            amoled_struct->data.backupMap[yC][xC][1] = G;
            amoled_struct->data.backupMap[yC][xC][2] = B;
#endif
        }
    }
}



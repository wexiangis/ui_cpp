

#ifndef _AMOLED_H
#define _AMOLED_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <linux/fb.h>

#include "viewConfig.h"

//设备地址
#define	AMOLED_DEV_PATH_FB  "/dev/fb0"
#define	AMOLED_DEV_PATH_TEMP  "/dev/amoled_spi1.0"
#define	AMOLED_DEV_PATH_TEMP2  "/dev/amoled_spi32766.0"

//使用frame buffer模式  0/不起用  1/使用  2/4412平台
#define AMOLED_USE_FRAMEBUFFER       0

//调整像素输出顺序(假设来源是RGB)  0/RGB, 1/BGR
#define AMOLED_RGB_MODE     0

//即写即输出
#define    AMOLED_REFRESH_ON_TIME       0

typedef struct{
    int fd;
    int fdTemp;
    pthread_t th;
    unsigned char *fb;
    struct fb_var_screeninfo fbSinf;
}Amoled_Hardware;

typedef struct{
    Amoled_Hardware hware;
    int displayDir;
    int xSize;
    int ySize;
    int perW;
    int size;
    int autoBackup;
}Amoled_Base;

typedef unsigned char *amoledMapX; //[3]
typedef amoledMapX *amoledMapY;    //[X][3]
typedef amoledMapY *amoledMap;     //[Y][X][3]

typedef struct{
    unsigned char *data;   // 刷屏缓冲区
    amoledMapY *dataMap;   // 初始化后通过 dataMap[y][x][3] 直接定位到像素点

    unsigned char *backup; // 备份
    amoledMapY *backupMap; // 初始化后通过 backupMap[y][x][3] 直接定位到像素点

    int map_xSize, map_ySize;   //
}Amoled_Data;

typedef struct{
    Amoled_Base base;
    Amoled_Data data;
}Amoled_Struct;

//----- 用户方法 -----

#define AS_EN_BACKUP(x)   (x->base.autoBackup = 1)    // 自动备份到缓存
#define AS_DIS_BACKUP(x)  (x->base.autoBackup = 0)

Amoled_Struct *amoled_malloc(int displayDir, int xSize, int ySize, int perW);
void amoled_free(Amoled_Struct *as);

void amoled_draw(Amoled_Struct *as);
void amoled_refresh(Amoled_Struct *as);
void amoled_refresh_draw(Amoled_Struct *as);
void amoled_backup(Amoled_Struct *as);

//----- 提供给view绘图的专用接口 -----
void amoled_print_dot(int x, int y, int data);
void amoled_print_dot2(int x, int y, int data, float alpha);
void amoled_print_clear(int value);
void amoled_print_en(void);
void amoled_displayDir(int displayDir);
void amoled_modeSet(int mode);
void amoled_brightSet(unsigned char bright);

#ifdef __cplusplus
};
#endif

#endif


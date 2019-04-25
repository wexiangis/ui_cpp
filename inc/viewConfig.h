
#ifndef _VIEWCONFIG_H
#define _VIEWCONFIG_H

//0/ubuntu调试  1/带触屏,充电,usb识别硬件调试  2/整机硬件调试  3/4412平台
#define VIEW_CONFIG_MODE   0

//==================== 屏幕参数 ====================
#define  VIEW_X_START    0
#define  VIEW_Y_START    0

#if(VIEW_CONFIG_MODE == 3)
#define  VIEW_X_SIZE    800
#define  VIEW_Y_SIZE    480
#define  VIEW_X_END     799
#define  VIEW_Y_END     479
#define  VIEW_PERW      4
#else
#define  VIEW_X_SIZE    500
#define  VIEW_Y_SIZE    500
#define  VIEW_X_END     (VIEW_X_SIZE-1)
#define  VIEW_Y_END     (VIEW_Y_SIZE-1)
#define  VIEW_PERW      3
#endif
#define  VIEW_MEM_SIZE  (VIEW_X_SIZE*VIEW_Y_SIZE*VIEW_PERW)
#define  VIEW_RESOLUTION  (VIEW_X_SIZE>VIEW_Y_SIZE?VIEW_Y_SIZE:VIEW_X_SIZE) //参考尺寸

//保存每帧图像 VIEW_CONFIG_MODE = 0时可用
#define VIEW_CONFIG_FRAME  0

//抗锯齿
#define VIEW_CONFIG_ANTI_ALIASING  0

//字库类型选择 0/gbk 1/ttf
#define VIEW_CONFIG_PRINT_LIB  1

//根据当前屏幕大小 得出推荐字号
#if(VIEW_CONFIG_PRINT_LIB)
#define  VIEW_RESOLUTION_PRINT  (VIEW_RESOLUTION/10) //字体大小
#else
#define  VIEW_RESOLUTION_PRINT  24  //(VIEW_RESOLUTION/10) //字体大小
#endif

//src
#if(VIEW_CONFIG_MODE)
#define VIEW_CONFIG_SRC_ROOT    "/usr/local/qbox10/source"
#else

#define VIEW_CONFIG_SRC_ROOT    "./lib"
#endif

//gbk
#define VIEW_CONFIG_GBK_ROOT  VIEW_CONFIG_SRC_ROOT"/gbk"

//ttf
#define VIEW_CONFIG_TTF_ROOT  VIEW_CONFIG_SRC_ROOT"/ttf"

//pic
#define VIEW_CONFIG_PIC_ROOT  VIEW_CONFIG_SRC_ROOT"/picSource"

#endif

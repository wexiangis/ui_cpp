
#ifndef _GBK2312_H
#define _GBK2312_H

#ifdef __cplusplus
extern "C" {
#endif

#include "viewConfig.h"

#if(VIEW_CONFIG_PRINT_LIB == 0)

// #define GBK_FILE_ROOT  "./lib/gbk"
// #define GBK_FILE_ROOT  "/usr/local/qbox10/source/gbk"
#define GBK_FILE_ROOT  VIEW_CONFIG_GBK_ROOT

//160~169
#define  FILEPATH_GBK16_0  GBK_FILE_ROOT"/wryh1616.DZK" //微软雅黑16
#define  FILEPATH_GBK16_1  GBK_FILE_ROOT"/wryhc1616.DZK"//微软雅黑 粗体 16
//200~209
#define  FILEPATH_GBK20_0  GBK_FILE_ROOT"/wryh2020.DZK" //微软雅黑20
#define  FILEPATH_GBK20_1  GBK_FILE_ROOT"/wryhc2020.DZK"//微软雅黑 粗体 20
//240~249
#define  FILEPATH_GBK24_0  GBK_FILE_ROOT"/wryh2424.DZK" //微软雅黑 24
#define  FILEPATH_GBK24_1  GBK_FILE_ROOT"/wryhc2424.DZK"//微软雅黑 粗体 24
//280~289
#define  FILEPATH_GBK28_0  GBK_FILE_ROOT"/wryh2828.DZK" //微软雅黑28
#define  FILEPATH_GBK28_1  GBK_FILE_ROOT"/wryhc2828.DZK"//微软雅黑 粗体 28
//320~329
#define  FILEPATH_GBK32_0  GBK_FILE_ROOT"/wryh3232.DZK" //微软雅黑 32
#define  FILEPATH_GBK32_1  GBK_FILE_ROOT"/wryhc3232.DZK"//微软雅黑 粗体 32
//400~409
#define  FILEPATH_GBK40_0  GBK_FILE_ROOT"/wryh4040.DZK" //微软雅黑 40
#define  FILEPATH_GBK40_1  GBK_FILE_ROOT"/wryhc4040.DZK"//微软雅黑 粗体 40
//480~489
#define  FILEPATH_GBK48_0  GBK_FILE_ROOT"/wryh4848.DZK" //微软雅黑 48
//560~569
#define  FILEPATH_GBK56_0  GBK_FILE_ROOT"/wryh5656.DZK" //微软雅黑 56
//640~649
#define  FILEPATH_GBK64_0  GBK_FILE_ROOT"/wryh6464.DZK" //微软雅黑 64
//NULL
#define  FILEPATH_NULL     GBK_FILE_ROOT"/null.dot"

//传入字符串,识别并返回第一个字或字符的点阵数据
// utf8_code : 传入字符串,例如: gbk_getBitMapByUtf8("字符A", ...);
// buf : 用来接收点阵数据的指针
// bufLen : buf可用长度
// type : 字体代号, 例如: 160,161,200,201,240,241... 前面两位数代表点阵大小,后面1表示使用粗体,160表示16x16点阵
//返回: -1/失败  其它/识别并使用掉的字节数,通常ascii是1字节,中文是3字节
int gbk_getBitMapByUtf8(unsigned char *utf8_code, unsigned char *buf, unsigned int *bufLen, int type);

//接下来要输出的字或字符的宽度(像素)
void gbk_getRangeByUtf8_multiLine(unsigned char *utf8_code, int type, int xEdge, int yEdge, int widthLimit, int *retW, int *retH);
int gbk_getRangeByUtf8(unsigned char *utf8_code, int type, int xEdge, int *retH);

#endif //VIEW_CONFIG_PRINT_LIB

#ifdef __cplusplus
};
#endif

#endif


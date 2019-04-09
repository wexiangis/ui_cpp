
#ifndef _TOUCH_H
#define _TOUCH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <stdbool.h>

#define  TOUCH_ENABLE   1

//触摸屏坐标数据输出调整
#define TOUCH_X_MIRROR  0   //镜像X数值
#define TOUCH_Y_MIRROR  1   //镜像Y数值

typedef void (*TouchCallBack)(void *own, void *privateData);

typedef struct{
    //
    int xSize;
    int ySize;
    //
    pthread_t th;
    //原始数据
    int id;     //本次事件的ID
    int xy[2];      //当前坐标
    int downXy[2];  //记录按下时的坐标
    int xyErr[2];   //当前坐标与按下时的坐标的偏差
    long time[2];   //记录down时的时间(sec, usev), 与up时的时间比较, 判断形成长短按的事件
    long currentTime[2];   //当前系统时间
    int touchEventCount;   //用来判断按下(down)事件的临时变量
    //用户可能用到的数据
    bool onTouch;     //output    //按下状态?
    bool onMov;       //output    //拖动状态?
    long timeErrMs;   //output    //当前触发事件距按下(down)事件的时间差, 可用于判断长/短按
    int screenDir;    //input     //根据需要, 动态修改屏幕倒向  0/正  1/右  2/倒  3/左  其它/正
    int xyReal[2];    //output    //screenDir 矫正后的 xy[]
    int downXyReal[2];//output    //screenDir 矫正后的 downXy[]
    int xyErrReal[2]; //output    //screenDir 矫正后的 xyErr[]
    int xyErrMatchThreshold;//input     //比较xyErr的起始值(像素), 小于该值算作无拖动, 默认值为10
    int xyErrMatchResult;   //output    //通过比较xyErr的绝对值, 得出当前是在往哪个方向拖动  0/无拖动  1/上拖  2/右拖  3/下拖  4/左拖
    int xyErrMatchResultMax;
    int xyErrMatchValue;    //output    //拖动了多少像素, xyErr的绝对值
    int xyErrMatchValueMax; //output    //xyErrMatchValue 在一个触屏周期内的最大值
    int xyErrMatchDivideValue;    //output    //拖动方向上, 相对上一点的偏移量, 分正负
    //
    bool downFlag;  //以确定拿到完整的坐标数值后才触发down事件
    //
    void *privateData;
    TouchCallBack downCallBack;
    TouchCallBack upCallBack;
    TouchCallBack movCallBack;
}Touch_Struct;

int touch_init(int screenDir, int xSize, int ySize, void *privateData, TouchCallBack upCallBack, TouchCallBack downCallBack, TouchCallBack movCallBack);

#ifdef __cplusplus
};
#endif

#endif


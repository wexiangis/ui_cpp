#ifndef _VIEW_H_
#define _VIEW_H_

#include "viewType.h"
#include "viewPen.h"

class View
{
public:

    View();

private:
    
    //----- 链表参数 (父级 -----
    View *parent;
    //----- 链表参数 (同级 -----
    View *last, *next;
    int order; //序号 从1数起
    //----- 链表参数 (子级 -----
    View *child, *lastChild; //child链表的一头一尾
    int total; //child的总数

    //----- 位置参数 -----
    int width, height; //实际宽高
    int range[2][2]; //输出范围

};

#endif

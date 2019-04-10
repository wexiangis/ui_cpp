#ifndef _VIEWTYPE_HPP_
#define _VIEWTYPE_HPP_

#include <typeinfo>
#include <iostream>

template <typename AnyType>

class ViewType{

    public:

        //构造函数
        explicit ViewType(AnyType v) //一个参数的构造函数 加explicit防止隐式定义
        {
            ISARRAY = false;
            LENGHT = 1;
            VALUE = new AnyType[1];
            *VALUE = v;
        }
        ViewType(AnyType *v, int count)
        {
            ISARRAY = true;
            LENGHT = count;
            VALUE = new AnyType[count];
            for(int  i = 0; i < count; i++)
                VALUE[i] = v[i];
        }
        //析构函数
        ~ViewType()
        {
            ISARRAY = false;
            LENGHT = 0;
            if(VALUE)
            {
                delete[] VALUE;
                VALUE = NULL;
            }
        }
        //基本参数获取
        bool isArray(){return ISARRAY;}
        int lenght(){return LENGHT;}
        //读数据
        AnyType get(int count = 0)
        {
            if(VALUE && count < LENGHT)
                return VALUE[count];
            return 0;
        }
        AnyType* getN(int index, int count)
        {
            if(VALUE && index + count <= LENGHT)
            {
                AnyType *ret = new AnyType[count];
                for(int i = 0, j = index; i < count; i++, j++)
                    ret[i] = VALUE[j];
                return ret;
            }
            return NULL;
        }
        AnyType* getAll()
        {
            if(VALUE)
            {
                AnyType *ret = new AnyType[LENGHT];
                for(int i = 0; i < LENGHT; i++)
                    ret[i] = VALUE[i];
                return ret;
            }
            return NULL;
        }
        //修改数据
        void set(AnyType v, int count = 0)
        {
            if(VALUE && count < LENGHT)
                VALUE[count] = v;
            else if(!VALUE && count == 0)
            {
                VALUE = new AnyType[1];
                VALUE[0] = v;
                LENGHT = 1;
            }
        }
        void set(AnyType *v, int count)
        {
            if(VALUE)
                delete[] VALUE;
            //
            ISARRAY = true;
            LENGHT = count;
            VALUE = new AnyType[count];
            for(int  i = 0; i < count; i++)
                VALUE[i] = v[i];
        }
        //删除数据
        int del(int count = -1)//默认删除最后一个
        {
            if(VALUE && count < LENGHT)
            {
                int i;
                if(count < 0)
                    i = LENGHT - 1;
                else
                    i = count + 1;
                for(; i < LENGHT; i++)
                    VALUE[i-1] = VALUE[i];
                // VALUE[i] = 0;
                LENGHT -= 1;
                if(LENGHT < 1)
                {
                    delete[] VALUE;
                    VALUE = NULL;
                    LENGHT = 0;
                }
            }
            return LENGHT;
        }
        //添加数据
        int add(AnyType v)
        {
            AnyType *temp = new AnyType[LENGHT+1];
            int  i = 0;
            for(; i < LENGHT; i++)
                temp[i] = VALUE[i];
            temp[i] = v;
            //
            if(VALUE)
                delete[] VALUE;
            VALUE = temp;
            LENGHT += 1;
            ISARRAY = true;
            return LENGHT;
        }
        int add(AnyType *v, int count)
        {
            AnyType *temp = new AnyType[LENGHT+count];
            int  i = 0, j = 0;
            for(; i < LENGHT; i++)
                temp[i] = VALUE[i];
            for(; j < count; i++, j++)
                temp[i] = v[j];
            //
            if(VALUE)
                delete[] VALUE;
            VALUE = temp;
            LENGHT += count;
            ISARRAY = true;
            return LENGHT;
        }
        //插入数据
        int insert(AnyType v, int index = -1)//安插到index的前面
        {
            if(index > LENGHT - 1)
                return LENGHT;
            else if(index < 0)
                return add(v);
            AnyType *temp = new AnyType[LENGHT+1];
            int  i = 0, j = 0;
            for(; i < LENGHT; i++, j++)
            {
                if(i == index)
                    temp[j++] = v;
                temp[j] = VALUE[i];
            }
            //
            if(VALUE)
                delete[] VALUE;
            VALUE = temp;
            LENGHT += 1;
            ISARRAY = true;
            return LENGHT;
        }

    private:
    
        bool ISARRAY;
        AnyType *VALUE;
        int LENGHT;
};

#endif

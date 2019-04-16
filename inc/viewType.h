#ifndef _VIEWTYPE_H_
#define _VIEWTYPE_H_

#include <typeinfo>
#include <iostream>

template <typename ATP>

//注意: 使用模板的类,被继承时需指定类型(等于失去了"任意类型")
//例如: class A : AnyType<int>
class AnyType{

    public:

        //----- 构造函数 -----
        AnyType(std::string name, ATP v):NAME(name)
        {
            ISARRAY = false;
            LENGHT = 1;
            VALUE = new ATP[1];
            *VALUE = v;
        }
        AnyType(std::string name, ATP *v, int count):NAME(name)
        {
            ISARRAY = true;
            LENGHT = count;
            VALUE = new ATP[count];
            for(int  i = 0; i < count; i++)
                VALUE[i] = v[i];
        }

        //----- 析构函数 -----
        ~AnyType()
        {
            ISARRAY = false;
            LENGHT = 0;
            if(VALUE)
            {
                delete[] VALUE;
                VALUE = NULL;
            }
        }
        
        //----- 读数据 -----
        ATP get(int count = 0)
        {
            if(VALUE && count < LENGHT)
                return VALUE[count];
            return 0;
        }
        ATP* getN(int index, int count)
        {
            if(VALUE && index + count <= LENGHT)
            {
                ATP *ret = new ATP[count];
                for(int i = 0, j = index; i < count; i++, j++)
                    ret[i] = VALUE[j];
                return ret;
            }
            return NULL;
        }
        ATP* getAll()
        {
            if(VALUE)
            {
                ATP *ret = new ATP[LENGHT];
                for(int i = 0; i < LENGHT; i++)
                    ret[i] = VALUE[i];
                return ret;
            }
            return NULL;
        }

        //----- 修改数据 -----
        void set(ATP v, int count = 0)
        {
            if(VALUE && count < LENGHT)
                VALUE[count] = v;
            else if(!VALUE && count == 0)
            {
                VALUE = new ATP[1];
                VALUE[0] = v;
                LENGHT = 1;
            }
        }
        void set(ATP *v, int count)
        {
            if(VALUE)
                delete[] VALUE;
            //
            ISARRAY = true;
            LENGHT = count;
            VALUE = new ATP[count];
            for(int  i = 0; i < count; i++)
                VALUE[i] = v[i];
        }

        //----- 删除数据 -----
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

        //----- 添加数据 -----
        int add(ATP v)
        {
            ATP *temp = new ATP[LENGHT+1];
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
        int add(ATP *v, int count)
        {
            ATP *temp = new ATP[LENGHT+count];
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
        
        //----- 插入数据 -----
        int insert(ATP v, int index = -1)//安插到index的前面
        {
            if(index > LENGHT - 1)
                return LENGHT;
            else if(index < 0)
                return add(v);
            ATP *temp = new ATP[LENGHT+1];
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

        //----- 基本参数获取 -----
        std::string name(){return NAME;};
        bool isArray(){return ISARRAY;}
        int lenght(){return LENGHT;}

    private:

        std::string NAME;
        bool ISARRAY;
        ATP *VALUE;
        int LENGHT;
};

#endif

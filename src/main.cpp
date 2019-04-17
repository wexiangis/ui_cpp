
#include <unistd.h>

#include "viewPen.h"
#include "viewPicture.h"
#include <iostream>

#define TEST_MODE 1

#if(TEST_MODE == 1)

//
int main(int argc, char **argv)
{
    ViewPicture pic("./res/jpeg/test.jpg");
    if(!pic.ready())
    {
        std::cout<<"pic init err !"<<std::endl;
        return -1;
    }

    int resW = pic.width(), resH = pic.height();
    unsigned char *res = pic.get_mem(resW, resH, NULL, NULL, 0);

    ViewPen pen(0);

    pen.clear(0xFF0000);

    while(1)
    {
        pen.print_map(0, 0, resW, resH, res, 0);
        pen.refresh();
        sleep(1);
    }

    return 0;
}

#elif(TEST_MODE == 0)
//
int main(int argc, char **argv)
{

    AnyType<string> vt13("vt13", new string[5]{"cdf", "234", "jjj", "kkk", "ddd"}, 5);

    cout<<"class name: "<<vt13.name()<<endl;

    cout<<vt13.get()<<", "<<vt13.get(1)<<endl;

    string *ret = vt13.getAll();
    cout<<ret[0]<<", "<<ret[1]<<", "<<ret[2]<<endl;
    delete[] ret;

    vt13.del(2);
    vt13.insert("bbb", 2);

    string *ret2 = vt13.getN(1, 3);
    cout<<ret2[0]<<", "<<ret2[1]<<", "<<ret2[2]<<endl;
    delete[] ret2;

    return 0;
}

#endif

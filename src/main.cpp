#include "viewType.hpp"

#include <iostream>
using namespace std;

//
int main(int argc, char **argv)
{
    ViewType<bool> vt0(true);
    ViewType<bool> vt1(new bool[3]{true, false, false}, 3);
    ViewType<int> vt2(10);
    ViewType<int> vt3(new int[3]{10, 9, 8}, 3);
    ViewType<char> vt4('6');
    ViewType<double> vt8(10.098);
    ViewType<float> vt10(10.098);
    ViewType<string> vt12("abc");
    ViewType<string> vt13(new string[5]{"cdf", "234", "jjj", "kkk", "ddd"}, 5);

    cout<<vt13.get()<<", "<<vt13.get(1)<<endl;

    string *ret = vt13.getN();
    cout<<ret[0]<<", "<<ret[1]<<", "<<ret[2]<<endl;
    delete[] ret;

    vt13.del(2);
    vt13.insert("bbb", 2);

    string *ret2 = vt13.getN(1, 3);
    cout<<ret2[0]<<", "<<ret2[1]<<", "<<ret2[2]<<endl;
    delete[] ret2;

    return 0;
}

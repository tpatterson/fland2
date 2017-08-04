
using namespace std;

#include <iostream>
#include "tyler.h"
#include "tyler.cpp"
#include "colorclass.h"
#include "colorclass.cpp"


int main(){
    cout << "hi!"<< endl;

    ColorClass cc;
    cc.color.red = .2;
    cc.color.green = .5;
    cc.color.blue = .3;
    cout << cc.toString(4);
    
    cout << cc.getColor().toString(8);
    cout << cc.getColor().toString(8);
    cout << cc.getColor().toString(8);


    cout << "\n";
    return 0;
}


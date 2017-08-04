


#ifndef COLORCLASS_CPP
#define COLORCLASS_CPP

#include "tyler.h"
#include "colorclass.h"
#include <string>

ColorClass::ColorClass(){
    valueShift = .2;
    saturationShift = .2;
}

Color ColorClass::getColor(){
    Color col;
    col.red = color.red;
    col.green = color.green;
    col.blue = color.blue;

    float valShift = valueShift * (rnd() - 0.5);
    valShift += 1.0;
    //float satShift = saturationShift * (rnd() - 0.5);

    col.red *= valShift;
    col.green *= valShift;
    col.blue *= valShift;
    
    clamp(col.red, (float)0.0, (float)1.0);
    clamp(col.green, (float)0.0, (float)1.0);
    clamp(col.blue, (float)0.0, (float)1.0);

    return col;
}

string ColorClass::toString( int depth ){
    ostringstream out;
    string ind = indent( depth );
    out
        << ind << "ColorClass::\n"
        << color.toString(depth)
        << ind << "valueShift       " << valueShift << endl
        << ind << "satShift         " << saturationShift << endl
        << endl
    ;
    return out.str();
}

#endif


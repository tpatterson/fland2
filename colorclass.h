#ifndef COLORCLASS_H
#define COLORCLASS_H

#include "tyler.h"

#include <string>
#include <sstream>

class ColorClass
{
    public:
    
    Color color;
    float valueShift, saturationShift;

    ColorClass();

    Color getColor();

    string toString( int depth );

};
 
#endif


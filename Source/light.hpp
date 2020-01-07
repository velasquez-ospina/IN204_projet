#ifndef _LIGHT_HPP
#define _LIGHT_HPP

#include <iostream>
#include "Vect.hpp"
#include "Color.hpp"
class light{
    Vect position;
    Color color;



    public:
    light():position(0,0,0), color(1,1,1,0){}
    light(Vect aposition): position(aposition), color(1,1,1,0){}
    light(Vect aposition, Color acolor): position(aposition), color(acolor){}
    ~light(){}

    Vect getLightPosition() {return position;}
    Color getLightColor() {return color;}

};
#endif // _LIGHT_HPP
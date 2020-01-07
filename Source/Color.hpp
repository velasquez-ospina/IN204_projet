#ifndef _COLOR_HPP
#define _COLOR_HPP

#include <iostream>


class Color{
    double red, green, blue, special; //the special value will represent reflectiveness of the surface

    public:

    //constructors
    Color():red(0.5),green(0.5),blue(0.5){}
    Color(double ared, double agreen, double ablue, double aspecial):red(ared), green(agreen), blue(ablue), special(aspecial){}

    //get functions
    double getColorRed() {return red;}
    double getColorGreen() {return green;}
    double getColorBlue() {return blue;}
    double getColorSpecial() {return special;}

    //set functions
    double setColorRed(double ared) {red = ared;}
    double setColorGreen(double agreen) {green = agreen;}
    double setColorBlue(double ablue) {blue = ablue;}
    double setColorSpecial(double aspecial) {special = aspecial;}

};
#endif // _COLOR_HPP
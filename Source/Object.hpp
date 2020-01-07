#ifndef _OBJECT_HPP
#define _OBJECT_HPP

#include <iostream>
#include "Vect.hpp"
#include "Ray.hpp"
#include "Color.hpp"



class Object{
    public:

    Object(){}
    ~Object(){}


    //Method functions
    Color getObjectColor () {return Color ( 0.0, 0.0, 0.0, 0);} // this is meant for shadows
    
    double findIntersection(Ray ray) {
        return 0;
    }

};
#endif //_OBJECT_HPP
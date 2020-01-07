#ifndef _SPHERE_HPP
#define _SPHERE_HPP

#include <iostream>
#include "Vect.hpp"
#include "Color.hpp"
#include "Object.hpp"



class Sphere: public Object{
    Vect center;
    double radius;
    Color color;

    public:
        Vect getSphereCenter(){return center;}
        double getSphereRadius(){return radius;}
        Color getSphereColor(){return color;}

        ~Sphere(){}
        Sphere():center(0,0,0), radius(1), color(0.5,0.5,0.5,0){}
        Sphere(Vect aCenter):center(aCenter), radius(1.0), color(0.5,0.5,0.5,0){}
        Sphere(Vect aCenter, double aRadius):center(aCenter), radius(aRadius), color(1,1,1,0){}
        Sphere(Vect aCenter, double aRadius, Color aColor):center(aCenter), radius(aRadius), color(aColor){}

};


#endif //_SPHERE_HPP
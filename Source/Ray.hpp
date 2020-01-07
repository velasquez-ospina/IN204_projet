#ifndef _RAY_HPP
#define _RAY_HPP

#include <iostream>
#include "Vect.hpp"
class Ray{
    
    private:

        Vect origin, direction;

    public:

        //Constructors
        Ray():origin(0,0,0), direction(1,0,0){}
        Ray(Vect adirection): origin(0,0,0), direction(adirection){}
        Ray(Vect adirection, Vect anorigin): origin(anorigin), direction(adirection){}
        ~Ray(){}

        //Get functions
        Vect getRayOrigin() {return origin;}
        Vect getRayVector() {return direction;}




};
#endif //_RAY_HPP
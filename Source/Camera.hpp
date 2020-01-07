#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include <iostream>
#include "Vect.hpp"

class Camera{
    Vect campos, camdir, camright, camdown; 
    public:

    Camera():campos(0,0,0), camdir(0,0,1), camright(0,0,0), camdown(0,0,0){}
    Camera(Vect acampos):campos(acampos), camdir(1,0,0), camright(0,0,0), camdown(0,0,0){}
    Camera(Vect acampos, Vect acamdir):campos(acampos), camdir(acamdir), camright(0,0,0), camdown(0,0,0){}
    Camera(Vect acampos, Vect acamdir, Vect acamright):campos(acampos), camdir(acamdir), camright(acamright), camdown(0,0,0){}
    Camera(Vect acampos, Vect acamdir, Vect acamright, Vect acamdown):campos(acampos), camdir(acamdir), camright(acamright), camdown(acamdown){}
    ~Camera(){}

    Vect getCameraPosition() {return campos;}
    Vect getCameraDirection() {return camdir;}
    Vect getcameraRight() {return camright;}
    Vect getCameraDown() {return camdown;}

};
#endif //_CAMERA_HPP
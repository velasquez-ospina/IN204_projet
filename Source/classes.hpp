#include <iostream>

class point3d{
    float px;
    float py;
    float pz;

    point3d();
    point3d(float x): px(x){}
    point3d(float x, float y): px(x), py(y){}
    point3d(float x, float y, float z): px(x), py(y), pz(z){}
    ~point3d();

};
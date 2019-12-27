#include <iostream>

class Vect{
    double x, y, z;

    public:
    Vect():x(0), y(0), z(0){}
    Vect(double px): x(px), y(0), z(0){}
    Vect(double px, double py): x(px), y(py), z(0){}
    Vect(double px, double py, double pz): x(px), y(py), z(pz){}

    ~Vect();

    //get functions
    double getVectX() {return x;}
    double getVectY() {return y;}
    double getVectZ() {return z;}

    double magnitude (){ return sqrt(x*x + y*y + z*z);}

    Vect normalize () {
        double magnitude = sqrt(x*x + y*y + z*z);
        return Vect (x/magnitude,y/magnitude,z/magnitude);
    }

    Vect negative () { return Vect (-x, -y, -z);}

    double dotProduct(Vect v) {
        return x*v.getVectX() + y*v.getVectY() + z*v.getVectZ();
    }

    Vect crossproduct(Vect v){
        return Vect (y*v.getVectZ()- z*v.getVectY(), - x*v.getVectZ() + z*v.getVectX(), x*v.getVectY() - y*v.getVectX());   }

    Vect vectAdd (Vect v) {
        return Vect (x + v.getVectX(), y + v.getVectY(), z + v.getVectZ() );
    }

    Vect VectMul (double scalar) {
        return Vect(x*scalar, y*scalar, z*scalar);
    }

};

class Ray{
    Vect origin, direction;


    public:
    Ray():origin(0,0,0), direction(1,0,0){}
    Ray(Vect adirection): origin(0,0,0), direction(adirection){}
    Ray(Vect adirection, Vect anorigin): origin(anorigin), direction(adirection){}
    ~Ray();

    Vect getRayOrigin() {return origin;}
    Vect getRayVector() {return direction;}




};

class Camera{
    Vect campos, camdir, camright, camdown; 
    public:

    Camera():campos(0,0,0), camdir(0,0,1), camright(0,0,0), camdown(0,0,0){}
    Camera(Vect acampos):campos(acampos), camdir(1,0,0), camright(0,0,0), camdown(0,0,0){}
    Camera(Vect acampos, Vect acamdir):campos(acampos), camdir(acamdir), camright(0,0,0), camdown(0,0,0){}
    Camera(Vect acampos, Vect acamdir, Vect acamright):campos(acampos), camdir(acamdir), camright(acamright), camdown(0,0,0){}
    Camera(Vect acampos, Vect acamdir, Vect acamright, Vect acamdown):campos(acampos), camdir(acamdir), camright(acamright), camdown(acamdown){}
    ~Camera();

    Vect getCameraPosition() {return campos;}
    Vect getCameraDirection() {return camdir;}
    Vect getcameraRight() {return camright;}
    Vect getCameraDown() {return camdown;}

};

class color{
    double red, green, blue, special; //the special value will represent reflectiveness of the surface

    public:

    //constructors
    color():red(0.5),green(0.5),blue(0.5){}
    color(double ared, double agreen, double ablue, double aspecial):red(ared), green(agreen), blue(ablue), special(aspecial){}

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

class light{
        Vect position;
        color Color;



    public:
    light():position(0,0,0), Color(1,1,1,0){}
    light(Vect aposition): position(aposition), Color(1,1,1,0){}
    light(Vect aposition, color acolor): position(aposition), Color(acolor){}
    ~light();

    Vect getLightPosition() {return position;}
    color getLightColor() {return Color;}

};

struct RGBType{
    double r;
    double g;
    double b;

    
};

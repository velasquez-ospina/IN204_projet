#ifndef _PLANE_HPP
#define _PLANE_HPP

#include "math.h"
#include "Object.hpp"

class Plane : public Object {

    private:

        Vect normal;
        double distance;
        Color color;

    public:

        //Constructors
        Plane():normal(1,0,0), distance(1.0), color(0,0,0,0){};
        Plane(Vect aNormal): normal(aNormal), distance(1.0), color(0,0,0,0){}
        Plane(Vect aNormal, double aDistance): normal(aNormal), distance(aDistance), color(0,0,0,0){}
        Plane(Vect aNormal, double aDistance, Color aColor): normal(aNormal), distance(aDistance), color(aColor){}
        ~Plane(){}

        //Get functions
        Vect getPlaneNormal () {return normal;}
        double getPlaneDistance () {return distance;}
        Color getPlaneColor () {return color;}

        //Method functions
        Vect getNormalAt (Vect point){
                return normal;
        }
        double findIntersection(Ray ray) {
            Vect ray_direction = ray.getRayVector();
            double a = ray_direction.dotProduct(normal);
            if (a== 0) {
                //ray is parallel to the plane
                return -1;
            }
            else{
                double b = normal.dotProduct(ray.getRayOrigin().vectAdd(normal.vectMul(distance).negative()));
                return -1*b/a;
            }
        }
};



#endif // _PLANE_HPP
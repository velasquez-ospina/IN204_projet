#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

//For more information about the function normalize_pixels
//https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays


//Explicit version
/*
void normalize_pixels(float w, float h){
    float Image_Ratio= w/h; //Assuming w > h
    float X_NDC,Y_NDC;    //X and Y in the NDC space (Normalized Device Coordinates)
    float X_Screen, Y_Screen; //X and Y coordenates of the pixels in the screen space
    float X_Camara,Y_Camara;    //X and Y in the camara space
    float alpha=3.1416/2;        //field of view
    for (int x=0;x<w;x++){
        for (int y=0;y<h;y++){
            X_NDC= (x+0.5)/w;       //we add 0.5 to get the middle point in the pixel
            Y_NDC= (y+0.5)/h;
            X_Screen=(2*X_NDC-1);
            Y_Screen=1-2*Y_NDC;     //we invert the operation here to get positive in the upper side of the cartesian map
            X_Camara=X_Screen*Image_Ratio*tan(alpha/2);
            Y_Camara=Y_Screen*tan(alpha/2);
            cout << "( " << x << ", " << y <<" ) ->" << "( " << X_Camara << ", " << Y_Camara<<", -1 )" << endl;
        }
    }
}

*/
// Simplified version

void normalize_pixels(float w, float h){
    float Image_Ratio= w/h;          //Assuming w > h
    float X_Camara,Y_Camara;         //X and Y in the camara space
    float alpha=3.1416/2;            //field of view
    for (int x=0;x<w;x++){
        for (int y=0;y<h;y++){
            X_Camara=(2*((x+0.5)/w)-1)*Image_Ratio*tan(alpha/2);
            Y_Camara=(1-2*((y+0.5)/h))*tan(alpha/2);
            cout << "( " << x << ", " << y <<" ) -> " << "( " << X_Camara << ", " << Y_Camara<<", -1 )" << endl;
        }
    }
}


int main(){
    float Width=2, Height=2;
    normalize_pixels(Width,Height);
    return 0;
}

//ooiii

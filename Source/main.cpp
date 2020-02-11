#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <mpi.h>
#include <chrono>

#include "Plane.hpp"
#include "Vect.hpp"
#include "Ray.hpp"
#include "Camera.hpp"
#include "Color.hpp"
#include "light.hpp"
#include "Source.hpp"
#include "Object.hpp"
#include "Sphere.hpp"
#include "json11.hpp"
#include "argh.hpp"


using namespace std;
using namespace json11;


/**
 * Structure qui contient les trois parametres d'un RGB
 * r = rouge
 * g = green
 * b = blue
 */
struct RGBType{
    double r;
    double g;
    double b; 
};

Vect stringToVector(string str)
{
	stringstream ss(str);
	double x, y, z;
	ss >> x >> y >> z;
	Vect v(x, y, z);
	return v;
}

Color stringToColor(string str)
{
	stringstream ss(str);
	double r, g, b, s;
	ss >> r >> g >> b >> s;
	Color color = {(double)r, (double)g, (double)b, (double)s};
	return color;
}

class Scene{
    public:
    std::vector <Source*> light_sources;
    std::vector <Object*> scene_objects;
    Camera *scene_camera; 

};

Scene jsonReader (string path){
    std::vector <Source*> light_sources;
    std::vector <Object*> scene_objects;
    std::vector <Camera*> scene_camera;
    Scene S;

    light_sources.clear();
    scene_objects.clear();
    
    ifstream t(path);
	string str((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
    //std::cout << path << '\n';
	string err;
	Json scene_json = Json::parse(str, err);
    //read array of lights
	Json::array lights_json = scene_json["lights"].array_items();
    //std::cout <<"num de luzes : " << lights_json.size() << '\n';
	for (size_t i = 0; i < lights_json.size(); i++){
        
		Vect position = stringToVector(lights_json[i]["position"].string_value());
		Color color = stringToColor(lights_json[i]["color"].string_value());

		string type = lights_json[i]["type"].string_value();
		light *L = new light (position, color);

		light_sources.push_back(dynamic_cast<Source*>(L));
	}

    //reads array of objects
    Json::array objects_json = scene_json["objects"].array_items();
	//std::cout <<"num de objetos : " << objects_json.size() << '\n';
    for (size_t i = 0; i < objects_json.size(); i++){

		Color color = stringToColor(objects_json[i]["color"].string_value());
		string type = objects_json[i]["type"].string_value();

		if (type == "sphere"){
			Vect center = stringToVector(objects_json[i]["center"].string_value());
			float radius = objects_json[i]["radius"].number_value();

			Sphere *S = new Sphere (center, radius, color);
			scene_objects.push_back(dynamic_cast<Object*>(S));
		}
        else if (type == "plan"){
            Vect normal = stringToVector(objects_json[i]["normal"].string_value());
            double distance = objects_json[i]["distance"].number_value();
            Plane *P = new Plane(normal, distance, color);
            scene_objects.push_back(dynamic_cast<Object*>(P));
        }
		
    }

    //read array of camera
	Json::array camera_json = scene_json["camera"].array_items();
    //std::cout <<"num de luzes : " << lights_json.size() << '\n';
	for (size_t i = 0; i < camera_json.size(); i++){
        
        //coordinate system
        Vect O (0,0,0);
        Vect X (1,0,0);
        Vect Y (0,1,0);
        Vect Z (0,0,1);

		Vect campos = stringToVector(camera_json[i]["position"].string_value());
		Vect look_at = stringToVector(camera_json[i]["lookat"].string_value());

        Vect diff_btw (campos.getVectX() - look_at.getVectX(),campos.getVectY() - look_at.getVectY(),campos.getVectZ()-look_at.getVectZ());
        Vect camdir = diff_btw.negative().normalize();
        Vect camright = Y.crossproduct(camdir).normalize();
        Vect camdown = camright.crossproduct(camdir);

        Camera *scene_cam = new Camera(campos, camdir, camright, camdown);
        S.scene_camera = scene_cam;
	}



    S.light_sources = light_sources;
    S.scene_objects = scene_objects;
    
    
    return S;

}
void savebmp (const char *filename, int w, int h, int dpi, RGBType *data ){
    FILE *f;
    int k = w*h;
    int s= 4*k;
    int filesize = 54 +s;

    double factor = 39.375;
    int m = static_cast<int>(factor);

    int ppm = dpi*m;

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54, 0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};


    bmpfileheader[ 2] = (unsigned char)(filesize);
    bmpfileheader[ 3] = (unsigned char)(filesize>>8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(w);
    bmpinfoheader[ 5] = (unsigned char)(w>>8);
    bmpinfoheader[ 6] = (unsigned char)(w>>16);
    bmpinfoheader[ 7] = (unsigned char)(w>>24);

    bmpinfoheader[ 8] = (unsigned char)(h);
    bmpinfoheader[ 9] = (unsigned char)(h>>8);
    bmpinfoheader[ 10] = (unsigned char)(h>>16);
    bmpinfoheader[ 11] = (unsigned char)(h>>24);

    bmpinfoheader[ 21] = (unsigned char)(s);
    bmpinfoheader[ 22] = (unsigned char)(s>>8);
    bmpinfoheader[ 23] = (unsigned char)(s>>16);
    bmpinfoheader[ 24] = (unsigned char)(s>>24);

    bmpinfoheader[ 25] = (unsigned char)(ppm);
    bmpinfoheader[ 26] = (unsigned char)(ppm>>8);
    bmpinfoheader[ 27] = (unsigned char)(ppm>>16);
    bmpinfoheader[ 28] = (unsigned char)(ppm>>24);

    bmpinfoheader[ 29] = (unsigned char)(ppm);
    bmpinfoheader[ 30] = (unsigned char)(ppm>>8);
    bmpinfoheader[ 31] = (unsigned char)(ppm>>16);
    bmpinfoheader[ 32] = (unsigned char)(ppm>>24);

    f = fopen(filename,"wb");

    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);

    for (int i = 0; i<k; i++) {

        double red = (data[i].r)*255;
        double green = (data[i].g)*255;
        double blue = (data[i].b)*255;

        unsigned char color[3] = {(unsigned char)floor(blue),(unsigned char)floor(green),(unsigned char)floor(red)};

        fwrite(color,1,3,f);

    }

    //fclose(f);
}



int winningObjectIndex(vector<double> object_intersections){
    // return the index of the winning intersection

    int index_of_minimum_value;

    //prevent unnecessary calculations
    if (object_intersections.size() == 0) {
        // if there are no intersections
        return -1;
    }
    else if(object_intersections.size() == 1){
            if (object_intersections.at(0) > 0){
                //if that intersection is greater than zero then its our index of minimum value
                return 0; //remember 0 is the index, not the value
            }
            else {
                //otherwise the only intersection is negative
                return -1;
            }
    }
    else {

        //otherwise there is more than one intersection
        //first find the maximum value
        double max=0;
        for (int i = 0; i < object_intersections.size(); i++){
            if (max < object_intersections.at(i)) {
                max = object_intersections.at(i);
            }
        }
        //then starting from the maximum value find the minimum positive value
        if(max > 0) {
            //we only want positive intersections
            index_of_minimum_value = 0;
            for (int index = 0; index < object_intersections.size(); index++){
                
                if (object_intersections.at(index) <= max && object_intersections.at(index)>0) {
                
                    max = object_intersections.at(index);
                    index_of_minimum_value = index;

                }
            }
            return index_of_minimum_value;
        }
        else {
            //all the intersections were negative

            return -1;
        }
    }

}

Color getColorAt(Vect intersection_position, Vect intersecting_ray_direction, vector<Object*> scene_objects, int index_of_winning_object, vector<Source*> light_sources, double accuracy, double ambientlight){
    
    Color winning_object_color = scene_objects.at(index_of_winning_object)->getColor();
    Vect winning_object_normal = scene_objects.at(index_of_winning_object)->getNormalAt(intersection_position);
    
    if (winning_object_color.getColorSpecial() == 2){
        //checkered/tile floor pattern
        //we give the special atribute 2 the property of being a black and white floor pattern

        int square = (int)floor(intersection_position.getVectX()) + (int)floor(intersection_position.getVectZ()); 

        if (square % 2 == 0) {
            //black tile
            winning_object_color.setColorRed(0);
            winning_object_color.setColorGreen(0);
            winning_object_color.setColorBlue(0);
        }
        else{
            winning_object_color.setColorRed(1);
            winning_object_color.setColorGreen(1);
            winning_object_color.setColorBlue(1); 
        }
    }

    Color final_color = winning_object_color.colorScalar(ambientlight);

    if (winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1){
        // reflection from objects with specular intensity

        double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
        Vect scalar1 = winning_object_normal.vectMul(dot1);
        Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
        Vect scalar2 = add1.vectMul(2);
        Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
        Vect reflection_direction = add2.normalize();

        Ray reflection_ray (intersection_position, reflection_direction);

        // determine what the ray intersects with first

        vector<double> reflection_intersections;

        for (int reflection_index = 0; reflection_index < scene_objects.size(); reflection_index++){
            reflection_intersections.push_back(scene_objects.at(reflection_index)->findIntersection(reflection_ray));
        }

        int index_of_winning_object_with_reflection = winningObjectIndex(reflection_intersections);

        if (index_of_winning_object_with_reflection != -1) {
            //if the reflection rays didn't miss
            if (reflection_intersections.at(index_of_winning_object_with_reflection) > accuracy){
                 //determine the position and direction at the point of intersection with the reflection ray
                 //the ray only affects the color of it reflected off something

                 Vect reflection_intersection_position = intersection_position.vectAdd(reflection_direction.vectMul(reflection_intersections.at(index_of_winning_object_with_reflection)));
                 Vect reflection_intersection_ray_direction = reflection_direction;

                 Color reflection_intersection_color = getColorAt(reflection_intersection_position, reflection_intersection_ray_direction, scene_objects, index_of_winning_object_with_reflection, light_sources, accuracy, ambientlight);
                 //we can get reflexions in the reflexions... recursive calls... more calcul time

                 final_color = final_color.colorAdd(reflection_intersection_color.colorScalar(winning_object_color.getColorSpecial()));
            }
        }

    }

    for(int light_index = 0; light_index < light_sources.size(); light_index++){
        Vect light_direction = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();


        float cosine_angle = winning_object_normal.dotProduct(light_direction);

        if(cosine_angle > 0){
            //test for shadows
            bool shadowed = false;
            Vect distance_to_light = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();
            float distance_to_light_magnitude = distance_to_light.magnitude();
            Ray shadow_ray (intersection_position, light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize());
            vector<double> secondary_intersections;
            for (int object_index = 0; object_index<scene_objects.size() && shadowed == false; object_index++){
                secondary_intersections.push_back(scene_objects.at(object_index)->findIntersection(shadow_ray));
            } 

            for (int c = 0; c < secondary_intersections.size(); c++){
                if (secondary_intersections.at(c) > accuracy){
                    if (secondary_intersections.at(c) <= distance_to_light_magnitude){
                        shadowed = true;
                    }
                }
                break;
            }

            if(shadowed == false){
                final_color = final_color.colorAdd(winning_object_color.colorMultiply(light_sources.at(light_index)->getLightColor()).colorScalar(cosine_angle));

                if (winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1){
                    //special  [0-1]
                    double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
                    Vect scalar1 = winning_object_normal.vectMul(dot1);
                    Vect add1 = scalar1.vectAdd(intersecting_ray_direction);
                    Vect scalar2 = add1.vectMul(2);
                    Vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
                    Vect reflection_direction = add2.normalize();
                    double specular = reflection_direction.dotProduct(light_direction);
                    if (specular > 0){
                        specular = pow(specular, 10);
                        final_color = final_color.colorAdd(light_sources.at(light_index)->getLightColor().colorScalar(specular*winning_object_color.getColorSpecial()));
                    }
                }
            }
        }
    }
    if(final_color.getColorBlue() >1.0 || final_color.getColorGreen() >1.0 || final_color.getColorRed() >1.0){
        final_color.setColorRed(1.0);
        final_color.setColorGreen(1.0);
        final_color.setColorBlue(1.0);
    }
    return final_color;
}

int thisone;


RGBType* Pixel_calcul(int Width, int Height,int aadepth, int accuracy,double aspect_ratio, int firstRow,int endRow, string path){
    int n = Width*Height;
    RGBType *pixels = new RGBType[n];
    double ambientlight = 0.2;

    

    int thisone, aa_index;
    double xamnt, yamnt;    //this variables are to allow the rays to go to the sides where the camera is pointing
    double tempRed, tempGreen, tempBlue;

    //coordinate system
    Vect O (0,0,0);
    Vect X (1,0,0);
    Vect Y (0,1,0);
    Vect Z (0,0,1);

/*

    //Camera deffinition
    Vect campos (3, 1.5, -4);
    Vect look_at (0,0,0);
    Vect diff_btw (campos.getVectX() - look_at.getVectX(),campos.getVectY() - look_at.getVectY(),campos.getVectZ()-look_at.getVectZ());
    Vect camdir = diff_btw.negative().normalize();
    Vect camright = Y.crossproduct(camdir).normalize();
    Vect camdown = camright.crossproduct(camdir);

    Camera scene_cam (campos, camdir, camright, camdown);
 
*/
    vector <Source*> light_sources;
    //light_sources.push_back(dynamic_cast<Source*>(&scene_light));

    //vector to store the objects of the scene nad loop through them
    vector<Object*> scene_objects;
    Scene S = jsonReader(path);
    light_sources = S.light_sources;
    scene_objects = S.scene_objects;
    Camera *scene_cam = S.scene_camera;
    Vect camdir = scene_cam->getCameraDirection();
    Vect camdown = scene_cam->getCameraDown();
    Vect camright = scene_cam->getcameraRight();
    Vect campos = scene_cam->getCameraPosition();
    for (int y = firstRow; y < endRow; y++)
    {

        for (int x = 0; x < Width; x++)     
        {
            thisone = (y - firstRow)*Width + x;      //determine coordenates of an individual pixel

            // start with a blank pixel
            double tempRed[aadepth*aadepth];
            double tempGreen[aadepth*aadepth];
            double tempBlue[aadepth*aadepth];



            for (int aax = 0; aax < aadepth; aax++)
            {
                for (int aay = 0; aay< aadepth; aay++)
                {

                    aa_index = aay*aadepth + aax;   //similar to thisone

                    srand(time(0));
                    //create the ray from the camera to this pixel
                    if(aadepth == 1){

                    //start with no anti-aliasing
                    
                        if (Width > Height){
                            // the image is wider than it is tall
                            xamnt = ((x+0.5)/Width)*aspect_ratio - (((Width-Height)/(double)Height)/2);
                            yamnt = ((Height - y) + 0.5)/Height;
                        }
                        else if (Height > Width){
                            // the image is taller than it is wide
                            xamnt = (x + 0.5)/Width;
                            yamnt = (((Height - y) + 0.5)/Height)/aspect_ratio - (((Height - Width)/(double)Width)/2);
                        }
                        else {
                            // the image is square
                            xamnt = (x + 0.5)/Width;
                            yamnt = ((Height - y) + 0.5)/Height;
                        }
                    }else{
                        //anti-aliasing
                        if (Width > Height){
                            // the image is wider than it is tall
                            xamnt = ((x + (double)aax/((double)aadepth -1))/Width)*aspect_ratio - (((Width-Height)/(double)Height)/2);
                            yamnt = ((Height - y) + (double)aax/((double)aadepth -1))/Height;
                        }
                        else if (Height > Width){
                            // the image is taller than it is wide
                            xamnt = (x + (double)aax/((double)aadepth -1))/Width;
                            yamnt = (((Height - y) + (double)aax/((double)aadepth -1))/Height)/aspect_ratio - (((Height - Width)/(double)Width)/2);
                        }
                        else {
                            // the image is square
                            xamnt = (x + (double)aax/((double)aadepth -1))/Width;
                        }

                    }
                    Vect cam_ray_origin = scene_cam->getCameraPosition();    //ray's origin is the same of camera's
                    Vect cam_ray_direction = camdir.vectAdd(camright.vectMul(xamnt - 0.5).vectAdd(camdown.vectMul(yamnt - 0.5))).normalize();

                    Ray cam_ray (cam_ray_origin, cam_ray_direction);
                    vector<double> intersections;   //vector that stores the intersections
                    //Loop through each of the objects in our scene and determine if there are any intersections
                    for (int index = 0; index < scene_objects.size(); index++){
                        intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray));    //Asks if there are any intersections between the objects and the ray and stores it
                    }

                    //now we need to find the closest object to the camera
                    int index_of_winning_object = winningObjectIndex(intersections);

                    if (index_of_winning_object == -1) {
                        //if the ray miss, put the color of the background which is black
                        tempRed[aa_index] = 0;
                        tempGreen[aa_index] = 0;
                        tempBlue[aa_index] = 0;
                    }
                    else {
                        //index coresponds to an object in our scene
                        if (intersections.at(index_of_winning_object)>accuracy){
                            Vect intersection_position = cam_ray_origin.vectAdd( cam_ray_direction.vectMul(intersections.at( index_of_winning_object ) ));

                            Vect intersecting_ray_direction = cam_ray_direction;

                            Color intersection_color = getColorAt(intersection_position, intersecting_ray_direction, scene_objects, index_of_winning_object, light_sources, accuracy, ambientlight);
                            
                            tempRed[aa_index] = intersection_color.getColorRed();
                            tempGreen[aa_index] = intersection_color.getColorGreen();
                            tempBlue[aa_index] = intersection_color.getColorBlue();
                        }
                    }
                }
                
            }

            //average the pixel color
            double totalred = 0;
            double totalgreen = 0;
            double totalblue = 0;

            for(int ired = 0; ired < aadepth*aadepth; ired++)
            {
                totalred = totalred + tempRed[ired];
            }
            for(int igreen = 0; igreen < aadepth*aadepth; igreen++)
            {
                totalgreen = totalgreen + tempGreen[igreen];
            }
            for(int iblue = 0; iblue < aadepth*aadepth; iblue++)
            {
                totalblue = totalblue + tempGreen[iblue];
            }

            double avgRed = totalred/(aadepth*aadepth);
            double avggreen = totalgreen/(aadepth*aadepth);
            double avgblue = totalblue/(aadepth*aadepth);

            pixels[thisone].r = avgRed;
            pixels[thisone].g = avggreen;
            pixels[thisone].b = avgblue;
        }
    }
    return pixels;
}


int main(int argc, char *argv[])
{
    std::chrono::time_point < std::chrono::system_clock > start_calcul, end_calcul, start_program, end_program;
    
    argh::parser cmdl(argv);
    std::string path;
    cmdl({"-s", "--scene"}, "scene.json") >> path;


    const int Width=600, Height=400, dpi = 72;
    float aspect_ratio= float(Width)/(float)Height;
    float ambientlight = 0.2;
    float accuracy = 0.00001;
    int aadepth = 1;        //anti-aliasing depth
    float aathreshold = 0.1;   //anti-aliasing threshold

    int n = Width*Height;

    RGBType *pixels = new RGBType[n];



    
    MPI_Init(&argc,&argv);
    start_program = std::chrono::system_clock::now();

    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    MPI_Status status;

    int local_Height = Height/size;
    int firstrow = (size-(size-rank)) * local_Height;
    int endrow = firstrow + local_Height;
 
    int local_n = local_Height*Width;
    RGBType *local_pixels = new RGBType[local_n];

    start_calcul = std::chrono::system_clock::now();

    local_pixels = Pixel_calcul(Width,Height,aadepth,accuracy,aspect_ratio,firstrow,endrow,path);


    end_calcul = std::chrono::system_clock::now();
    std::chrono::duration < double >elapsed_seconds = end_calcul - start_calcul;
    std::cout << "Temps de calcul: " << elapsed_seconds.count() << " secondes, tache : " << rank << "\n";


    float red_pixels[local_n];
    float blue_pixels[local_n];
    float green_pixels[local_n];


    for(int i=0;i<local_n;i++){

        red_pixels[i] = local_pixels[i].r;
        blue_pixels[i] = local_pixels[i].b;
        green_pixels[i] = local_pixels[i].g;
    }

    if (rank == 0){
    
        float total_red_pixels[n];
        float total_blue_pixels[n];
        float total_green_pixels[n];

        //Get all the pixels
        MPI_Gather( red_pixels,local_n, MPI_FLOAT,
                total_red_pixels, local_n, MPI_FLOAT,
                0,
                MPI_COMM_WORLD               
               );
        MPI_Gather( green_pixels,local_n, MPI_FLOAT,
                total_green_pixels, local_n, MPI_FLOAT,
                0,
                MPI_COMM_WORLD               
               );
        MPI_Gather( blue_pixels,local_n, MPI_FLOAT,
                total_blue_pixels, local_n, MPI_FLOAT,
                0,
                MPI_COMM_WORLD               
               );
        for(int i=0; i < n; i++){
            pixels[i].r = total_red_pixels[i];
            pixels[i].b = total_blue_pixels[i];
            pixels[i].g = total_green_pixels[i];

        }

        //save the image
        savebmp("Scene.bmp",Width,Height,dpi,pixels);
        end_program = std::chrono::system_clock::now();
        std::chrono::duration < double >total_time = end_program - start_program;
        std::cout << "Temps de calcul: " << total_time.count() << " secondes\n";

    }else{
        MPI_Gather( red_pixels,local_n, MPI_FLOAT,
                NULL, local_n, MPI_FLOAT,
                0,
                MPI_COMM_WORLD               
               );
        MPI_Gather( green_pixels,local_n, MPI_FLOAT,
                NULL, local_n, MPI_FLOAT,
                0,
                MPI_COMM_WORLD               
               );
        MPI_Gather( blue_pixels,local_n, MPI_FLOAT,
                NULL, local_n, MPI_FLOAT,
                0,
                MPI_COMM_WORLD               
               );
    }

    delete local_pixels;
    delete pixels;

    MPI_Finalize();
    return 0;
}

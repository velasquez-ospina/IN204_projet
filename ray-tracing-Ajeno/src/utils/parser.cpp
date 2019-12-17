#include <fstream>
#include <streambuf>
#include <sstream>
#include <math.h>
#include "includes/json11.hpp"

#include "utils/parser.hpp"
#include "utils/loader_obj.hpp"

#include "objects/triangle.hpp"
#include "objects/sphere.hpp"

#include "lights/spot.hpp"
#include "lights/sun.hpp"

using namespace json11;
using namespace std;

Vector stringToVector(string str)
{
	stringstream ss(str);
	float x, y, z;
	ss >> x >> y >> z;
	Vector v(x, y, z);
	return v;
}

color_rgba stringToColor(string str)
{
	stringstream ss(str);
	float r, g, b;
	ss >> r >> g >> b;
	color_rgba color = {(float)r, (float)g, (float)b};
	return color;
}

vec2 stringToVec2(string str)
{
	stringstream ss(str);
	float x, y;
	ss >> x >> y;
	vec2 v = {x, y};
	return v;
}

Scene jsonToScene(string path)
{
	ifstream t(path);
	string str((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());

	string err;
	Json scene_json = Json::parse(str, err);

	// Parse Lights
	Json::array lights_json = scene_json["lights"].array_items();

	std::vector<Light *> lights;
	for (size_t i = 0; i < lights_json.size(); i++)
	{
		Vector position = stringToVector(lights_json[i]["position"].string_value());
		color_rgba color = stringToColor(lights_json[i]["color"].string_value());

		string type = lights_json[i]["type"].string_value();
		Light *L;
		if (type == "spot")
			L = new Spot(position, color);
		else if (type == "sun")
			L = new Sun(position, color);
		else
			continue;
		lights.push_back(L);
	}

	// Parse Objects
	Json::array objects_json = scene_json["objects"].array_items();

	std::vector<Object *> obj;
	for (size_t i = 0; i < objects_json.size(); i++)
	{
		float opacity = objects_json[i]["opacity"].number_value();
		float reflexitivity = objects_json[i]["reflexitivity"].number_value();
		color_rgba color = stringToColor(objects_json[i]["color"].string_value());

		// The rotation value is in degree in the json file
		Vector translation = stringToVector(objects_json[i]["translation"].string_value());
		Vector rotation = stringToVector(objects_json[i]["rotation"].string_value()) * (M_PI / 180);
		bool enabled_uv = objects_json[i]["enabled_uv"].bool_value();
		bool enabled_smooth = objects_json[i]["enabled_smooth"].bool_value();

		string type = objects_json[i]["type"].string_value();
		if (type == "triangle")
		{
			Json::array positions = objects_json[i]["positions"].array_items();

			if (positions.size() != 3)
				continue;
			Vector A = stringToVector(positions[0].string_value()) + translation;
			Vector B = stringToVector(positions[1].string_value()) + translation;
			Vector C = stringToVector(positions[2].string_value()) + translation;

			if (enabled_uv)
			{
				Json::array uvs = objects_json[i]["uvs"].array_items();
				if (positions.size() != 3)
					continue;
				vec2 A_uv = stringToVec2(uvs[0].string_value());
				vec2 B_uv = stringToVec2(uvs[1].string_value());
				vec2 C_uv = stringToVec2(uvs[2].string_value());

				string texture_path = objects_json[i]["texture_path"].string_value();
				texture *tex = new texture(texture_path);

				Triangle *T = new Triangle(A, B, C, A_uv, B_uv, C_uv, tex, opacity, reflexitivity);
				obj.push_back(T);
			}
			else
			{
				Triangle *T = new Triangle(A, B, C, color, opacity, reflexitivity);
				obj.push_back(T);
			}
		}
		else if (type == "sphere")
		{
			Vector center = stringToVector(objects_json[i]["center"].string_value());
			float radius = objects_json[i]["radius"].number_value();

			Sphere *S = new Sphere(center, radius, color, opacity, reflexitivity);
			obj.push_back(S);
		}
		else if (type == "obj")
		{
			Vector mag(1, 1, 1); // Hardcoded for now
			string path = objects_json[i]["path"].string_value();

			if (enabled_uv && enabled_smooth)
			{
				string texture_path = objects_json[i]["texture_path"].string_value();
				std::vector<Object *> obj_loaded = loaderObj(path, color, translation, rotation, mag, opacity, reflexitivity, true, true, texture_path);
				obj.insert(obj.end(), obj_loaded.begin(), obj_loaded.end());
			}
			else if (enabled_uv)
			{
				string texture_path = objects_json[i]["texture_path"].string_value();
				std::vector<Object *> obj_loaded = loaderObj(path, color, translation, rotation, mag, opacity, reflexitivity, false, true, texture_path);
				obj.insert(obj.end(), obj_loaded.begin(), obj_loaded.end());
			}
			else if (enabled_smooth)
			{
				std::vector<Object *> obj_loaded = loaderObj(path, color, translation, rotation, mag, opacity, reflexitivity, true, false);
				obj.insert(obj.end(), obj_loaded.begin(), obj_loaded.end());
			}
			else
			{
				std::vector<Object *> obj_loaded = loaderObj(path, color, translation, rotation, mag, opacity, reflexitivity, false, false);
				obj.insert(obj.end(), obj_loaded.begin(), obj_loaded.end());
			}
		}
		else
			continue;
	}

	// Camera
	Json camera_json = scene_json["camera"];

	Vector camera_pos = stringToVector(camera_json["position"].string_value());
	Vector camera_dir = stringToVector(camera_json["direction"].string_value());
	float camera_dist = camera_json["dist_normal"].number_value();

	Camera cam(camera_pos, camera_dir, camera_dist);

	// Engine
	Json engine_json = scene_json["engine"];

	unsigned height = engine_json["screen_height"].int_value();
	unsigned width = engine_json["screen_width"].int_value();
	float field_of_vue = engine_json["field_of_vue"].number_value() * M_PI / 180;
	float pixel_size = 2 * tan(field_of_vue / 2) / height;
	Vector top_dir = stringToVector(engine_json["top_dir"].string_value());
	unsigned recursion_level = engine_json["recursion_level"].int_value();
	float env_diffusion = engine_json["env_diffusion"].number_value();
	string output_name = engine_json["output_name"].string_value();

	if (output_name == "")
		output_name = "scene";

	Json::array pixels_offset_json = engine_json["pixels_offset"].array_items();

	std::vector<vec2> pixels_offset;
	for (size_t i = 0; i < pixels_offset_json.size(); i++)
	{
		pixels_offset.push_back(stringToVec2(pixels_offset_json[i].string_value()));
	}

	Screen screen(height, width, pixel_size, top_dir, pixels_offset);

	Scene scene(cam, screen, lights, obj, recursion_level, env_diffusion, output_name);
	return scene;
}

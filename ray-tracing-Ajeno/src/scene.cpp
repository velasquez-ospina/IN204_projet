#include "scene.hpp"
#include "utils/params.hpp"

Scene::Scene(Camera c, Screen s, std::vector<Light *> l, std::vector<Object *> t, unsigned recursion_level, float env_diffusion, std::string output_name) : _camera(c), _screen(s), _lights(l), _objects(t), _recursion_level(recursion_level), _env_diffusion(env_diffusion), _output_name(output_name) {}

Scene::~Scene()
{
	for (size_t i = 0; i < _objects.size(); i++)
	{
		delete _objects[i];
	}
}

int Scene::getObjectsIntersection(const Vector &direction, const Vector &point, Vector &intersection) const
{
	int index = -1;
	Vector tmp_intersection(0, 0, 0);
	double min_distance = -1;

	for (size_t i = 0; i < _objects.size(); i++)
	{
		if (_objects[i]->isIntersecting(point, direction, tmp_intersection))
		{
			double distance = (tmp_intersection - point).getNorm();
			if ((index == -1 || min_distance > distance) && distance > EPSILON)
			{
				intersection = tmp_intersection;
				min_distance = distance;
				index = i;
			}
		}
	}
	return index;
}

color_rgba Scene::getLightContribution(const Vector &point, const Vector &direction, int light_index, int deep) const
{
	if (deep == 0)
		return {0, 0, 0};

	Vector intersection(0, 0, 0);

	int index = getObjectsIntersection(direction, point, intersection);

	// If intersection is not found or is after the light
	if (index == -1 || _lights[light_index]->isIntersectionAfterLight(point, intersection))
		return _lights[light_index]->getColor();

	color_rgba c_inter = _objects[index]->getColor(intersection);
	float opacity = _objects[index]->getOpacity() * (c_inter[3] / 255);

	return (1 - opacity) * getLightContribution(intersection, direction, light_index, deep - 1);
}

color_rgba Scene::getLightsContribution(const Vector &point, const Vector &obj_normal, int actual_obj) const
{
	Vector direction_to_light;
	color_rgba source = _env_diffusion * _objects[actual_obj]->getColor(point);
	color_rgba loc_source = {0, 0, 0};
	Vector intersection_pt = {0, 0, 0};
	int deep = _recursion_level;

	for (unsigned l = 0; l < _lights.size(); l++)
	{
		direction_to_light = _lights[l]->getDirectionToLight(point);

		loc_source = getLightContribution(point, direction_to_light, l, deep);

		float intensity = obj_normal.dotProduct(direction_to_light.normalize());
		if (intensity < 0)
			intensity = 0;
		source = addSynthese(intensity * loc_source, source);
	}

	return source;
}

color_rgba Scene::trace(const Vector &point, const Vector &direction, int traced) const
{

	Vector intersection(0, 0, 0);

	// find if there is an intersection
	int index = getObjectsIntersection(direction, point, intersection);

	//Check light that contributes to the intersection point
	if (index > -1) // if intersect
	{
		color_rgba local_source = {0, 0, 0};
		color_rgba transparency_source = {0, 0, 0};
		color_rgba reflexion_source = {0, 0, 0};
		color_rgba c = _objects[index]->getColor(intersection);

		float opacity = (c[3] / 255) * _objects[index]->getOpacity();
		float reflexivity = _objects[index]->getReflexivity();

		Vector obj_normal = _objects[index]->getNormal(intersection, direction);

		// //Transparency
		if (traced > 0)
		{
			transparency_source = trace(intersection, direction, traced - 1);
		}

		// Reflexion
		if (traced > 0)
		{
			Vector u = -1 * direction;
			Vector p = u.dotProduct(obj_normal) * obj_normal;
			Vector new_direction = 2 * p - u;

			reflexion_source = trace(intersection, new_direction, traced - 1);
		}

		// Light color
		color_rgba lights_color = {0, 0, 0};
		lights_color = getLightsContribution(intersection, obj_normal, index);

		// set the pixel color
		local_source = (1 - reflexivity) * lights_color * c + reflexivity * reflexion_source;
		return opacity * local_source + (1 - opacity) * transparency_source;
	}
	return {205, 197, 188};
}

void Scene::render()
{
	unsigned width = _screen.getWidth();
	unsigned height = _screen.getHeight();
	unsigned steps = 0;

	Vector camera_dir = _camera.getDirection();
	Vector screen_center = camera_dir.normalize() * _camera.getDistance();
	Vector camera_position = _camera.getPosition();
	std::vector<Offset> pixels_offset = _screen.getPixelsOffset();

#pragma omp parallel for schedule(dynamic, 20)
	for (unsigned col = 0; col < width; col++)
	{
		for (unsigned row = 0; row < height; row++)
		{
			// initialisation
			color_rgba source = {0, 0, 0};
			// Antialiasing
			for (Offset offset : pixels_offset)
			{
				Vector direction = _screen.pixelDirection(row, col, offset.right, offset.bottom, screen_center);
				color_rgba local_source = trace(camera_position, direction, _recursion_level);
				source = addSynthese(offset.weight * local_source, source);
			}

			source = colorFloor(source);

			_screen.setPixelColor(row, col, source);
		}

		//Progress status
		if (col % 200 == 0)
		{
			steps < col ? steps = col : steps = steps;
			std::cout << "Progress " << (float)steps / (float)width * 100 << " %";
			std::cout << "\n";
		}
	}

	// Dithering
	// for (unsigned col = 0; col < width; col++)
	// {
	// 	for (unsigned row = 0; row < height; row++)
	// 	{
	// 		color_rgba old_pixel = _screen.pixelAt(row, col);
	// 		color_rgba new_pixel = colorFloor(old_pixel);
	// 		color_rgba quant_error = old_pixel - new_pixel;

	// 		_screen.setPixelColor(row, col + 1, _screen.pixelAt(row, col + 1) + quant_error * (7 / 16));
	// 		_screen.setPixelColor(row + 1, col, _screen.pixelAt(row + 1, col) + quant_error * (5 / 16));
	// 		_screen.setPixelColor(row + 1, col - 1, _screen.pixelAt(row + 1, col - 1) + quant_error * (3 / 16));
	// 		_screen.setPixelColor(row + 1, col + 1, _screen.pixelAt(row + 1, col + 1) + quant_error * (1 / 16));
	// 	}
	// }

	_screen.save(_output_name + ".ppm");
}

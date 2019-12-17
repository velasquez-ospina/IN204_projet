#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>

#include "camera.hpp"
#include "screen.hpp"
#include "lights/light.hpp"
#include "objects/object.hpp"

// This class is used to render the scene
// It contains the logic of the rays
class Scene
{
  private:
	Camera _camera;
	Screen _screen;
	std::vector<Light *> _lights;
	std::vector<Object *> _objects;
	unsigned _recursion_level; // Number of rays traced until it "fades of"
	float _env_diffusion;	  // Between 0 and 1 to specify the ambiant diffusion of objects
	std::string _output_name;

	int getObjectsIntersection(const Vector &direction, const Vector &point, Vector &intersection) const;

	color_rgba getLightContribution(const Vector &start_point, const Vector &direction, int light_index, int deep) const;
	color_rgba getLightsContribution(const Vector &point, const Vector &obj_normal, int actual_obj) const;

	// Recursive fonction that shoots a ray
	color_rgba trace(const Vector &intersection, const Vector &direction, int traced) const;

  public:
	Scene(Camera c, Screen s, std::vector<Light *> l, std::vector<Object *> t, unsigned recursion_level, float env_diffusion, std::string output_name);
	~Scene();

	// Main function that renders the scene and saves it
	void render();
};

#endif //SCENE_HPP

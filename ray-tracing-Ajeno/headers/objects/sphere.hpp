#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "utils/vector.hpp"
#include "utils/color.hpp"
#include "object.hpp"

// Sphere Object
class Sphere : public Object
{
  private:
	Vector _center;
	float _radius;

  public:
	Sphere(const Vector &center, float radius, const color_rgba &col, float opacity, float reflexivity);
	~Sphere() {}

	Vector getNormal(const Vector &, const Vector &) const;
	bool isIntersecting(const Vector &point, const Vector &direction, Vector &intersection) const;
};

#endif //SPHERE_HPP

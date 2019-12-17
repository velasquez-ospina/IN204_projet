#include "objects/sphere.hpp"
#include "utils/params.hpp"

Sphere::Sphere(const Vector &center, float radius, const color_rgba &col, float opacity, float reflexivity) : Object(col, opacity, reflexivity), _center(center), _radius(radius) {}

Vector Sphere::getNormal(const Vector &point, const Vector &) const
{
	return (point - _center).normalize();
}

bool Sphere::isIntersecting(const Vector &point, const Vector &direction, Vector &intersection) const
{
	Vector dir_to_center = point - _center;
	Vector dir_normalized = direction.normalize();
	double l_sqrt = dir_normalized.dotProduct(dir_to_center);
	double l = l_sqrt * l_sqrt;
	double norm = dir_to_center.getNorm();
	double r = norm * norm - _radius * _radius;

	// A negative discriminant corresponds to ray missing sphere
	double discr = l - r;
	if (discr < EPSILON)
		return false;

	// Ray now found to intersect sphere, compute smallest dist value of intersection
	double dist = -l_sqrt - sqrt(discr);

	// If dist is negative, ray started inside sphere so clamp t to zero
	if (dist < EPSILON)
		dist = -l_sqrt + sqrt(discr);

	if (dist > EPSILON)
	{
		intersection = point + dist * direction.normalize();
		return true;
	}

	return false;
}

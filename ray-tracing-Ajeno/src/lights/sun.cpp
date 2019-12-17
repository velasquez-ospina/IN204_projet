#include "lights/sun.hpp"

Sun::Sun(const Vector &vec, const color_rgba &c) : Light(vec, c) {}

Vector Sun::getDirectionToLight(const Vector &) const
{
	return -1 * _vec;
}

bool Sun::isIntersectionAfterLight(const Vector &, const Vector &) const
{
	return false;
}

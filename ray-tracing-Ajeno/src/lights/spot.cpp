#include "lights/spot.hpp"

Spot::Spot(const Vector &vec, const color_rgba &c) : Light(vec, c) {}

Vector Spot::getDirectionToLight(const Vector &point) const
{
	return _vec - point;
}

Vector Spot::getPosition() const
{
	return _vec;
}

bool Spot::isIntersectionAfterLight(const Vector &A, const Vector &B) const
{
	return ((A - B).getNorm() > (A - _vec).getNorm());
}

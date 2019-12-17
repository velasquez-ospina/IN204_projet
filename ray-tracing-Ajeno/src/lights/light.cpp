#include "lights/light.hpp"

Light::Light(const Vector &vec, const color_rgba &c) : _vec(vec), _color(c) {}

color_rgba Light::getColor() const
{
	return _color;
}

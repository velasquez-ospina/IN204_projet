#include "objects/object.hpp"

Object::Object(const color_rgba &col, float opacity, float reflexivity) : _color(col), _opacity(opacity), _reflexivity(reflexivity) {}

color_rgba Object::getColor(const Vector &)
{
	_color[3] = 255;
	return _color;
}

float Object::getOpacity() const
{
	return _opacity;
}

float Object::getReflexivity() const
{
	return _reflexivity;
}

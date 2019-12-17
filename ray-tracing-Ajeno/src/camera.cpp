#include "camera.hpp"

Camera::Camera(const Vector &pos, const Vector &dir, float dist_n) : _position(pos), _direction(dir), _dist_normal(dist_n)
{
	_direction.normalize();
}

Vector Camera::getDirection() const
{
	return _direction;
}

Vector Camera::getPosition() const
{
	return _position;
}

float Camera::getDistance() const
{
	return _dist_normal;
}

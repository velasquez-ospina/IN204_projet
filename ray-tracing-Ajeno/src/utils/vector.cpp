#include "utils/vector.hpp"

Vector::Vector(float x, float y, float z) : _x(x), _y(y), _z(z) {}

Vector::Vector(const Vector &v) : _x(v.x()), _y(v.y()), _z(v.z()) {}

float Vector::x() const
{
	return _x;
}

float Vector::y() const
{
	return _y;
}

float Vector::z() const
{
	return _z;
}

Vector Vector::crossProduct(const Vector &v) const
{
	return Vector(_y * v.z() - _z * v.y(), _z * v.x() - _x * v.z(), _x * v.y() - _y * v.x());
}

float Vector::dotProduct(const Vector &v) const
{
	return _x * v.x() + _y * v.y() + _z * v.z();
}

Vector Vector::normalize() const
{
	float norm = getNorm();
	float x = _x / norm;
	float y = _y / norm;
	float z = _z / norm;
	return Vector(x, y, z);
}

float Vector::getNorm() const
{
	return sqrt(_x * _x + _y * _y + _z * _z);
}

Vector Vector::rotateX(float angle)
{
	return Vector(_x, _y * cos(angle) - _z * sin(angle), _y * sin(angle) + _z * cos(angle));
}

Vector Vector::rotateY(float angle)
{
	return Vector(_x * cos(angle) + _z * sin(angle), _y, -_x * sin(angle) + _z * cos(angle));
}

Vector Vector::rotateZ(float angle)
{
	return Vector(_x * cos(angle) - _y * sin(angle), _x * sin(angle) + _y * cos(angle), _z);
}

Vector Vector::rotate3D(const Vector &rot)
{
	Vector res(_x, _y, _z);
	return res.rotateX(rot.x()).rotateY(rot.y()).rotateZ(rot.z());
}

Vector Vector::magnify(const Vector &mag)
{
	return Vector(_x * mag.x(), _y * mag.y(), _z * mag.z());
}

Vector
operator+(const Vector &v1, const Vector &v2)
{
	return Vector(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
}

Vector operator-(const Vector &v1, const Vector &v2)
{
	return Vector(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
}

Vector operator*(const Vector &v, float lambda)
{
	return Vector(v.x() * lambda, v.y() * lambda, v.z() * lambda);
}

Vector operator*(float lambda, const Vector &v)
{
	return v * lambda;
}

bool operator==(const Vector &v1, const Vector &v2)
{

	return (v1.x() == v2.x() && v1.y() == v2.y() && v1.z() == v2.z());
}

bool operator!=(const Vector &v1, const Vector &v2)
{

	return (v1.x() != v2.x() || v1.y() != v2.y() || v1.z() != v2.z());
}

std::ostream &operator<<(std::ostream &stream, const Vector &v)
{
	stream << "( " << v.x() << ", " << v.y()
		   << ", " << v.z()
		   << ")";
	return stream;
}

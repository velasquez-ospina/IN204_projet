#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
#include <cmath>

// Defining a class vector in order to easily use vectors
class Vector
{
  private:
	float _x;
	float _y;
	float _z;

	Vector rotateX(float angle);
	Vector rotateY(float angle);
	Vector rotateZ(float angle);

  public:
	Vector() {}
	Vector(float x, float y, float z);
	Vector(const Vector &v);
	~Vector() {}

	float x() const;
	float y() const;
	float z() const;

	Vector crossProduct(const Vector &v) const;
	float dotProduct(const Vector &v) const;

	Vector normalize() const;
	float getNorm() const;

	//Angle in radian
	Vector rotate3D(const Vector &rot);
	Vector magnify(const Vector &mag);
};

Vector operator+(const Vector &v1, const Vector &v2);
Vector operator-(const Vector &v1, const Vector &v2);
Vector operator*(const Vector &v, float lambda);
Vector operator*(float lambda, const Vector &v);
bool operator==(const Vector &v1, const Vector &v2);
bool operator!=(const Vector &v1, const Vector &v2);

std::ostream &operator<<(std::ostream &stream, const Vector &v);

#endif //VECTOR_HPP

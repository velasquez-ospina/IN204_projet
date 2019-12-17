#include "objects/triangle.hpp"
#include "utils/params.hpp"

Triangle::Triangle(const Vector &A, const Vector &B, const Vector &C, const color_rgba &col, float opacity, float reflexivity) : Object(col, opacity, reflexivity), _A(A), _B(B), _C(C) {}

// Constructor of triangle with uv mapping
Triangle::Triangle(const Vector &A, const Vector &B, const Vector &C, const vec2 &A_uv, const vec2 &B_uv, const vec2 &C_uv, texture *t, float opacity, float reflexivity)
{
	setBase(A, B, C, opacity, reflexivity);
	setUv(A_uv, B_uv, C_uv, t);
}

// Constructor of triangle with smoothering
Triangle::Triangle(const Vector &A, const Vector &B, const Vector &C, const Vector &A_normal, const Vector &B_normal, const Vector &C_normal, float opacity, float reflexivity)
{
	setBase(A, B, C, opacity, reflexivity);
	setSmooth(A_normal, B_normal, C_normal);
}

Triangle::Triangle(const Vector &A, const Vector &B, const Vector &C, const vec2 &A_uv, const vec2 &B_uv, const vec2 &C_uv, texture *t, const Vector &A_normal, const Vector &B_normal, const Vector &C_normal, float opacity, float reflexivity)
{
	setBase(A, B, C, opacity, reflexivity);
	setSmooth(A_normal, B_normal, C_normal);
	setUv(A_uv, B_uv, C_uv, t);
}

void Triangle::setBase(const Vector &A, const Vector &B, const Vector &C, float opacity, float reflexivity)
{
	_A = A;
	_B = B;
	_C = C;
	_opacity = opacity;
	_reflexivity = reflexivity;
}

void Triangle::setUv(const vec2 &A_uv, const vec2 &B_uv, const vec2 &C_uv, texture *t)
{
	_A_uv = A_uv;
	_B_uv = B_uv;
	_C_uv = C_uv;
	_enabled_uv = true;
	_texture = t;
}

void Triangle::setSmooth(const Vector &A_normal, const Vector &B_normal, const Vector &C_normal)
{
	_A_normal = A_normal;
	_B_normal = B_normal;
	_C_normal = C_normal;
	_enabled_smooth = true;
}

Vector Triangle::getObjectNormal(const Vector &point) const
{
	// If the smoothering is enabled
	if (_enabled_smooth)
	{
		Vector u = _B - _C;
		Vector v = _A - _C;
		Vector a = (point - _C).crossProduct(u);
		Vector b = (point - _C).crossProduct(v);
		Vector tot = u.crossProduct(v);
		double tot_area = tot.getNorm();

		// Compute barycentic coefficents
		double alpha = a.getNorm() / tot_area;
		double beta = b.getNorm() / tot_area;
		double gamma = 1 - alpha - beta;

		return alpha * _A_normal + beta * _B_normal + gamma * _C_normal;
	}
	else
	{
		Vector edge1 = _A - _B;
		Vector edge2 = _A - _C;
		return edge1.crossProduct(edge2);
	}
}

Vector Triangle::getNormal(const Vector &point, const Vector &direction) const
{
	Vector obj_normal = this->getObjectNormal(point).normalize();
	bool is_good_normal = obj_normal.dotProduct(direction) < 0;
	if (!is_good_normal)
	{
		obj_normal = -1 * obj_normal;
	}
	return obj_normal;
}

color_rgba Triangle::getColor(const Vector &point)
{
	// If uv mapping is not enabled
	if (!_enabled_uv)
		return Object::getColor(point);

	// Barycentric coordinate
	Vector u = _B - _C;
	Vector v = _A - _C;
	Vector a = (point - _C).crossProduct(u);
	Vector b = (point - _C).crossProduct(v);
	Vector tot = u.crossProduct(v);
	double tot_area = tot.getNorm();

	double alpha = a.getNorm() / tot_area;
	double beta = b.getNorm() / tot_area;
	double gamma = 1 - alpha - beta;

	vec2 text = {(float)(alpha * _A_uv[0] + beta * _B_uv[0] + gamma * _C_uv[0]), (float)(alpha * _A_uv[1] + beta * _B_uv[1] + gamma * _C_uv[1])};

	return _texture->sample(text[0], text[1]);
}

bool Triangle::isIntersecting(const Vector &point, const Vector &direction, Vector &intersection) const
{
	Vector edge1 = (_B - _A);
	Vector edge2 = (_C - _A);

	Vector orthogonal = direction.crossProduct(edge2);
	double is_parallel = edge1.dotProduct(orthogonal);

	if (is_parallel > -EPSILON && is_parallel < EPSILON)
		return false; // This ray is parallel to this triangle.

	double deviation = 1.0 / is_parallel;
	Vector distance = (point - _A);
	double u = deviation * (distance.dotProduct(orthogonal));

	if (u < 0.0 || u > 1.0)
		return false;

	Vector q = distance.crossProduct(edge1);
	double v = deviation * direction.dotProduct(q);
	if (v < 0.0 || u + v > 1.0)
		return false;

	// At this stage we can compute t to find out where the intersection point is on the line.
	double t = deviation * edge2.dotProduct(q);
	if (t > EPSILON) // ray intersection
	{
		intersection = point + direction * t;
		return true;
	}
	else // This means that there is a line intersection but not a ray intersection.
		return false;
}

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "utils/vector.hpp"
#include "utils/color.hpp"
#include "utils/texture.hpp"
#include "utils/type.hpp"
#include "object.hpp"

// Typical object for ray-tracing
// Used in .obj files
class Triangle : public Object
{
  private:
	Vector _A;
	Vector _B;
	Vector _C;

	// Used for the uv mapping
	bool _enabled_uv = false;
	vec2 _A_uv;
	vec2 _B_uv;
	vec2 _C_uv;
	texture *_texture;

	// Used for the smoothing
	bool _enabled_smooth = false;
	Vector _A_normal;
	Vector _B_normal;
	Vector _C_normal;

	void setBase(const Vector &A, const Vector &B, const Vector &C, float opacity, float reflexivity);
	void setUv(const vec2 &A_uv, const vec2 &B_uv, const vec2 &C_uv, texture *t);
	void setSmooth(const Vector &A_normal, const Vector &B_normal, const Vector &C_normal);

	Vector getObjectNormal(const Vector &point) const;

  public:
	// Basic constructor with neither uv mapping nor smoothering
	Triangle(const Vector &A, const Vector &B, const Vector &C, const color_rgba &col, float opacity, float reflexivity);
	// Constructor of triangle with uv mapping
	Triangle(const Vector &A, const Vector &B, const Vector &C, const vec2 &A_uv, const vec2 &B_uv, const vec2 &C_uv, texture *t, float opacity, float reflexivity);
	// Constructor of triangle with smoothering
	Triangle(const Vector &A, const Vector &B, const Vector &C, const Vector &A_normal, const Vector &B_normal, const Vector &C_normal, float opacity, float reflexivity);
	// Constructor of triangle with uv mapping and smoothering
	Triangle(const Vector &A, const Vector &B, const Vector &C, const vec2 &A_uv, const vec2 &B_uv, const vec2 &C_uv, texture *t, const Vector &A_normal, const Vector &B_normal, const Vector &C_normal, float opacity, float reflexivity);
	~Triangle() {}

	color_rgba getColor(const Vector &point);
	Vector getNormal(const Vector &point, const Vector &direction) const;

	bool isIntersecting(const Vector &point, const Vector &direction, Vector &intersection) const;
};

#endif //TRIANGLE_HPP

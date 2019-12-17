#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "utils/vector.hpp"
#include "utils/color.hpp"

// Pure abstract class
class Light
{
  protected:
	Vector _vec;
	color_rgba _color;

  public:
	Light(const Vector &v, const color_rgba &c);
	~Light() {}

	color_rgba getColor() const;
	virtual Vector getDirectionToLight(const Vector &point) const = 0;

	virtual bool isIntersectionAfterLight(const Vector &A, const Vector &B) const = 0;
};

#endif //LIGHT_HPP

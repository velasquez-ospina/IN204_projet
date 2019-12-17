#ifndef SPOT_HPP
#define SPOT_HPP

#include "lights/light.hpp"
#include "utils/color.hpp"
#include "utils/vector.hpp"

// A light that is represented by a infinitisemal point
class Spot : public Light
{
  public:
	Spot(const Vector &vec, const color_rgba &c);

	Vector getDirectionToLight(const Vector &point) const;
	Vector getPosition() const;

	bool isIntersectionAfterLight(const Vector &A, const Vector &B) const;
};

#endif //SPOT_HPP

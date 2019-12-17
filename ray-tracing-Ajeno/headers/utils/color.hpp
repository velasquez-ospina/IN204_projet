#ifndef _TYPE_HPP_
#define _TYPE_HPP_

#include <array>

#include "utils/type.hpp"

// Substractive synthese
color_rgba subbSynthese(const color_rgba &obj, const color_rgba &li);
// Additive synthese
color_rgba addSynthese(const color_rgba &l1, const color_rgba &l2);

color_rgba colorFloor(const color_rgba &col);

color_rgba operator*(float lambda, const color_rgba &col);
color_rgba operator*(const color_rgba &col, float lambda);
color_rgba operator*(const color_rgba &col1, const color_rgba &col2);
color_rgba operator+(const color_rgba &col1, const color_rgba &col2);
color_rgba operator-(const color_rgba &col1, const color_rgba &col2);

std::ostream &operator<<(std::ostream &stream, const color_rgba &col);

#endif

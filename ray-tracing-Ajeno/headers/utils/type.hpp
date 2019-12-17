#ifndef TYPE_HPP
#define TYPE_HPP

#include <array>

typedef std::array<float, 2> vec2;
typedef std::array<float, 4> color_rgba;

struct Offset
{
	float right;
	float bottom;
	float weight;
};

#endif //TYPE_HPP

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <vector>
#include <array>

#include "utils/color.hpp"

// Class where the texture is loaded and sampled
// Only used with the uv mapping of a triangle or a set of triangles
class texture
{
  private:
	unsigned _height;
	unsigned _width;
	std::vector<color_rgba> _data;

  public:
	texture(std::string filepath);
	~texture() {}

	color_rgba sample(float u, float v) const;
};

#endif // TEXTURE_HPP

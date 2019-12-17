#include "utils/texture.hpp"
#include "includes/lodepng.hpp"

#include <iostream>
#include <math.h>

texture::texture(std::string filepath)
{
	std::vector<unsigned char> image;
	//decode
	unsigned error = lodepng::decode(image, _width, _height, filepath);

	//if there's an error, display it
	if (error)
		std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	_data.resize(_width * _height);
	for (size_t i = 0; i < image.size();)
	{
		color_rgba c = {(float)image[i], (float)image[i + 1], (float)image[i + 2], (float)image[i + 3]};
		_data[i / 4] = c;
		i += 4;
	}
}

color_rgba texture::sample(float u, float v) const
{
	unsigned col = floor(u * _width);
	unsigned row = floor(v * _height);
	return _data[_height * (_height - 1 - row) + col];
}

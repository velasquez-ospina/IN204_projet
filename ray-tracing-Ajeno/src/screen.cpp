#include <math.h>

#include "screen.hpp"

Screen::Screen(unsigned h, unsigned w, float s, const Vector &top_dir, const std::vector<vec2> &p) : _height(h), _width(w), _pixel_size(s)
{
	_data = std::vector<color_rgba>(h * w, {0, 0, 0});
	_top_dir = top_dir.normalize();
	setWeight(p);
}

void Screen::setPixelColor(int row, int col, const color_rgba &color)
{
	if (!(row >= (int)_width || row < 0 || col >= (int)_height || col < 0))
		_data[_height * col + row] = color;
}

unsigned Screen::getHeight() const
{
	return _height;
}

unsigned Screen::getWidth() const
{
	return _width;
}

float Screen::getPixelSize() const
{
	return _pixel_size;
}

color_rgba Screen::pixelAt(int row, int col) const
{
	if (row >= (int)_width || row < 0 || col >= (int)_height || col < 0)
		return {0, 0, 0};
	return _data[_height * col + row];
}

std::vector<Offset> Screen::getPixelsOffset() const
{
	return _pixels_offset;
}

void Screen::setWeight(const std::vector<vec2> &p)
{
	int size = p.size();
	_pixels_offset.resize(size);
	float norm = 0;
	for (int i = 0; i < size; i++)
	{
		float x = p[i][0] - 0.5;
		float y = p[i][1] - 0.5;
		float w = std::exp(-9 * (x * x + y * y));
		_pixels_offset[i] = {p[i][0], p[i][1], w};
		norm += w;
	}

	for (Offset &o : _pixels_offset)
	{
		o.weight /= norm;
	}
}

Vector Screen::pixelDirection(unsigned row, unsigned col, float offset_right, float offset_bottom, const Vector &center) const
{
	Vector _left_dir = center.crossProduct(_top_dir).normalize();
	float right_distance = row * _pixel_size + _pixel_size * offset_right - _width * _pixel_size / 2;
	float bottom_distance = col * _pixel_size + _pixel_size * offset_bottom - _height * _pixel_size / 2;
	Vector inner = _top_dir * (-bottom_distance) + _left_dir * (-right_distance);
	return inner + center;
}

void Screen::save(std::string output_name)
{
	std::ofstream ofs(output_name.c_str(), std::ios::out | std::ios::binary);
	ofs << "P6\n"
		<< _width << " " << _height << "\n255\n";
	for (unsigned i = 0; i < _width * _height; ++i)
	{
		ofs << (uint8_t)_data[i][0] << (uint8_t)_data[i][1] << (uint8_t)_data[i][2];
	}

	ofs.close();
}

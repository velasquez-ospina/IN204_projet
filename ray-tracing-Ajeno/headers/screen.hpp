#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <vector>
#include <fstream>

#include "utils/vector.hpp"
#include "utils/color.hpp"
#include "utils/type.hpp"

// Simulate the screen (so the pixels)
class Screen
{
  private:
	unsigned _height, _width;
	float _pixel_size;
	Vector _top_dir;
	std::vector<color_rgba> _data;
	std::vector<Offset> _pixels_offset;

	void setWeight(const std::vector<vec2> &p);

  public:
	Screen(unsigned h, unsigned w, float s, const Vector &top_dir, const std::vector<vec2> &p);
	~Screen() {}

	unsigned getHeight() const;
	unsigned getWidth() const;
	float getPixelSize() const;
	std::vector<Offset> getPixelsOffset() const;
	color_rgba pixelAt(int row, int col) const;

	Vector pixelDirection(unsigned row, unsigned col, float offset_right, float offset_bottom, const Vector &center) const;
	void setPixelColor(int row, int col, const color_rgba &color);

	void save(std::string output_name);
};

#endif //SCREEN_HPP

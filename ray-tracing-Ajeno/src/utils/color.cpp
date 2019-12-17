#include "utils/color.hpp"
#include <iostream>
#include <math.h>

color_rgba subbSynthese(const color_rgba &obj, const color_rgba &li)
{
	float r = (float)(obj[0] * li[0] / 255);
	float g = (float)(obj[1] * li[1] / 255);
	float b = (float)(obj[2] * li[2] / 255);
	return {r, g, b};
}

color_rgba addSynthese(const color_rgba &l1, const color_rgba &l2)
{
	float p0 = (l1[0] + l2[0] > 255) ? 255 : (l1[0] + l2[0]);
	float p1 = (l1[1] + l2[1] > 255) ? 255 : (l1[1] + l2[1]);
	float p2 = (l1[2] + l2[2] > 255) ? 255 : (l1[2] + l2[2]);
	return {p0, p1, p2};
}

color_rgba colorFloor(const color_rgba &col)
{
	return {(float)floor(col[0]), (float)floor(col[1]), (float)floor(col[2])};
}

color_rgba operator*(float lambda, const color_rgba &col)
{
	if (lambda > 1 || lambda < 0)
	{
		return col;
	}
	return {(float)(col[0] * lambda), (float)(col[1] * lambda), (float)(col[2] * lambda)};
}

color_rgba operator*(const color_rgba &col, float lambda)
{
	return lambda * col;
}

color_rgba operator*(const color_rgba &col1, const color_rgba &col2)
{
	float r = (float)(col1[0] * col2[0] / 255);
	float g = (float)(col1[1] * col2[1] / 255);
	float b = (float)(col1[2] * col2[2] / 255);
	return {r, g, b};
}

color_rgba operator+(const color_rgba &col1, const color_rgba &col2)
{
	return addSynthese(col1, col2);
}

color_rgba operator-(const color_rgba &col1, const color_rgba &col2)
{
	return {col1[0] - col2[0], col1[1] - col2[1], col1[2] - col2[2]};
}

std::ostream &operator<<(std::ostream &stream, const color_rgba &col)
{
	stream << '(' << unsigned(col[0]) << ',' << unsigned(col[1])
		   << ',' << unsigned(col[2])
		   << ')';
	return stream;
}

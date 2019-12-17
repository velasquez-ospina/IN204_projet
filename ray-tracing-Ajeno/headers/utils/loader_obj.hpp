#ifndef LOADER_OBJ_HPP
#define LOADER_OBJ_HPP

#include <vector>

#include "objects/object.hpp"

std::vector<Object *> loaderObj(std::string filename, const color_rgba &col, const Vector &translation, const Vector &rotation, const Vector &mag, float opacity, float reflexivity, bool enabled_smooth, bool enabled_uv, std::string texture_path = "");

#endif //LOADER_OBJ_HPP

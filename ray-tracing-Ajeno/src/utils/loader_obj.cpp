#include <fstream>
#include <sstream>
#include <iostream>

#include "utils/loader_obj.hpp"
#include "objects/triangle.hpp"

std::vector<Object *> loaderObj(std::string filename, const color_rgba &col, const Vector &translation, const Vector &rotation, const Vector &mag, float opacity, float reflexivity, bool enabled_smooth, bool enabled_uv, std::string texture_path)
{
	texture *tex = 0;
	if (enabled_uv)
		tex = new texture(texture_path);

	std::vector<Object *> objects;
	std::ifstream file(filename);
	if (file.is_open())
	{
		std::string line;
		// std::vector<Triangle *> objects;
		float X = 0, Y = 0, Z = 0;
		std::vector<Vector> vertices;
		std::vector<vec2> uvs;
		std::vector<Vector> normals;
		while (getline(file, line))
		{
			std::string type = line.substr(0, 2);
			if (type == "v ")
			{
				std::istringstream v(line.substr(2));
				float x, y, z;
				v >> x;
				v >> y;
				v >> z;
				vertices.push_back(Vector(x, y, z));
				X += x;
				Y += y;
				Z += z;
			}
			else if (type == "vt")
			{
				std::istringstream vt(line.substr(3));
				float u, v;
				vt >> u;
				vt >> v;
				uvs.push_back({u, v});
			}
			else if (type == "vn")
			{
				std::istringstream vn(line.substr(3));
				float x, y, z;
				vn >> x;
				vn >> y;
				vn >> z;
				normals.push_back(Vector(x, y, z));
			}
			else if (type == "f ")
			{
				std::stringstream ss(line.substr(2));
				std::vector<unsigned> index_vertices;
				std::vector<unsigned> index_uvs;
				std::vector<unsigned> index_normals;

				while (ss)
				{
					unsigned index_vertice;
					unsigned index_uv;
					unsigned index_normal;
					char token;
					ss >> index_vertice >> token >> index_uv >> token >> index_normal;

					index_vertices.push_back(index_vertice);
					index_uvs.push_back(index_uv);
					index_normals.push_back(index_normal);
				}

				Vector barycenter(X / vertices.size(), Y / vertices.size(), Z / vertices.size());

				//create triangle
				for (unsigned i = 1; i < index_vertices.size() - 1; ++i)
				{
					Vector a = vertices[index_vertices[0] - 1];
					a = (a - barycenter).rotate3D(rotation).magnify(mag) + translation + barycenter;
					Vector b = vertices[index_vertices[i] - 1];
					b = (b - barycenter).rotate3D(rotation).magnify(mag) + translation + barycenter;
					Vector c = vertices[index_vertices[i + 1] - 1];
					c = (c - barycenter).rotate3D(rotation).magnify(mag) + translation + barycenter;

					vec2 a_uv;
					vec2 b_uv;
					vec2 c_uv;
					if (enabled_uv)
					{
						a_uv = uvs[index_uvs[0] - 1];
						b_uv = uvs[index_uvs[i] - 1];
						c_uv = uvs[index_uvs[i + 1] - 1];
					}

					Vector a_normal;
					Vector b_normal;
					Vector c_normal;
					if (enabled_smooth)
					{
						a_normal = normals[index_normals[0] - 1];
						b_normal = normals[index_normals[i] - 1];
						c_normal = normals[index_normals[i + 1] - 1];
					}

					if (enabled_uv && enabled_smooth)
					{
						Triangle *t = new Triangle(a, b, c, a_uv, b_uv, c_uv, tex, a_normal, b_normal, c_normal, opacity, reflexivity);
						objects.push_back(t);
					}
					else if (enabled_uv)
					{
						Triangle *t = new Triangle(a, b, c, a_uv, b_uv, c_uv, tex, opacity, reflexivity);
						objects.push_back(t);
					}
					else if (enabled_smooth)
					{
						Triangle *t = new Triangle(a, b, c, a_normal, b_normal, c_normal, opacity, reflexivity);
						objects.push_back(t);
					}
					else
					{
						Triangle *t = new Triangle(a, b, c, col, opacity, reflexivity);
						objects.push_back(t);
					}
				}
			}
		}
		return objects;
	}
	else
	{
		std::cerr << "Cannot open " << filename << std::endl;
		exit(-1);
	}
}

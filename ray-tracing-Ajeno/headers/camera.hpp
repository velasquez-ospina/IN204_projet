#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "utils/vector.hpp"

// Simulate a camera
class Camera
{
  private:
	Vector _position;
	Vector _direction;
	float _dist_normal; // Minimal distance to the screen (usually 1)

  public:
	Camera(const Vector &pos, const Vector &dir, float dist_n);
	~Camera() {}

	Vector getDirection() const;
	Vector getPosition() const;
	float getDistance() const;
};

#endif //CAMERA_HPP

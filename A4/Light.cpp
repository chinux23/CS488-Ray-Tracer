#include <iostream>

#include <glm/ext.hpp>

#include "Light.hpp"

Light::Light()
  : colour(0.0, 0.0, 0.0),
    position(0.0, 0.0, 0.0)
{
  falloff[0] = 1.0;
  falloff[1] = 0.0;
  falloff[2] = 0.0;
}

Light::Light(glm::vec3 position, glm::vec3 color) : colour(color), position(position)
{
	falloff[0] = 1.0;
	falloff[1] = 0.0;
	falloff[2] = 0.0;
}

std::ostream& operator<<(std::ostream& out, const Light& l)
{
  out << "L[" << glm::to_string(l.colour) 
  	  << ", " << glm::to_string(l.position) << ", ";
  for (int i = 0; i < 3; i++) {
    if (i > 0) out << ", ";
    out << l.falloff[i];
  }
  out << "]";
  return out;
}

glm::dvec3 Light::randomPoint()
{
	return position;	// for point light, it's always the original point.
}


SphericalLight::SphericalLight(glm::vec3 position, glm::vec3 color, double radius) :
AreaLight(position, color),
_radius(radius)
{
	m_primitive = NonhierSphere(position, _radius);
}

glm::dvec3 SphericalLight::randomPoint()
{
	glm::dvec3 vector = glm::sphericalRand(_radius);
	glm::dvec3 point = glm::dvec3(position) + vector;
	return point;
}

AreaLight::AreaLight(glm::vec3 position, glm::vec3 color) : Light(position, color)
{
	
}

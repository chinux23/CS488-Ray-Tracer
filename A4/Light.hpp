#pragma once

#include <iosfwd>

#include <glm/glm.hpp>
#include "Primitive.hpp"

// Represents a simple point light.
struct Light {
	Light();
	Light(glm::vec3 color, glm::vec3 position);
  
	glm::vec3 colour;
	glm::vec3 position;
	double falloff[3];
	
	// Return a random point in the light source.
	virtual glm::dvec3 randomPoint();
	virtual bool isAreaLight() {return false;};
};

std::ostream& operator<<(std::ostream& out, const Light& l);


struct AreaLight : Light {
	AreaLight(glm::vec3 position, glm::vec3 color);
	virtual bool isAreaLight() {return true;};
	Primitive m_primitive;
	virtual glm::dvec3 randomPoint() = 0;
};

struct SphericalLight : AreaLight {
	SphericalLight(glm::vec3 position, glm::vec3 color, double radius);
	virtual glm::dvec3 randomPoint();
	
	double _radius;
};

struct BoxLight : AreaLight {
	BoxLight();
	virtual glm::dvec3 randomPoint();
};

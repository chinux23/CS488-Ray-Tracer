#pragma once

#include <glm/glm.hpp>
#include "Ray.hpp"
#include "Intersection.hpp"

class Primitive {
public:
	virtual ~Primitive();
	virtual Intersection intersect(const Ray & ray);
};


class Sphere : public Primitive {
public:
	virtual ~Sphere();
	virtual Intersection intersect(const Ray & ray);
};


class Cube : public Primitive {
public:
	virtual ~Cube();
	virtual Intersection intersect(const Ray & ray);
};


class NonhierSphere : public Primitive {
public:
	NonhierSphere(const glm::vec3& pos, double radius)
		: m_pos(pos), m_radius(radius)
	{
	}
	virtual ~NonhierSphere();
	virtual Intersection intersect(const Ray & ray);

private:
	glm::vec3 m_pos;
	double m_radius;
};


class NonhierBox : public Primitive {
public:
	NonhierBox(const glm::vec3& pos, double size)
		: m_pos(pos), m_size(size)
	{
	}
  
	virtual ~NonhierBox();
	virtual Intersection intersect(const Ray & ray);

private:
	glm::vec3 m_pos;
	double m_size;
};

#pragma once

#include <glm/glm.hpp>
#include "Ray.hpp"
#include "Intersection.hpp"
#include <list>
#include <glm/glm.hpp>

#define EPSILON 1

template<typename T1, typename T2>
bool isEqual(const T1 & t1, const T2 & t2) {
    if (std::abs(t1 - t2) < EPSILON) {
        return true;
    } else {
        return false;
    }
}

class NonhierSphere;
class NonhierBox;


class Primitive {
public:
	virtual ~Primitive();
	virtual Intersection intersect(const Ray & ray);
	virtual Intersection intersect(const Ray & ray, std::list<glm::mat4> transformations);
protected:
	Primitive *m_primitive;
};


class Sphere : public Primitive {
public:
	Sphere();
	virtual ~Sphere();
	virtual Intersection intersect(const Ray & ray);
//	virtual Intersection intersect(const Ray & ray, std::list<glm::mat4> transformations);

};


class Cube : public Primitive {
public:
	Cube();
	virtual ~Cube();
	virtual Intersection intersect(const Ray & ray);
//	virtual Intersection intersect(const Ray & ray, std::list<glm::mat4> transformations);
};


class NonhierSphere : public Primitive {
public:
	NonhierSphere(const glm::vec3& pos, double radius)
		: m_pos(pos), m_radius(radius)
	{
	}
	virtual ~NonhierSphere();
	virtual Intersection intersect(const Ray & ray);
	virtual Intersection intersect(const Ray & ray, std::list<glm::mat4> transformations);

private:
	glm::vec3 m_pos;
	double m_radius;
};


class NonhierBox : public Primitive {
public:
	NonhierBox(const glm::vec3& pos, double size)
	: m_pos(pos), m_size(size), m_pos2(size * glm::vec3(1,1,1))
	{
	}
  
	virtual ~NonhierBox();
	virtual Intersection intersect(const Ray & ray);
	virtual Intersection intersect(const Ray & ray, std::list<glm::mat4> transformations);

private:
	glm::vec3 m_pos;
	double m_size;
	glm::vec3 m_pos2;
};

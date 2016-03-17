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
	virtual bool isOpticsEnabled() {return false;};
protected:
	Primitive *m_primitive;
};


class Sphere : public Primitive {
public:
	Sphere();
	virtual ~Sphere();
	virtual Intersection intersect(const Ray & ray);
//	virtual Intersection intersect(const Ray & ray, std::list<glm::mat4> transformations);
	virtual bool isOpticsEnabled() {return false;};

};


class Cube : public Primitive {
public:
	Cube();
	virtual ~Cube();
	virtual Intersection intersect(const Ray & ray);
//	virtual Intersection intersect(const Ray & ray, std::list<glm::mat4> transformations);
};

// Create standard cone with Centered at Origin, H = 2 (spanning from -1 to 1), R = 1
class Cone : public Primitive {
public:
	Cone();
    virtual ~Cone();
    virtual Intersection intersect(const Ray & ray);
};

class Cylinder : public Primitive {
public:
    Cylinder();
    virtual ~Cylinder();
    virtual Intersection intersect(const Ray & ray);
    bool    isValidRoot(const Ray &ray, double t);
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

// Create a x from -1 to 1, z from -1 to 1, y = 0 plane
class Plane : public Primitive {
	virtual Intersection intersect(const Ray & ray);
	virtual bool isOpticsEnabled() {return true;};
};

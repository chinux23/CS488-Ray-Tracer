#include "Primitive.hpp"
#include "polyroots.hpp"

Primitive::~Primitive()
{
}

Intersection Primitive::intersect(const Ray &ray)
{
	Intersection result(ray, 0);
	return result;
}

Sphere::~Sphere()
{
}

Cube::~Cube()
{
}

NonhierSphere::~NonhierSphere()
{
}

Intersection Sphere::intersect(const Ray & ray)
{
	Intersection result(ray, 0);
	return result;
}

Intersection Cube::intersect(const Ray &ray)
{
	Intersection result(ray, 0);
	return result;
}

Intersection NonhierBox::intersect(const Ray &ray)
{
	Intersection result(ray, 0);
	return result;
}

Intersection NonhierSphere::intersect(const Ray &ray)
{
	Intersection result(ray, 0);
	glm::dvec4 c(m_pos.x, m_pos.y, m_pos.z, 1);
	const glm::dvec4 & l = ray.direction;
	const glm::dvec4 & o = ray.origin;
	const glm::dvec4 & o_c = o - c;
	
	double A = glm::dot(l, l);
	double B = 2 * glm::dot(l, o_c);
	double C = glm::dot(o_c, o_c) - m_radius;
	double roots[2];
	size_t num_of_roots = quadraticRoots(A, B, C, roots);
	
	if (num_of_roots == 0) {
		// No roots, miss.
		result.hit = false;
	
	} else if (num_of_roots == 1) {
		result.hit = true;
		result.t = roots[0];
		
	} else if (num_of_roots == 2) {
		result.hit = true;
		
		if (std::abs(roots[0]) > std::abs(roots[1])) {
			result.t = roots[0];
		} else {
			result.t = roots[1];
		}
	} else {
		// Should never happen here.
		assert(false);
	}
	
	// update normal
	glm::dvec4 point = ray.origin + ray.direction * result.t;
	glm::dvec4 normal = glm::normalize(point - c);
	result.normal = normal;
	
	return result;
}

NonhierBox::~NonhierBox()
{
}

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


// Reference: http://www.cs.utah.edu/~awilliam/box/box.pdf
Intersection NonhierBox::intersect(const Ray &r)
{
	Intersection result(r, 0);
	glm::dvec3 bounds[2] = {m_pos, m_pos + m_size * glm::vec3(1, 1, 1)};
	double tmin, tmax, tymin, tymax, tzmin, tzmax;
	
	if (r.direction.x >= 0) {
		tmin = (bounds[0].x - r.origin.x) / r.direction.x;
		tmax = (bounds[1].x - r.origin.x) / r.direction.x;
	} else {
		tmin = (bounds[1].x - r.origin.x) / r.direction.x;
		tmax = (bounds[0].x - r.origin.x) / r.direction.x;
	}
	if (r.direction.y >= 0) {
		tymin = (bounds[0].y - r.origin.y) / r.direction.y;
		tymax = (bounds[1].y - r.origin.y) / r.direction.y;
	} else {
		tymin = (bounds[1].y - r.origin.y) / r.direction.y;
		tymax = (bounds[0].y - r.origin.y) / r.direction.y;
	}
	if ( (tmin > tymax) || (tymin > tmax) ) {
		result.hit = false;
		return result;
	}
	
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	if (r.direction.z >= 0) {
		tzmin = (bounds[0].z - r.origin.z) / r.direction.z;
		tzmax = (bounds[1].z - r.origin.z) / r.direction.z;
	} else {
		tzmin = (bounds[1].z - r.origin.z) / r.direction.z;
		tzmax = (bounds[0].z - r.origin.z) / r.direction.z;
	}
	if ( (tmin > tzmax) || (tzmin > tmax) ) {
		result.hit = false;
		return result;
	}
	
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;
	
	if ( (tmin < 1 ) && (tmax > 0) && tmin > 0) {
		result.hit = true;
		result.t = tmin;
	}
    
    glm::dvec4 hitPoint = r.origin + r.direction * result.t;
    
    // Figure out which face it's hitting.
    if (isEqual(hitPoint.x, m_pos.x)) {
        result.normal = {-1, 0, 0, 0};
        
    } else if (isEqual(hitPoint.x, m_pos.x + m_size)) {
        result.normal = {1, 0, 0, 0};
        
    } else if (isEqual(hitPoint.y, m_pos.y)) {
        result.normal = {0, -1, 0, 0};
        
    } else if (isEqual(hitPoint.y, m_pos.y + m_size)) {
        result.normal = {0, 1, 0, 0};
        
    } else if (isEqual(hitPoint.z, m_pos.z)) {
        result.normal = {0, 0, -1, 0};
        
    } else if (isEqual(hitPoint.z, m_pos.z + m_size)) {
        result.normal = {0, 0, 1, 0};
    } else {
        // We should not be here.
        assert(false);
    }
	
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
	double C = glm::dot(o_c, o_c) - m_radius * m_radius;
	double roots[2];
	size_t num_of_roots = quadraticRoots(A, B, C, roots);
	
	if (num_of_roots == 0) {
		// No roots, miss.
		result.hit = false;
	} else if (num_of_roots == 1) {
		result.hit = roots[0] > 0;
		result.t = roots[0];
	} else if (num_of_roots == 2) {
//        auto hitpoint = ray.origin + ray.direction * roots[0];
        
		// Return the smallest positive number
		result.t = std::min(roots[0], roots[1]);
		result.hit = result.t > 0;
	} else {
		// Should never happen here.
		assert(false);
	}
	
	if (result.hit) {
		assert(result.t > 0);
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

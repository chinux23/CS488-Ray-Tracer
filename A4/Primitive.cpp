#include "Primitive.hpp"
#include "polyroots.hpp"
#include "Mesh.hpp"


Primitive::~Primitive()
{
}

Intersection Primitive::intersect(const Ray &ray)
{
	Intersection result(ray, 0);
	return result;
}

Intersection Primitive::intersect(const Ray & ray, std::list<glm::mat4> transformations)
{
	assert(m_primitive);
	auto result = m_primitive->intersect(ray, transformations);
	return result;
}

Sphere::Sphere()
{
	m_primitive = new NonhierSphere({0, 0, 0}, 1.0);
}

Sphere::~Sphere()
{
	delete m_primitive;
}

Cube::Cube()
{
	m_primitive = new NonhierBox({0, 0, 0}, 1.0);
}

Cube::~Cube()
{
	delete m_primitive;
}

NonhierSphere::~NonhierSphere()
{
}

Intersection Sphere::intersect(const Ray & ray)
{
	Intersection result = m_primitive->intersect(ray);
	return result;
}

Intersection Cube::intersect(const Ray &ray)
{
	Intersection result = m_primitive->intersect(ray);
	return result;
}

// Reference: http://www.cs.utah.edu/~awilliam/box/box.pdf
Intersection NonhierBox::intersect(const Ray &r)
{
	Intersection result(r, 0);
    std::vector<glm::vec3> vertices = {
        m_pos + glm::vec3(0, 0, m_size),
        m_pos + glm::vec3(m_size, 0, m_size),
        m_pos + glm::vec3(m_size, m_size, m_size),
        m_pos + glm::vec3(0, m_size, m_size),
        
        m_pos + glm::vec3(0, 0, 0),
        m_pos + glm::vec3(m_size, 0, 0),
        m_pos + glm::vec3(m_size, m_size, 0),
        m_pos + glm::vec3(0, m_size, 0)
    };
    
    std::vector<Triangle> faces = {
        Triangle(2, 3, 0),
        Triangle(0, 1, 2),
        Triangle(2, 1, 5),
        Triangle(5, 6, 2),
        Triangle(3, 7, 4),
        Triangle(4, 0, 3),
        Triangle(7, 6, 5),
        Triangle(5, 4, 7),
        
        Triangle(1, 0, 4),
        Triangle(4, 5, 1),
        Triangle(2, 6, 7),
        Triangle(7, 3, 2)
    };
	
	glm::vec3 baryPosition;
	for (auto triangle : faces) {
		auto o = glm::vec3(r.origin);
		auto d = glm::vec3(r.direction);
		
		bool hit = glm::intersectRayTriangle(o, d,
											 vertices[triangle.v1],
											 vertices[triangle.v2],
											 vertices[triangle.v3],
											 baryPosition);
		
		if (hit && baryPosition.z > 0) {
			if (!result.hit) {
				result.t = baryPosition.z;
				result.hit = true;
				auto normal = -glm::normalize(
											  glm::cross(
														 vertices[triangle.v3] - vertices[triangle.v1],
														 vertices[triangle.v2] - vertices[triangle.v1]));
				result.normal = glm::dvec4(normal, 0);
			} else if (baryPosition.z < result.t) {
				result.t = baryPosition.z;
				auto normal = -glm::normalize(
											  glm::cross(
														 vertices[triangle.v3] - vertices[triangle.v1],
														 vertices[triangle.v2] - vertices[triangle.v1]));
				result.normal = glm::dvec4(normal, 0);
			}
		} else {
			if (hit && baryPosition.z < 0) {
				assert("Got normal problem?");
			}
		}
	}
	return result;
}

Intersection NonhierBox::intersect(const Ray & ray, std::list<glm::mat4> transformations)
{
	return intersect(ray);
}

Intersection NonhierSphere::intersect(const Ray & ray, std::list<glm::mat4> transformations)
{
	return intersect(ray);
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
	
    // are we inside the sphere? if we are, the normal will be the negative of the normal computed above.
    if ( glm::distance(glm::vec3(ray.origin), m_pos) < m_radius) {
        // we are inside.
        result.normal = -result.normal;
    }
    
	return result;
}



NonhierBox::~NonhierBox()
{

}

Cone::Cone()
{
    
}

Cone::~Cone()
{
    
}

Intersection Cone::intersect(const Ray &ray)
{
    Intersection result(ray, 0);
	double A = ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z - ray.direction.y * ray.direction.y;
	double B = 2 * (ray.origin.x * ray.direction.x + ray.origin.z * ray.direction.z - ray.origin.y * ray.direction.y);
	double C = ray.origin.x * ray.origin.x + ray.origin.z * ray.origin.z - ray.origin.y * ray.origin.y;
	
	double roots[2];
	size_t num_of_roots = quadraticRoots(A, B, C, roots);
	
	if (num_of_roots == 0) {
		// No roots, miss.
		result.hit = false;
		
	} else if (num_of_roots == 1) {
		double t = roots[0];
		double y = ray.origin.y + ray.direction.y * t;
		if (y <= 1 && y >= 0) {
			result.t = roots[0];
			result.hit = true;
			
			// calculate normal
			glm::dvec4 hitpoint = ray.origin + ray.direction * t;
			double x2_z2_sqrt = sqrt( hitpoint.x * hitpoint.x + hitpoint.z * hitpoint.z );
			glm::dvec3 p_x {1.0, hitpoint.x / x2_z2_sqrt, 0.0};
			glm::dvec3 p_z {0.0, hitpoint.z / x2_z2_sqrt, 1.0};
			glm::dvec3 normal = glm::cross(p_x, p_z);
			result.normal = glm::dvec4(glm::normalize(normal), 0);
			
		} else {
			result.hit = false;
		}
		
	} else if (num_of_roots == 2) {
		double t1 = roots[0];
		double t2 = roots[1];
		
		glm::dvec4 p1 = ray.origin + ray.direction * t1;
		glm::dvec4 p2 = ray.origin + ray.direction * t2;
		
		if (t1 > t2) {
			// Make sure z1 is smaller than z2.
			std::swap(t1, t2);
			std::swap(p1, p2);
		}
		
		if (p1.y < 0) {
			if (p2.y < 0) {
				// Missed
				result.hit = false;
			} else {
				// if p2.y is above 0, we need to see if p2 is a valid hitpoint or not.
				if (p2.y <= 1 && p2.y >= 0) {
					if (t2 > 0) {
						result.hit = true;
						result.t = t2;
						
						glm::dvec4 hitpoint = ray.origin + ray.direction * t2;
						double x2_z2_sqrt = sqrt( hitpoint.x * hitpoint.x + hitpoint.z * hitpoint.z );
						glm::dvec3 p_x {1.0, hitpoint.x / x2_z2_sqrt, 0.0};
						glm::dvec3 p_z {0.0, hitpoint.z / x2_z2_sqrt, 1.0};
						glm::dvec3 normal = glm::cross(p_x, p_z);
						result.normal = glm::dvec4(glm::normalize(normal), 0);
						
					} else {
						result.hit = false;
					}
				} else {
					result.hit = false;
				}
				
			}
			
		} else if (p1.y >= 0 && p1.y <= 1) {
			// Hit the cone.
			if (t1 <= 0) {
				result.hit = false;
			} else {
				result.hit = true;
				result.t = t1;
				
				auto hitpoint = ray.origin + ray.direction * t1;
				double x2_z2_sqrt = sqrt( hitpoint.x * hitpoint.x + hitpoint.z * hitpoint.z );
				glm::dvec3 p_x {1.0, hitpoint.x / x2_z2_sqrt, 0.0};
				glm::dvec3 p_z {0.0, hitpoint.z / x2_z2_sqrt, 1.0};
				glm::dvec3 normal = glm::cross(p_x, p_z);
				result.normal = glm::dvec4(glm::normalize(normal), 0);
			}
			
		} else if (p1.y > 1) {
			if (p2.y > 1) {
				result.hit = false;
			} else {
				if (p2.y <= 1 && p2.y >= 0) {
					if (t2 > 0) {
						// Hit the top cap
						double th = ( 1 - ray.origin.y ) / ray.direction.y;
						auto hitpoint = ray.origin + ray.direction * th;
						
						if (th >= 0 && (hitpoint.x <= 1 && hitpoint.x >= -1) && (hitpoint.z <= 1 && hitpoint.z >= -1)) {
							result.hit = true;
							result.t = th;
							result.normal = glm::dvec4(0, 1, 0, 0);
						} else {
							result.hit = false;
						}
						
					} else {
						result.hit = false;
					}
					
				} else {
					result.hit = false;
				}
				
			}
		}
		
	}
	
    return result;
}

Cylinder::Cylinder()
{
	
}

Cylinder::~Cylinder()
{
	
}


Intersection Cylinder::intersect(const Ray &ray)
{
    Intersection result(ray, 0);

    // X^2 + Z^2 = 1    (r == 1)
    // y_max = 1, y_min = -1
    
    double A = ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z;
    double B = 2 * ray.origin.x * ray.direction.x + 2 * ray.origin.z * ray.direction.z;
    double C = ray.origin.x * ray.origin.x + ray.origin.z * ray.origin.z - 1;
    
    double roots[2];
    size_t num_of_roots = quadraticRoots(A, B, C, roots);

    if (num_of_roots == 0) {
        // No roots, miss.
        result.hit = false;
        
    } else if (num_of_roots == 1) {
        double t = roots[0];
        double y = ray.origin.y + ray.direction.y * t;
        if (y <= 1 && y >= -1) {
            result.t = roots[0];
            result.hit = true;
			
			auto hitpoint = ray.origin + ray.direction * t;
			result.normal = glm::normalize(glm::dvec4(hitpoint.x, 0, hitpoint.z, 0));
			
        } else {
            result.hit = false;
        }
        
    } else if (num_of_roots == 2) {
        // If both are > 1, miss
        // if both are < -1, miss
        // If one is > 1, one is < 1, hit the top cap and side,
        // if one is < -1, one is > -1, hit the bottom cap, and side
        
        double t1 = roots[0];
        double t2 = roots[1];
        
        double y1 = ray.origin.y + ray.direction.y * t1;
        double y2 = ray.origin.y + ray.direction.y * t2;
        
        if (t1 > t2) {
            // Make sure z1 is smaller than z2.
            std::swap(t1, t2);
            std::swap(y1, y2);
        }
		
		if (y1 < -1) {
			
			if (y2 < -1) {
				result.hit = false;
			} else {
				// hit the bottom cap
				float th = t1 + (t2 - t1) * (y1 + 1) / (y1 - y2);
				if (th <= 0)
					result.hit = false;
				else
					result.hit = true;
				result.t = th;
				result.normal = glm::dvec4(0, -1, 0, 0);
			}
			
		} else if (y1 >= -1 && y1 <= 1) {
			// Hit the cylinder bit.
			if (t1 <= 0) {
				result.hit = false;
			} else {
				result.hit = true;
				result.t = t1;
				
				auto hitpoint = ray.origin + ray.direction * t1;
				result.normal = glm::normalize(glm::dvec4(hitpoint.x, 0, hitpoint.z, 0));
			}
			
		} else if (y1 > 1) {
			if (y2 > 1) {
				result.hit = false;
			} else {
				// hit the top cap
				float th = t1 + (t2 - t1) * (y1 - 1) / (y1 - y2);
				if (th <= 0)
					result.hit = false;
				else
					result.hit = true;
				
				result.t = th;
				result.normal = glm::dvec4(0, 1, 0, 0);
			}
		}
	}
	
    
    return result;
}

bool Cylinder::isValidRoot(const Ray &ray, double t)
{
    // calculate Z
    double z = ray.origin.z + ray.direction.z * t;
    
    if ( z <= 1 && z >= -1 ) {
        return true;
    } else {
        return false;
    }
}

Intersection Plane::intersect(const Ray &ray)
{
	double t = - ray.origin.y / ray.direction.y;
	auto hitPoint = ray.origin + ray.direction * t;
	
	Intersection intersect(ray, 0);
	
	if (hitPoint.x <= 1 && hitPoint.x >= -1 && hitPoint.z <= 1 && hitPoint.z >= -1 && t > 0) {
		intersect.t = t;
		intersect.hit = true;
		if (ray.origin.y < 0) {
			intersect.normal = glm::dvec4(0, -1, 0, 0);
		} else {
			intersect.normal = glm::dvec4(0, 1, 0, 0);
		}
		
	} else {
		intersect.hit = false;
	}
	
	return intersect;
}

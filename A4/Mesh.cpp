#include <iostream>
#include <fstream>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"

Mesh::Mesh( const std::string& fname )
	: m_vertices()
	, m_faces()
{
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;

	std::ifstream ifs( fname.c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}
}

Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<Triangle> faces)
:m_faces(faces), m_vertices(vertices)
{
    
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  /*
  
  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
  	const MeshVertex& v = mesh.m_verts[idx];
  	out << glm::to_string( v.m_position );
	if( mesh.m_have_norm ) {
  	  out << " / " << glm::to_string( v.m_normal );
	}
	if( mesh.m_have_uv ) {
  	  out << " / " << glm::to_string( v.m_uv );
	}
  }

*/
  out << "}";
  return out;
}

Intersection Mesh::intersect(const Ray &r)
{
	Intersection result(r, 0);
	
	glm::vec3 baryPosition;
	for (auto triangle : m_faces) {
		auto o = glm::vec3(r.origin);
		auto d = glm::vec3(r.direction);
		
		bool hit = glm::intersectRayTriangle(o, d,
											 m_vertices[triangle.v1],
											 m_vertices[triangle.v2],
											 m_vertices[triangle.v3],
											 baryPosition);
		if (hit && baryPosition.z > 0) {
			if (!result.hit) {
				result.t = baryPosition.z;
				result.hit = true;
				auto normal = -glm::normalize(
									glm::cross(
									  m_vertices[triangle.v3] - m_vertices[triangle.v1],
									  m_vertices[triangle.v2] - m_vertices[triangle.v1]));
				result.normal = glm::dvec4(normal, 0);
			} else if (baryPosition.z < result.t) {
				result.t = baryPosition.z;
				auto normal = -glm::normalize(
											 glm::cross(
														m_vertices[triangle.v3] - m_vertices[triangle.v1],
														m_vertices[triangle.v2] - m_vertices[triangle.v1]));
				result.normal = glm::dvec4(normal, 0);
			}
		}
	}
	return result;
}

Intersection Mesh::intersect(const Ray & ray, std::list<glm::mat4> transformations)
{
	glm::mat4 total_trans;
	
	for (auto tranform : transformations) {
		total_trans = tranform * total_trans;
	}
	
	// Do inverse transform of the ray, then test intersection.
	glm::mat4 inv_total_trans = glm::inverse(total_trans);
	
	auto origin = inv_total_trans * ray.origin;
	auto dir	= inv_total_trans * ray.direction;
	
	Ray new_ray(origin, dir);
	
	auto i = intersect(new_ray);
	
	if (i.hit) {
		// Once hit, transform normal and incoming ray back to the world coordinates.
		i.normal = total_trans * i.normal;
		i.incoming_ray = ray;
	}
	return i;
}

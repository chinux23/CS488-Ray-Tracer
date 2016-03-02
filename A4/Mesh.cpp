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
	
	bool set = false;
	double max_x, max_y, max_z;
	double min_x, min_y, min_z;

	std::ifstream ifs( fname.c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			if (!set) {
				max_x = vx;
				max_y = vy;
				max_z = vz;
				
				min_x = vx;
				min_y = vy;
				min_z = vz;
				
				set = true;
			} else {
				max_x = std::max(vx, max_x);
				max_y = std::max(vy, max_y);
				max_z = std::max(vz, max_z);
				
				min_x = std::min(vx, min_x);
				min_y = std::min(vy, min_y);
				min_z = std::min(vz, min_z);
			}
			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}
	
	double min_v = std::min(std::min(min_y, min_z), min_x);
	double max_v = std::max(std::max(max_y, max_z), max_x);
	
	// Create a bounding volume
	if (fname == "plane.obj" || fname == "Assets/plane.obj") {
		m_boundingVolume = NULL;
	} else
		m_boundingVolume = new NonhierBox(glm::vec3(min_v, min_v, min_v), max_v - min_v);
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
	Intersection boundingVolumeHit(r, 0);
	if (m_boundingVolume) {
		boundingVolumeHit = m_boundingVolume->intersect(r);
	}

	Intersection result(r, 0);
	
	if (!m_boundingVolume || boundingVolumeHit.hit) {
	
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
			} else {
				if (hit && baryPosition.z < 0) {
					std::cout << "Got normal problem? " << std::endl;
				}
			}
		}
		
	}
	return result;
}

Intersection Mesh::intersect(const Ray & ray, std::list<glm::mat4> transformations)
{
	return intersect(ray);
}

#include "GeometryNode.hpp"

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, Material *mat )
	: SceneNode( name )
	, m_material( mat )
	, m_primitive( prim )
{
	m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial( Material *mat )
{
	// Obviously, there's a potential memory leak here.  A good solution
	// would be to use some kind of reference counting, as in the 
	// C++ shared_ptr.  But I'm going to punt on that problem here.
	// Why?  Two reasons:
	// (a) In practice we expect the scene to be constructed exactly
	//     once.  There's no reason to believe that materials will be
	//     repeatedly overwritten in a GeometryNode.
	// (b) A ray tracer is a program in which you compute once, and 
	//     throw away all your data.  A memory leak won't build up and
	//     crash the program.

	m_material = mat;
}

Intersection GeometryNode::intersect(const Ray & ray)
{
	auto origin = invtrans * ray.origin;
	auto dir	= invtrans * ray.direction;
	
	Ray new_ray(origin, dir);
	
	Intersection i = m_primitive->intersect(ray);
	if (i.hit) {
		// update material at the point of the hit
		i.material = (PhongMaterial *)m_material;
		i.node = this;
	}
	return i;
}

Intersection GeometryNode::intersect(const Ray & ray, std::list<glm::mat4> transformations)
{
	auto origin = invtrans * ray.origin;
	auto dir	= invtrans * ray.direction;
	
//	if (m_name == "plane") {
////		std::cout << "plane" << std::endl;
//		// Add additional rotation around X for 180 degree
//		glm::mat4 rotation = glm::rotate(glm::radians(180.0f), glm::vec3(0.0,0.0,1.0));
//		origin = glm::inverse(rotation) * origin;
//		dir = glm::inverse(rotation) * dir;
//	}

	Ray new_ray(origin, dir);
	
	Intersection i = m_primitive->intersect(new_ray);
	
	if (i.hit) {
		// get the materials as usual
		i.material = (PhongMaterial *)m_material;
		i.node = this;
	}
	
	// Test with all remaning child, and return the closet intersection point.
	for (auto child : children) {
		Intersection child_i = child->intersect(new_ray, transformations);
		
		if (child_i.hit) {
			if (!i.hit || child_i.t < i.t) {
				i = child_i;
			}
		}
	}
	
	if (i.hit) {
		// Once hit, transform normal and incoming ray back to the world coordinates.
//		if (m_name == "plane") {
////			std::cout << "hit plane " << i.hit << std::endl;
//			glm::mat4 rotation = glm::rotate(glm::radians(180.0f), glm::vec3(0.0,0.0,1.0));
//			i.normal = glm::transpose(glm::inverse(rotation) * invtrans) * i.normal;
//		} else {
			i.normal = glm::transpose(invtrans) * i.normal;
//		}
		i.incoming_ray = ray;
	}
	return i;
}


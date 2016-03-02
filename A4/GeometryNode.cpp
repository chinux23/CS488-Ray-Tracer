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
	transformations.push_back(trans);
	
	glm::mat4 total_trans;
	
	for (auto tranform : transformations) {
		total_trans = tranform * total_trans;
	}
	
	// Do inverse transform of the ray, then test intersection.
	glm::mat4 inv_total_trans = glm::inverse(total_trans);
	
	auto origin = inv_total_trans * ray.origin;
	auto dir	= inv_total_trans * ray.direction;
	
	Ray new_ray(origin, dir);
	
	Intersection i = m_primitive->intersect(new_ray);
	
	if (i.hit) {
		// Once hit, transform normal and incoming ray back to the world coordinates.
		i.normal = total_trans * i.normal;
		i.incoming_ray = ray;
		
		// get the materials as usual
		i.material = (PhongMaterial *)m_material;
		i.node = this;
	}
	
	// Test with all remaning child, and return the closet intersection point.
	for (auto child : children) {
		Intersection child_i = child->intersect(ray, transformations);
		
		if (child_i.hit) {
			if (!i.hit || child_i.t < i.t) {
				i = child_i;
			}
		}
	}
	
	return i;
}


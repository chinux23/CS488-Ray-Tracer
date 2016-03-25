#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"
#include "Texture.hpp"


class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim, 
		Material *mat = nullptr );

	void setMaterial( Material *material );

	Material *m_material;
	Primitive *m_primitive;
    Texture   *m_texture;
	
	virtual Intersection intersect(const Ray & ray);
	
	virtual Intersection intersect(const Ray & ray, std::list<glm::mat4> transformations);
	
	virtual bool isOpticsEnabled();
	
};



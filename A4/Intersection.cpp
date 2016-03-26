//
//  Intersection.cpp
//  CS488-Projects
//
//  Created by Chen Huang on 2/28/16.
//  Copyright © 2016 none. All rights reserved.
//

#include "Intersection.hpp"
#include "SceneNode.hpp"
#include "GeometryNode.hpp"

Intersection::Intersection(const Ray & ray, double t, bool hit) :
	incoming_ray(ray), t(t), hit(hit)
{
	
}

Intersection::Intersection(const Ray & ray, double t)
	:incoming_ray(ray), t(t), hit(false)
{
	
}

Intersection::Intersection() :
	incoming_ray({glm::vec4(),
	glm::vec4()}),
	t(0),
	hit(false)
{
	
}

Texture* Intersection::getTexture() const
{
    if (this->node->m_nodeType == NodeType::GeometryNode) {
        auto node = dynamic_cast<GeometryNode*>(this->node);
        return node->m_texture;
    } else {
        return nullptr;
    }
}


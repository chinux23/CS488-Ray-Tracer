//
//  Intersection.hpp
//  CS488-Projects
//
//  Created by Chen Huang on 2/28/16.
//  Copyright © 2016 none. All rights reserved.
//

#ifndef Intersection_hpp
#define Intersection_hpp

#include <stdio.h>
#include <glm/ext.hpp>
#include <vector>
#include "Ray.hpp"
#include "PhongMaterial.hpp"

class SceneNode;


struct Intersection {

	// Incoming ray
	Ray incoming_ray;
	
//	// Reflected outgoing rays
//	std::vector<glm::dvec4> outgoing_reflected_rays;
	
	// Intersection point
	double t;
	bool hit;				// ray miss or ray hit
	
	// normal of the intersection point in the geometry
	PhongMaterial *material;
	glm::dvec4	   normal;
	SceneNode	  *node;
	
	PhongMaterial *fromMaterial;
	
	// Construct an intersection point.
	Intersection(const Ray & ray,
				 double t,
				 bool hit);
	
	Intersection(const Ray & ray, double t);
	
	Intersection();
    
    // Stores the intersection point in the primitive.
    glm::dvec4 primitive_intersection_point;
	
};

#endif /* Intersection_hpp */

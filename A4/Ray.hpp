//
//  Ray.hpp
//  CS488-Projects
//
//  Created by Chen Huang on 2/27/16.
//  Copyright © 2016 none. All rights reserved.
//

#ifndef Ray_hpp
#define Ray_hpp

#include <stdio.h>
#include <glm/ext.hpp>

class Ray {
public:
	glm::dvec4 origin;
	glm::dvec4 direction;
	
	Ray(glm::dvec4 o, glm::dvec4 d);
	Ray(glm::dvec3 o, glm::dvec3 d);
};

#endif /* Ray_hpp */

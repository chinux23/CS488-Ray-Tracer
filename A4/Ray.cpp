//
//  Ray.cpp
//  CS488-Projects
//
//  Created by Chen Huang on 2/27/16.
//  Copyright © 2016 none. All rights reserved.
//

#include "Ray.hpp"

// Unless intersection function is modified, do not normalize the direction.

Ray::Ray(glm::dvec4 o, glm::dvec4 d) : origin(o), direction(d), material(PhongMaterial::Air)
{
    // do not normalize direction. it will affect the intersection function.
}

Ray::Ray(glm::dvec3 o, glm::dvec3 d) : origin({o.x, o.y, o.z, 1}), direction(d.x, d.y, d.z, 0), material(PhongMaterial::Air)
{
    // do not normalize direction. it will affect the intersection function.
}

Ray::Ray(const Ray & ray) : origin(ray.origin), direction(ray.direction), material(PhongMaterial::Air)
{
    // do not normalize direction. it will affect the intersection function.
}
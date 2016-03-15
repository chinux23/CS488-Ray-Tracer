//
//  Ray.cpp
//  CS488-Projects
//
//  Created by Chen Huang on 2/27/16.
//  Copyright © 2016 none. All rights reserved.
//

#include "Ray.hpp"

Ray::Ray(glm::dvec4 o, glm::dvec4 d) : origin(o), direction(d), material(PhongMaterial::AirMaterial)
{
//	direction = glm::normalize(direction);
}

Ray::Ray(glm::dvec3 o, glm::dvec3 d) : origin({o.x, o.y, o.z, 1}), direction(d.x, d.y, d.z, 0), material(PhongMaterial::AirMaterial)
{
//	direction = glm::normalize(direction);
}

Ray::Ray(const Ray & ray) : origin(ray.origin), direction(ray.direction), material(PhongMaterial::AirMaterial)
{
//	direction = glm::normalize(direction);
}
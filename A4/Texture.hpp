//
//  Texture.hpp
//  CS488-Projects
//
//  Created by Chen Huang on 3/25/16.
//  Copyright © 2016 none. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include <glm/glm.hpp>

enum TEXTUREIDS
{
    TEXTUREIDS_MARBLE = 0
};


class Texture
{
public:
    int textureID;
    Texture(int id);
};


class Marble : public Texture {
public:
    Marble();
    
    glm::dvec3 color(double u, double v);
};

#endif /* Texture_hpp */

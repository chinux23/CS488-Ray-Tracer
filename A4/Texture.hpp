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
#include <string>
#include <vector>

enum TEXTUREIDS
{
    TEXTUREIDS_MARBLE = 0,
    TEXTUREIDS_IMAGE = 1
};


class Texture
{
public:
    int textureID;
    Texture(int id);
    virtual glm::dvec3 color(double u, double v);
};


class Marble : public Texture {
public:
    Marble();
    
    virtual glm::dvec3 color(double u, double v);
};

class Picture : public Texture {
public:
    Picture(std::string filename);
    virtual glm::dvec3 color(double u, double v);
    
    std::vector<unsigned char> image;
    unsigned width;
    unsigned height;
};

#endif /* Texture_hpp */

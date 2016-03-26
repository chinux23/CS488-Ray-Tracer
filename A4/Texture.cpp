//
//  Texture.cpp
//  CS488-Projects
//
//  Created by Chen Huang on 3/25/16.
//  Copyright © 2016 none. All rights reserved.
//

#include "Texture.hpp"
#include <glm/gtc/noise.hpp>
#include <lodepng.h>
#include <iostream>


Texture::Texture(int id) : textureID(id) { };

glm::dvec3 Texture::color(double u, double v)
{
    return glm::dvec3{0, 0, 0};
}


Marble::Marble() : Texture(TEXTUREIDS_MARBLE)
{
    
}

glm::dvec3 Marble::color(double u, double v)
{

    assert(std::abs(u) <= 1);
    assert(std::abs(v) <= 1);
    
    glm::dvec3 result;
    
    double freq = 1.0;
    
    double val = 0.0;
    
    glm::dvec2 p2 {u * freq, v * freq};
    val = glm::sin(std::abs(glm::perlin(p2)) +
             1 / 2.0 * std::abs(glm::perlin(p2 * 2.0)) +
             1 / 4.0 * std::abs(glm::perlin(p2 * 4.0)) +
             1 / 8.0 * std::abs(glm::perlin(p2 * 8.0)));

//    val = (val + 1.0f) / 2.0;                // val is between 0 and 2 at this point, divided by 2 to get into [0, 1]
    
    return glm::dvec3{val, val, val};
}

Picture::Picture(std::string filename) : Texture(TEXTUREIDS_IMAGE)
{
    unsigned error = lodepng::decode(image, width, height, filename);
    if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

glm::dvec3 Picture::color(double u, double v)
{
    assert(std::abs(u) <= 1);
    assert(std::abs(v) <= 1);
    
    int pixel_x = (u + 1) / 2 * (width - 1);
    int pixel_y = (v + 1) / 2 * (height - 1);
    
    unsigned long position = pixel_y * width + pixel_x;
    position = position * 4;
    
    return glm::dvec3{image[position] / 255.0, image[position + 1] / 255.0, image[position + 2] / 255.0};

}
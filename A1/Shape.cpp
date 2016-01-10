//
//  Shape.cpp
//  CS488-Projects
//
//  Created by Chen Huang on 1/10/16.
//  Copyright © 2016 none. All rights reserved.
//

#include "Shape.hpp"

Shape::Shape(glm::vec3 position) : world_position(position)
{
	glGenBuffers(1, &ebo);
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	
}

Shape::~Shape()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Shape::draw()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glDrawElements(GL_TRIANGLES, (GLsizei)layout.size() * 3, GL_UNSIGNED_INT, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Shape::translate(glm::vec3 movement)
{
	
}


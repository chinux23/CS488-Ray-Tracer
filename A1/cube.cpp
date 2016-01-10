//
//  cube.cpp
//  CS488-Projects
//
//  Created by Chen Huang on 1/10/16.
//  Copyright © 2016 none. All rights reserved.
//

#include "cube.hpp"
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <iomanip>


// Constructor
Cube::Cube(glm::vec3 position, GLfloat size) : Shape(position), side_size(size)
{
	glm::vec3 centroid = position;
	GLfloat half_side = side_size / 2;
	
	// Model Vertices
	vertices = {
		centroid + glm::vec3( -half_side, -half_side,  half_side ),			// vertex # 0
		centroid + glm::vec3(  half_side, -half_side,  half_side ),			// vertex # 1
		centroid + glm::vec3(  half_side,  half_side,  half_side ),			// vertex # 2
		centroid + glm::vec3( -half_side,  half_side,  half_side ),			// vertex # 3
		centroid + glm::vec3( -half_side, -half_side, -half_side ),			// vertex # 4
		centroid + glm::vec3(  half_side, -half_side, -half_side ),			// vertex # 5
		centroid + glm::vec3(  half_side,  half_side, -half_side ),			// vertex # 6
		centroid + glm::vec3( -half_side,  half_side, -half_side ),			// vertex # 7
	};
	debugPrint();
	
	layout = {
		0, 1, 2,	// front face (bottom right)
		0, 3, 2,	// front face (top left)
		
		4, 5, 6,	// back face (bottom right)
		4, 7, 6,	// back face (top left)
		
		0, 1, 4,	// bottom face (left)
		1, 4, 5,	// bottom face (right)
		
		2, 3, 6,	// top face (right)
		3, 6, 7,	// top face (left)
		
		0, 3, 4,	// left face (bottom)
		3, 4, 7,	// left face (top)
		
		1, 2, 5,	// right face (bottom)
		2, 5, 6,	// right face (top)
	};
}

Cube::~Cube()
{
	
}

void Cube::debugPrint()
{
	std::cout << "Cube vertices: " << std::endl;
	for (auto vertex : vertices) {
		std::cout << std::showpos << " " << vertex.x << " " << vertex.y << " " << vertex.z << " " << std::endl;
	}
}

void Cube::uploadVboData()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size() , vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECK_GL_ERRORS;
}

void Cube::uploadEboData()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * layout.size(), layout.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	CHECK_GL_ERRORS;
}

void Cube::uploadVaoData(const ShaderProgram &shader)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	GLint positionAttribLocation = shader.getAttribLocation( "position" );
	glEnableVertexAttribArray(positionAttribLocation);
	glVertexAttribPointer(positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	
	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	CHECK_GL_ERRORS;
}

void Cube::uploadData(const ShaderProgram &shader)
{
	uploadVboData();
	uploadEboData();
	uploadVaoData(shader);
}

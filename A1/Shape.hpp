//
//  Shape.hpp
//  CS488-Projects
//
//  Created by Chen Huang on 1/10/16.
//  Copyright © 2016 none. All rights reserved.
//

#ifndef Shape_hpp
#define Shape_hpp

#include <glm/glm.hpp>

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <vector>


// Abstract class to define all interfaces related to Shapes in 3D world.
class Shape {
public:
	/*****************************
	 * Constructor & Destructor
	 *****************************/
	Shape(glm::vec3 world_position);
	virtual ~Shape();
	
	
	/*****************************
	 * Properties
	 *****************************/
public:
	glm::vec3	world_position;			// Define the position of the shape in the world. It's the centroid of the 3D shape.

	// Vertices
	std::vector<glm::vec3>	vertices;
	std::vector<GLuint>		layout;
	
protected:
	// Vertex Array Object
	GLuint vao;
	
	// Vertex Buffer Object
	GLuint vbo;
	
	// Element Buffer Object
	GLuint ebo;

	
	/*****************************
	 * Interface functions
	 *****************************/
public:
	// This function will be called each frame to draw the shape in the world.
	virtual void draw();
	
	// Move the shape in the world.
	virtual void translate(glm::vec3 movement);
	
};

#endif /* Shape_hpp */

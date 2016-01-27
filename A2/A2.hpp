#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include <glm/glm.hpp>

#include <vector>

using namespace std;
using namespace glm;

// Set a global maximum number of vertices in order to pre-allocate VBO data
// in one shot, rather than reallocating each frame.
const GLsizei kMaxVertices = 1000;


// Convenience class for storing vertex data in CPU memory.
// Data should be copied over to GPU memory via VBO storage before rendering.
class VertexData {
public:
	VertexData();

	std::vector<glm::vec2> positions;
	std::vector<glm::vec3> colours;
	GLuint index;
	GLsizei numVertices;
};


class A2 : public CS488Window {
public:
	A2();
	virtual ~A2();

protected:
	virtual void init() override;
	virtual void appLogic() override;
	virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;

	void createShaderProgram();
	void enableVertexAttribIndices();
	void generateVertexBuffers();
	void mapVboDataToVertexAttributeLocation();
	void uploadVertexDataToVbos();

	void initLineData();

	void setLineColour(const glm::vec3 & colour);

	void drawLine (
			const glm::vec2 & v0,
			const glm::vec2 & v1
	);

	ShaderProgram m_shader;

	GLuint m_vao;            // Vertex Array Object
	GLuint m_vbo_positions;  // Vertex Buffer Object
	GLuint m_vbo_colours;    // Vertex Buffer Object

	VertexData m_vertexData;

	glm::vec3 m_currentLineColour;

private:
	// Stores lines for the cube in model coordinates.
	std::vector<pair<glm::vec4, glm::vec4>> cube_vertices;

	// Cube location in the world coordinates
	glm::vec4 cube_location;

	// View Reference Point
	glm::vec4 vrp;

	// View Plane Normal
	glm::vec4 vpn;

	// View Up Vector
	glm::vec4 vup;
	
	// View transformations.
	glm::mat4 V;
	
	// Model transformations.
	glm::mat4 M;
	
	// Projection transformations
	glm::mat4 P;

	// Field of View in degree
	float fov;

public:
	// Initialize cube vertices
	void initCube();

	// Initialize view
	void initView();

private:
	// Calculate a new translated matrix for m.
	glm::mat4 translate(glm::mat4 const & m, const glm::vec3 & v);

	// Calculate a new roated matrix for m.
	glm::mat4 rotate(glm::mat4 const & m, const glm::vec3 & v);

	// Calculate a new scaled matrix for m.
	glm::mat4 scale(glm::mat4 const & m, const glm::vec3 & v);

	// test & debug
	void test_debug();
	
	// As user interact with the environment, we need to update our transformation matrix V and M.
	void update_model_transformation(const glm::mat4 & T);
	void update_view_transoformation(const glm::mat4 & T);


public:
	// Translate cube in model coordinates
	void translate_cube(const glm::vec3 &movement);

	// Rotate cube in model coordinates.
	void rotate_cube(glm::vec3 rotation_radian);

	// Scale cube in model coordinates.
	void scale_cube();

public:
	// The following functions are for unit tests.
	void test_translate_model();
	void test_rotate_model();
	void test_scale_model();

	void print_line_vertices();

};

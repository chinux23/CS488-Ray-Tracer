#include "A2.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>
using namespace std;

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

// For Debugging
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

//----------------------------------------------------------------------------------------
// Constructor
VertexData::VertexData()
	: numVertices(0),
	  index(0)
{
	positions.resize(kMaxVertices);
	colours.resize(kMaxVertices);
}


//----------------------------------------------------------------------------------------
// Constructor
A2::A2()
	: m_currentLineColour(vec3(0.0f))
{

}

//----------------------------------------------------------------------------------------
// Destructor
A2::~A2()
{

}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A2::init()
{
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);


	// Set the background colour.
	glClearColor(0.3, 0.5, 0.7, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao);

	enableVertexAttribIndices();

	generateVertexBuffers();

	mapVboDataToVertexAttributeLocation();

	// Modification
	M = glm::mat4();
	cout << "Initial Model Transformation: " << M << endl;
	
	// V is the inverse of View coordinate frame, assuming standard world frame (identity matrix).
	V = inverse(glm::mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, -1, 0), vec4(0, 0, 8, 1)));
	cout << "initial view frame:" << V << endl;

	// Init the cube in the standard world frame.
	initCube();
	
	// Test & debug

}

void A2::initCube()
{
	cube_vertices.push_back({glm::vec4(-1.0f, -1.0f,  1.0f, 1), glm::vec4( 1.0f, -1.0f,  1.0f, 1)});
	cube_vertices.push_back({glm::vec4( 1.0f, -1.0f,  1.0f, 1), glm::vec4( 1.0f, -1.0f, -1.0f, 1)});
	cube_vertices.push_back({glm::vec4(-1.0f, -1.0f, -1.0f, 1), glm::vec4( 1.0f, -1.0f, -1.0f, 1)});
	cube_vertices.push_back({glm::vec4(-1.0f, -1.0f, -1.0f, 1), glm::vec4(-1.0f, -1.0f,  1.0f, 1)});

	cube_vertices.push_back({glm::vec4(-1.0f,  1.0f,  1.0f, 1), glm::vec4( 1.0f,  1.0f,  1.0f, 1)});
	cube_vertices.push_back({glm::vec4( 1.0f,  1.0f,  1.0f, 1), glm::vec4( 1.0f,  1.0f, -1.0f, 1)});
	cube_vertices.push_back({glm::vec4( 1.0f,  1.0f, -1.0f, 1), glm::vec4(-1.0f,  1.0f, -1.0f, 1)});
	cube_vertices.push_back({glm::vec4(-1.0f,  1.0f,  1.0f, 1), glm::vec4(-1.0f,  1.0f, -1.0f, 1)});

	cube_vertices.push_back({glm::vec4(-1.0f, -1.0f,  1.0f, 1), glm::vec4(-1.0f,  1.0f,  1.0f, 1)});
	cube_vertices.push_back({glm::vec4(-1.0f, -1.0f, -1.0f, 1), glm::vec4(-1.0f,  1.0f, -1.0f, 1)});
	cube_vertices.push_back({glm::vec4( 1.0f, -1.0f,  1.0f, 1), glm::vec4( 1.0f,  1.0f,  1.0f, 1)});
	cube_vertices.push_back({glm::vec4( 1.0f, -1.0f, -1.0f, 1), glm::vec4( 1.0f,  1.0f, -1.0f, 1)});

	cube_location = glm::vec4(0.0f, 0.0f, 0.0f, 1);

	std::cout << "Cube location:\n" << cube_location << std::endl;
}

//----------------------------------------------------------------------------------------
void A2::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();
}

//----------------------------------------------------------------------------------------
void A2::enableVertexAttribIndices()
{
	glBindVertexArray(m_vao);

	// Enable the attribute index location for "position" when rendering.
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray(positionAttribLocation);

	// Enable the attribute index location for "colour" when rendering.
	GLint colourAttribLocation = m_shader.getAttribLocation( "colour" );
	glEnableVertexAttribArray(colourAttribLocation);

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A2::generateVertexBuffers()
{
	// Generate a vertex buffer to store line vertex positions
	{
		glGenBuffers(1, &m_vbo_positions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	// Generate a vertex buffer to store line colors
	{
		glGenBuffers(1, &m_vbo_colours);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A2::mapVboDataToVertexAttributeLocation()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao);

	// Tell GL how to map data from the vertex buffer "m_vbo_positions" into the
	// "position" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_colours" into the
	// "colour" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
	GLint colorAttribLocation = m_shader.getAttribLocation( "colour" );
	glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void A2::initLineData()
{
	m_vertexData.numVertices = 0;
	m_vertexData.index = 0;
}

//---------------------------------------------------------------------------------------
void A2::setLineColour (
		const glm::vec3 & colour
) {
	m_currentLineColour = colour;
}

//---------------------------------------------------------------------------------------
void A2::drawLine(
		const glm::vec2 & v0,   // Line Start (NDC coordinate)
		const glm::vec2 & v1    // Line End (NDC coordinate)
) {

	m_vertexData.positions[m_vertexData.index] = v0;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;
	m_vertexData.positions[m_vertexData.index] = v1;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;

	m_vertexData.numVertices += 2;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A2::appLogic()
{
	// Place per frame, application logic here ...

	// Call at the beginning of frame, before drawing lines:
	initLineData();

	// Draw outer square:
	setLineColour(vec3(1.0f, 0.7f, 0.8f));
	drawLine(vec2(-0.5f, -0.5f), vec2(0.5f, -0.5f));
	drawLine(vec2(0.5f, -0.5f), vec2(0.5f, 0.5f));
	drawLine(vec2(0.5f, 0.5f), vec2(-0.5f, 0.5f));
	drawLine(vec2(-0.5f, 0.5f), vec2(-0.5f, -0.5f));


	// Draw inner square:
	setLineColour(vec3(0.2f, 1.0f, 1.0f));
	drawLine(vec2(-0.25f, -0.25f), vec2(0.25f, -0.25f));
	drawLine(vec2(0.25f, -0.25f), vec2(0.25f, 0.25f));
	drawLine(vec2(0.25f, 0.25f), vec2(-0.25f, 0.25f));
	drawLine(vec2(-0.25f, 0.25f), vec2(-0.25f, -0.25f));
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A2::guiLogic()
{
	static bool firstRun(true);
	if (firstRun) {
		ImGui::SetNextWindowPos(ImVec2(50, 50));
		firstRun = false;
	}

	static bool showDebugWindow(true);
	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Properties", &showDebugWindow, ImVec2(100,100), opacity,
			windowFlags);


		// Add more gui elements here here ...


		// Create Button, and check if it was clicked:
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();
}

//----------------------------------------------------------------------------------------
void A2::uploadVertexDataToVbos() {

	//-- Copy vertex position data into VBO, m_vbo_positions:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec2) * m_vertexData.numVertices,
				m_vertexData.positions.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	//-- Copy vertex colour data into VBO, m_vbo_colours:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * m_vertexData.numVertices,
				m_vertexData.colours.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}


void A2::update_model_transformation(const glm::mat4 & T)
{
	M = M * T;
}

void A2::update_view_transoformation(const glm::mat4 & T)
{
	V = inverse(T) * V;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A2::draw()
{
	uploadVertexDataToVbos();

	glBindVertexArray(m_vao);

	m_shader.enable();
		glDrawArrays(GL_LINES, 0, m_vertexData.numVertices);
	m_shader.disable();

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A2::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A2::cursorEnterWindowEvent (
		int entered
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A2::mouseMoveEvent (
		double xPos,
		double yPos
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A2::mouseButtonInputEvent (
		int button,
		int actions,
		int mods
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A2::mouseScrollEvent (
		double xOffSet,
		double yOffSet
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A2::windowResizeEvent (
		int width,
		int height
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A2::keyInputEvent (
		int key,
		int action,
		int mods
) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if (action == GLFW_PRESS) {
		switch(key){
			case GLFW_KEY_T:
				cout << "Model Translation Mode." << endl;
				break;
			case GLFW_KEY_R:
				cout << "Model Rotation Mode" << endl;
				break;
			case GLFW_KEY_S:
				cout << "Model Scale Mode" << endl;
				break;
			case GLFW_KEY_U:
				cout << "Enter Unit Test Mode" << endl;
				test_debug();
				break;
			case GLFW_KEY_Q:
				cout << "Quit" << endl;
				glfwSetWindowShouldClose(m_window, GL_TRUE);
				break;
			default:
				break;
		}
	}

	return eventHandled;
}


void A2::translate_cube(const glm::vec3& movement)
{
	glm::vec4 movement_vector(movement.x, movement.y, movement.z, 0);

	for (auto & line : cube_vertices) {
		line.first  = translate(mat4(), movement) * line.first;
		line.second = translate(mat4(), movement) * line.second;
	}
}

void A2::rotate_cube(glm::vec3 rotation_radian)
{
	
}

glm::mat4 A2::scale(glm::mat4 const & m, const glm::vec3 & v)
{
	float scale_variables[] = {
		v.x,   0,   0,  0,
		  0, v.y,   0,  0,
		  0,   0, v.z,  0,
		  0,   0,   0,  1};
	glm::mat4 scale_matrix = glm::make_mat4(scale_variables);
	return scale_matrix * m;
}

glm::mat4 A2::translate(const glm::mat4 &m, const glm::vec3 &v)
{
	float translation_variables[] = {
		  1,   0,   0,  0,
		  0,   1,   0,  0,
		  0,   0,   1,  0,
		v.x, v.y, v.z,  1};
	glm::mat4 translation = glm::make_mat4(translation_variables);
	return translation * m;
}

glm::mat4 A2::rotate(glm::mat4 const &m, const glm::vec3 & v)
{
	float rot_z[] = {
		 cos(v.z),  sin(v.z), 0, 0,
		-sin(v.z),  cos(v.z), 0, 0,
		 0,          0,       1, 0,
		 0,          0,       0, 1};
	glm::mat4 rotate_z = glm::make_mat4(rot_z);
	
	float rot_x[] = {
		1,  0,        0,        0,
		0,  cos(v.x), sin(v.x), 0,
		0, -sin(v.x), cos(v.x), 0,
		0,  0,        0,        1};
	glm::mat4 rotate_x = glm::make_mat4(rot_x);
	
	float rot_y[] = {
		cos(v.y),  0,  -sin(v.y), 0,
		0,		   1,   0,	      0,
		sin(v.y),  0,   cos(v.y), 0,
		0,         0,   0,        1};
	glm::mat4 rotate_y = glm::make_mat4(rot_y);
	
	return rotate_x * rotate_y * rotate_z * m;
}


#pragma mark - Unit Test

void A2::test_translate_model()
{
	// Test translate model about X


}

void A2::print_line_vertices()
{
	cout << "----------------------" << endl;
	for (const auto &line : cube_vertices) {
		cout << glm::to_string(line.first) << " " << glm::to_string(line.second) << endl;
	}
	cout << "----------------------" << endl;
}

void A2::test_debug()
{
	cout << endl;
	cout << "-- Testing rotating a point at [1,0,0] 90 degrees along Z -> X -> Y --" << endl;
	float theta = glm::radians(90.0f);
	float rot_z[] = {
		 cos(theta), sin(theta), 0, 0,
	    -sin(theta), cos(theta), 0, 0,
		 0,          0,          1, 0,
		 0,          0,          0, 1};
	glm::mat4 rotate_z = glm::make_mat4(rot_z);
	
	float rot_x[] = {
	 	 1,  0,          0,          0,
	 	 0,  cos(theta), sin(theta), 0,
	 	 0, -sin(theta), cos(theta), 0,
	 	 0,  0,          0,          1};
	glm::mat4 rotate_x = glm::make_mat4(rot_x);
	
	float rot_y[] = {
		 cos(theta),  0,  -sin(theta), 0,
		 0,			  1,   0,	       0,
	     sin(theta),  0,   cos(theta), 0,
		 0,           0,   0,          1};
	glm::mat4 rotate_y = glm::make_mat4(rot_y);
	
	glm::vec4 point = glm::vec4({1.0f, 0.0f, 0.0f, 1});		// A point on X
	
	glm::vec4 rotated_p_z = rotate_z * point;
	cout << "Rotating\n\t" << glm::to_string(point) << "\n\tagainst Z for 90 degree:\n\t" << glm::to_string(rotated_p_z) << endl;
	
	glm::vec4 rotated_p_x = rotate_x * rotated_p_z;
	cout << "Rotating\n\t" << glm::to_string(rotated_p_z) << "\n\tagainst X for 90 degree:\n\t" << glm::to_string(rotated_p_x) << endl;
	
	glm::vec4 rotated_p_y = rotate_y * rotated_p_x;
	cout << "Rotating\n\t" << glm::to_string(rotated_p_x) << "\n\tagainst Y for 90 degree:\n\t" << glm::to_string(rotated_p_y) << endl;
	
	cout << endl << endl;
	cout << "-- Testing rotating matrix --" << endl;
	glm::mat4 W;
	W = rotate(W, vec3(            0,              0,  radians(90.0f)));
	W = rotate(W, vec3(radians(90.0f),             0,              0));
	W = rotate(W, vec3(            0,  radians(90.0f),             0));
	
	glm::vec4 rotated_point = W * point;
	cout << "Rotating\n\t" << glm::to_string(point) << "\n\tagainst ZXY for 90 degree each:\n\t" << glm::to_string(rotated_point) << endl;
	cout << "\tCorrect Result:\n\t" << "[1, 0, 0]" << endl;
	
	cout << endl << endl;
	cout << "-- Testing translating matrix --" << endl;
	W = glm::mat4();	// Reset W to identify matrix.
	W = translate(W, vec3(0, 0, 2));
	W = translate(W, vec3(0, 1, 0));
	W = translate(W, vec3(-4, 0, 0));
	glm::vec4 translated_point = W * rotated_point;
	cout << "Translate " << glm::to_string(rotated_point) << "\n\twith vector[-4, 1, 2]\n\tCalculated Results: " << glm::to_string(translated_point) << endl;
	cout << "\tCorrect Result: \t" << "[-3, 1, 2]" << endl;
	
	cout << "-- Testing Scale matrix --" << endl;
	W = glm::mat4();
	W = scale(W, vec3(2, -9, 4));
	point = vec4(2, 1.0/3, 1, 1);
	glm::vec4 scaled_point = W * point;
	cout << "Scale " << glm::to_string(point) << "\n\twith vector [2, 3, 4]\n\tCalculated Results: " << glm::to_string(scaled_point) << endl;
	cout << "\tCorrect Results: \t" << "[4, -3, 4]" << endl;
	
	cout << endl;
	cout << "-- Testing Model transformation --" << endl;
	glm::vec4 p = glm::vec4(0, 2, 0, 1);						// assume a point in Y axis with 2 unit length
	W = glm::mat4();
	W = translate(W, vec3(1, 0, 1));				// translate model with 1 unit in X and 1 unit in Z
	W = rotate(W, vec3(0, radians(-90.0), 0));		// rotate -90 degree against Y.
	p = W * p;										// That's my new P.
	
	cout << "\tNew P         : " << p << endl;
	cout << "\tCorrect Result: " << vec4(-1, 2, 1, 1) << endl;
	
	cout << endl;
	cout << "-- Testing Model-View tranformation --" << endl;
	// Assuming the view coordinate frame is as following
	// Assuming standard world frame is [1, 1, 1] and origin is at [0, 0, 0]
	// [1, 1, -1] and origin is at [0, 0, 8]
	// Calculate the point coordinate in View frame. (change of basis).
	glm::mat4 V_prime = glm::mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, -1, 0), vec4(0, 0, 8, 1));
	glm::mat4 W_frame = glm::mat4();		// This is an identity matrix.
	// P in new frame should be: [-1, 2, 7]
	p = inverse(V_prime) * W_frame * p;
	cout << "\tCompulated p   : " << p << endl;
	cout << "\tCorrect Results: " << vec4(-1, 2, 7, 1) << endl;
	
	cout << endl;
	cout << "-- Testing View transformation --" << endl;
	p = vec4(-1, 2, 1, 1);
	// Move view frame along x by 1
	mat4 T = translate(mat4(), vec3(1, 0, 0));
	V_prime = glm::mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, -1, 0), vec4(0, 0, 8, 1));
	V_prime = inverse(V_prime);
	V_prime = inverse(T) * V_prime;
	p = V_prime * W_frame * p;
	cout << "\tComputed p	  : " << p << endl;
	cout << "\tCorrect Results: " << vec4(-2, 2, 7, 1) << endl;
	
}

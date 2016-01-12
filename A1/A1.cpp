#include "A1.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>
#include <sstream>

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace glm;
using namespace std;

static const size_t DIM = 16;

// Default colors
static float default_colors[8][3] = {
	1.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f
};

//----------------------------------------------------------------------------------------
// Constructor
A1::A1()
	: current_col( 0 )
{
	colour[0] = 0.0f;
	colour[1] = 0.0f;
	colour[2] = 0.0f;
}

//----------------------------------------------------------------------------------------
// Destructor
A1::~A1()
{}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A1::init()
{
	// Set the background colour.
	glClearColor( 0.3, 0.5, 0.7, 1.0 );

	// Build the shader
	m_shader.generateProgramObject();
	m_shader.attachVertexShader(
		getAssetFilePath( "VertexShader.vs" ).c_str() );
	m_shader.attachFragmentShader(
		getAssetFilePath( "FragmentShader.fs" ).c_str() );
	m_shader.link();

	// Set up the uniforms
	P_uni = m_shader.getUniformLocation( "P" );
	V_uni = m_shader.getUniformLocation( "V" );
	M_uni = m_shader.getUniformLocation( "M" );
	col_uni = m_shader.getUniformLocation( "colour" );

	initGrid();

	// Set up initial view and projection matrices (need to do this here,
	// since it depends on the GLFW window being set up correctly).
	view = glm::lookAt( 
		glm::vec3( 0.0f, float(DIM)*2.0*M_SQRT1_2, float(DIM)*2.0*M_SQRT1_2 ),
		glm::vec3( 0.0f, 0.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ) );
	proj = glm::perspective( 
		glm::radians( 45.0f ),
		float( m_framebufferWidth ) / float( m_framebufferHeight ),
		1.0f, 1000.0f );
	
	// Initialize each grid with an empty Cube stack.
	for (int i = 0; i < DIM; i++) {
		vector<CubeStack> col_of_cubes;
		for (int j = 0; j < DIM; j++) {
			col_of_cubes.push_back(CubeStack());
		}
		grid_of_cubes.push_back(col_of_cubes);
	}
	
	t_start = std::chrono::high_resolution_clock::now();
	isCopyEnabled = 0;
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			colors[i][j] = default_colors[i][j];
		}
	}
	
	scale_factor = 1.0f;
}

void A1::reset()
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			colors[i][j] = default_colors[i][j];
		}
	}
	
	rotation_degree = 0.0f;
	scale_factor = 1.0f;
	
	for (auto & col_of_stack : grid_of_cubes) {
		for (auto & stack : col_of_stack) {
			stack.clear();
		}
	}
	
	active_cell_position.first = 0;
	active_cell_position.second = 0;
}

void A1::initGrid()
{
	size_t sz = 3 * 2 * 2 * (DIM+3);

	float *verts = new float[ sz ];
	size_t ct = 0;
	for( int idx = 0; idx < DIM+3; ++idx ) {
		verts[ ct ] = -1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = idx-1;
		verts[ ct+3 ] = DIM+1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = idx-1;
		ct += 6;

		verts[ ct ] = idx-1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = -1;
		verts[ ct+3 ] = idx-1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = DIM+1;
		ct += 6;
	}

	// Create the vertex array to record buffer assignments.
	glGenVertexArrays( 1, &m_grid_vao );
	glBindVertexArray( m_grid_vao );

	// Create the cube vertex buffer
	glGenBuffers( 1, &m_grid_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_grid_vbo );
	glBufferData( GL_ARRAY_BUFFER, sz*sizeof(float),
		verts, GL_STATIC_DRAW );

	// Specify the means of extracting the position values properly.
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Reset state to prevent rogue code from messing with *my* 
	// stuff!
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	// OpenGL has the buffer now, there's no need for us to keep a copy.
	delete [] verts;

	CHECK_GL_ERRORS;
}

void A1::extendStack(){
	// Get current active cell
	GLint x = active_cell_position.first;
	GLint z = active_cell_position.second;
	CubeStack&  active_stack = activeStack();
	
	size_t num_of_cubes = active_stack.size();
	glm::vec3 new_position(0.5f + x, 0.5f + num_of_cubes, 0.5f + z);
	
	std::shared_ptr<Cube> new_cube = make_shared<Cube>(new_position, 1.0f);
	new_cube->colorIndex = current_col;
	new_cube->uploadData(m_shader);
	
	// Add new cube to current stack of cubes
	active_stack.push_back(new_cube);
}

void A1::shrinkStack(){
	if (activeStack().size() > 0) {
		activeStack().pop_back();
	}
}

CubeStack& A1::activeStack()
{
	GLint x = active_cell_position.first;
	GLint z = active_cell_position.second;
	return grid_of_cubes[x][z];
}

void A1::adjustCurrentStackSize(long size)
{
	if (size == 0) {
		return;
	}
	
	if (size > 0) {
		for (long i = 0; i < size; i++) {
			extendStack();
		}
	}
	
	if (size < 0) {
		for (long i = 0; i > size; i--) {
			shrinkStack();
		}
	}
}

void A1::moveActiveCellUp()
{
	long last_active_stack_size = activeStack().size();
	if (active_cell_position.second > 0) {
		active_cell_position.second--;
		updateActiveCellColor();
	}
	long current_stack_size = activeStack().size();
	long difference = last_active_stack_size - current_stack_size;
	if (isCopyEnabled) {
		adjustCurrentStackSize(difference);
	}
//	debugPrintActiveCell();
}

void A1::moveActiveCellDown()
{
	long last_active_stack_size = activeStack().size();
	if (active_cell_position.second < DIM-1) {
		active_cell_position.second++;
		updateActiveCellColor();
	}
//	debugPrintActiveCell();
	long current_stack_size = activeStack().size();
	long difference = last_active_stack_size - current_stack_size;
	if (isCopyEnabled) {
		adjustCurrentStackSize(difference);
	}
}

void A1::moveActiveCellLeft()
{
	long last_active_stack_size = activeStack().size();
	if (active_cell_position.first > 0) {
		active_cell_position.first--;
		updateActiveCellColor();
	}
//	debugPrintActiveCell();
	
	long current_stack_size = activeStack().size();
	long difference = last_active_stack_size - current_stack_size;
	if (isCopyEnabled) {
		adjustCurrentStackSize(difference);
	}
}

void A1::moveActiveCellRight()
{
	long last_active_stack_size = activeStack().size();
	if (active_cell_position.first < DIM-1) {
		active_cell_position.first++;
		updateActiveCellColor();
	}
//	debugPrintActiveCell();
	
	long current_stack_size = activeStack().size();
	long difference = last_active_stack_size - current_stack_size;
	if (isCopyEnabled) {
		adjustCurrentStackSize(difference);
	}
}

void A1::updateActiveCellColor()
{
	CubeStack &stack = activeStack();
	for (auto cube : stack) {
		cube->colorIndex = current_col;
	}
}

void A1::debugPrintActiveCell()
{
	std::cout << "ActiveCell: [" << active_cell_position.first << ", " << active_cell_position.second << "]" << std::endl;
}

void A1::enableShiftCopy()
{
	isCopyEnabled++;
}

void A1::disableShiftCopy()
{
	isCopyEnabled--;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A1::appLogic()
{
	// Place per frame, application logic here ...
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A1::guiLogic()
{
	// We already know there's only going to be one window, so for 
	// simplicity we'll store button states in static local variables.
	// If there was ever a possibility of having multiple instances of
	// A1 running simultaneously, this would break; you'd want to make
	// this into instance fields of A1.
	static bool showTestWindow(false);
	static bool showDebugWindow(true);

	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Debug Window", &showDebugWindow, ImVec2(100,100), opacity, windowFlags);
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}

		// Eventually you'll create multiple colour widgets with
		// radio buttons.  If you use PushID/PopID to give them all
		// unique IDs, then ImGui will be able to keep them separate.
		// This is unnecessary with a single colour selector and
		// radio button, but I'm leaving it in as an example.

		// Prefixing a widget name with "##" keeps it from being
		// displayed.
	
		for (int i = 0; i < 8; i++) {
			std::stringstream ss;
			ss << "##Colour" << i;
			ImGui::PushID( i );
			ImGui::ColorEdit3( ss.str().c_str() , colors[i] );
			ImGui::SameLine();
			if( ImGui::RadioButton( "##Col", &current_col, i ) ) {
				// Select this colour.
				std::cout << "RadioButton activate: " << i << std::endl;
				std::cout << "Current colour " << current_col << std::endl;
				
				CubeStack &stack = activeStack();
				for (auto cube : stack) {
					cube->colorIndex = current_col;
				}
			}
			ImGui::PopID();
		}

		// For convenience, you can uncomment this to show ImGui's massive
		// demonstration window right in your application.  Very handy for
		// browsing around to get the widget you want.  Then look in 
		// shared/imgui/imgui_demo.cpp to see how it's done.
//		if( ImGui::Button( "Test Window" ) ) {
//			showTestWindow = !showTestWindow;
//		}

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();

	if( showTestWindow ) {
		ImGui::ShowTestWindow( &showTestWindow );
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A1::draw()
{
	// Create a global transformation for the model (centre it).
	mat4 W;
	W = glm::rotate(W, rotation_degree, glm::vec3(0, 1, 0));
	W = glm::scale(W, glm::vec3(scale_factor, scale_factor, scale_factor));
	W = glm::translate( W, vec3( -float(DIM)/2.0f, 0, -float(DIM)/2.0f ) );

	m_shader.enable();
		glEnable( GL_DEPTH_TEST );

		glUniformMatrix4fv( P_uni, 1, GL_FALSE, value_ptr( proj ) );
		glUniformMatrix4fv( V_uni, 1, GL_FALSE, value_ptr( view ) );
		glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr( W ) );

		// Just draw the grid for now.
		glBindVertexArray( m_grid_vao );
		glUniform4f( col_uni, 1, 1, 1, 1);
		glDrawArrays( GL_LINES, 0, (3+DIM)*4 );
	
		// Draw the cubes
		for (auto col_of_stack : grid_of_cubes) {
			for (auto stack : col_of_stack) {
				for (auto cube : stack) {
					// Get cube color
					int color_index = cube->colorIndex;
					glUniform4f( col_uni, colors[color_index][0], colors[color_index][1], colors[color_index][2], 1);
					cube->draw();
					glUniform4f( col_uni, 1, 1, 1, 1);
				}
			}
		}
	
		// Highlight the active square.
		TimePoint t_now = std::chrono::high_resolution_clock::now();
		// The following line of code is referenced from: https://open.gl/drawing
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		glUniform4f(col_uni, 0.1f, 0.1f, 0.6f, (sin(time * 8.0f) + 1.0f) / 4.0f + 0.5f);
	
		CubeStack& active_stack = activeStack();
		GLint x = active_cell_position.first;
		GLint z = active_cell_position.second;
		size_t num_of_cubes = active_stack.size();
		glm::vec3 new_position(0.5f + x, 0.5f + num_of_cubes, 0.5f + z);
		std::shared_ptr<Cube> highlighting_cube = make_shared<Cube>(new_position, 1.0f);
		highlighting_cube->uploadData(m_shader);
		highlighting_cube->draw();

	
	m_shader.disable();

	// Restore defaults
	glBindVertexArray( 0 );

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A1::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A1::cursorEnterWindowEvent (
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
bool A1::mouseMoveEvent(double xPos, double yPos) 
{
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// Put some code here to handle rotations.  Probably need to
		// check whether we're *dragging*, not just moving the mouse.
		// Probably need some instance variables to track the current
		// rotation amount, and maybe the previous X position (so 
		// that you can rotate relative to the *change* in X.
		if (ImGui::IsMouseDown(0)) {
			double difference = xPos - mouse_x_pos;
//			std::cout << "Mouse move in x axis with: " << difference << std::endl;
			rotation_degree += difference * 0.01;
		}
		mouse_x_pos = xPos;
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A1::mouseButtonInputEvent(int button, int actions, int mods) {
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// The user clicked in the window.  If it's the left
		// mouse button, initiate a rotation.
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A1::mouseScrollEvent(double xOffSet, double yOffSet) {
	bool eventHandled(false);

	// Zoom in or out.
//	std::cout << "Scroll: " << xOffSet << " " << yOffSet << std::endl;
	if (yOffSet > 0) {
		scale_factor = scale_factor * (0.001 * yOffSet + 1);
	} else {
		scale_factor = scale_factor / (0.001 * (-yOffSet) + 1);
	}
	
	if (scale_factor < 0.01) {
		scale_factor = 0.01;
	}
	
	if (scale_factor > 10) {
		scale_factor = 10;
	}
	
	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A1::windowResizeEvent(int width, int height) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A1::keyInputEvent(int key, int action, int mods) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if( action == GLFW_PRESS ) {
		// Respond to some key events.
		switch (key) {
			case GLFW_KEY_SPACE:
				extendStack();
				break;
			case GLFW_KEY_BACKSPACE:
				shrinkStack();
				break;
			case GLFW_KEY_UP:
				moveActiveCellUp();
				break;
			case GLFW_KEY_DOWN:
				moveActiveCellDown();
				break;
			case GLFW_KEY_LEFT:
				moveActiveCellLeft();
				break;
			case GLFW_KEY_RIGHT:
				moveActiveCellRight();
				break;
			case GLFW_KEY_RIGHT_SHIFT:
			case GLFW_KEY_LEFT_SHIFT:
				enableShiftCopy();
				break;
			case GLFW_KEY_Q:
				glfwSetWindowShouldClose(m_window, GL_TRUE);
				break;
			case GLFW_KEY_R:
				reset();
				break;
			default:
				break;
		}
	} else if (action == GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_LEFT_SHIFT:
			case GLFW_KEY_RIGHT_SHIFT:
				disableShiftCopy();
				break;
				
			default:
    break;
		}
	}

	return eventHandled;
}

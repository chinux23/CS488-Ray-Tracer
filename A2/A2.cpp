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
#include <glm/gtc/reciprocal.hpp>

#include <sstream>

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

	reset();

}

void A2::reset()
{
	// Modification
	M = glm::mat4();
	//    M = M * rotate(mat4(), vec3(0, 1, 0));
	cout << "Initial Model Transformation: " << M << endl;
	
	// V is the inverse of View coordinate frame, assuming standard world frame (identity matrix).
	V = inverse(glm::mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, -1, 0), vec4(0, 0, 10, 1)));
	//    V = inverse(rotate(mat4(), vec3(0.2, 0, 0))) * V;
	cout << "initial view frame:" << V << endl;
	
	fov = 30.0;
	cout << "initial Fov is " << fov << endl;
	
	near = 5;
	far = 20;
	
	cout << "near plane is " << near << " distance away" << endl;
	cout << "far  plane is " << far  << " distance away" << endl;
	
	P = mat4(vec4(1/tan(glm::radians(fov/2)), 0, 0, 0),
			 vec4(0, 1/tan(radians(fov/2)), 0, 0),
			 vec4(0, 0, (far + near)/(far - near), 1),
			 vec4(0, 0, -2 * far * near / (far - near), 0));
	
	
	// Init the cube in the standard world frame.
	initCube();
	
	// Init world and model reference coordinate.
	initGnomon();
	
	// Test & debug
	curr_mode = 'R';    // default mode is rotate model.
	mouse_x_pos = 0;
	mouse_y_pos = 0;
	
	view_port_center = vec4({0, 0, 0, 1});
	view_port_side_x = 0.95;
	view_port_side_y = 0.95;
	
	construct_view_port();
	
	shouldViewportResize = false;
}

void A2::update_perspective_view()
{
	// Update persepective view.
	P = mat4(vec4(1/tan(glm::radians(fov/2)), 0, 0, 0),
			 vec4(0, 1/tan(radians(fov/2)), 0, 0),
			 vec4(0, 0, (far + near)/(far - near), 1),
			 vec4(0, 0, -2 * far * near / (far - near), 0));
}

void A2::initCube()
{
	cube_vertices.clear();
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
}

void A2::initGnomon()
{
	world_reference.clear();
    // world reference frame
    world_reference.push_back({vec4(0, 0, 0, 1), vec4(1.0f, 0.0f, 0.0f, 1)});
    world_reference.push_back({vec4(0, 0, 0, 1), vec4(0.0f, 1.0f, 0.0f, 1)});
    world_reference.push_back({vec4(0, 0, 0, 1), vec4(0.0f, 0.0f, 1.0f, 1)});
    
    // Model reference frame
	model_reference.clear();
    model_reference.push_back({vec4(0, 0, 0, 1), vec4(1.0f, 0.0f, 0.0f, 1)});
    model_reference.push_back({vec4(0, 0, 0, 1), vec4(0.0f, 1.0f, 0.0f, 1)});
    model_reference.push_back({vec4(0, 0, 0, 1), vec4(0.0f, 0.0f, 1.0f, 1)});
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

//	// Draw outer square:
//	setLineColour(vec3(1.0f, 0.7f, 0.8f));
//	drawLine(vec2(-0.5f, -0.5f), vec2(0.5f, -0.5f));
//	drawLine(vec2(0.5f, -0.5f), vec2(0.5f, 0.5f));
//	drawLine(vec2(0.5f, 0.5f), vec2(-0.5f, 0.5f));
//	drawLine(vec2(-0.5f, 0.5f), vec2(-0.5f, -0.5f));
//
//
//	// Draw inner square:
//	setLineColour(vec3(0.2f, 1.0f, 1.0f));
//	drawLine(vec2(-0.25f, -0.25f), vec2(0.25f, -0.25f));
//	drawLine(vec2(0.25f, -0.25f), vec2(0.25f, 0.25f));
//	drawLine(vec2(0.25f, 0.25f), vec2(-0.25f, 0.25f));
//	drawLine(vec2(-0.25f, 0.25f), vec2(-0.25f, -0.25f));
	
	// Update viewport
	if (shouldViewportResize) {
		int width;
		int height;
		vec2 v1, v2;
		
		glfwGetWindowSize(m_window, &width, &height);
		v1.x = viewport_v1.first < 0 ? 0 : viewport_v1.first;
		v1.x = v1.x > width ? width : v1.x;
		v1.x = (v1.x * 2 / width) - 1;	// normalize
		
		v1.y = viewport_v1.second < 0 ? 0 : viewport_v1.second;
		v1.y = v1.y > height ? height : v1.y;
		v1.y = 1 - (v1.y * 2 / height);	// normalize
		
		v2.x = viewport_v2.first < 0 ? 0 : viewport_v2.first;
		v2.x = v2.x > width ? width : v2.x;
		v2.x = (v2.x * 2 / width) - 1;
		
		v2.y = viewport_v2.second < 0 ? 0 : viewport_v2.second;
		v2.y = v2.y > height ? height : v2.y;
		v2.y = 1 - (v2.y * 2 / height);	// normalize
		
		update_view_port(v1, v2);
		construct_view_port();
	}
	
	// draw cube
	setLineColour(vec3(1.0f, 0.7f, 0.8f));
	for (const auto & line: cube_vertices) {
		vec4 v1 = P * V * M * line.first;
		vec4 v2 = P * V * M * line.second;
		// Do clipping.
		if (clip(v1, v2)){
			drawLineInViewPort({v1.x/v1.w, v1.y/v1.w}, {v2.x/v2.w, v2.y/v2.w});
		}
	}
    
    // draw world reference
    for (int i = 0; i < 3; i++) {
        vec4 v1 = world_reference[i].first;
        vec4 v2 = world_reference[i].second;
        
        if ( i == 0)
            setLineColour(vec3(1.0f, 0.0f, 0.0f));
        else if (i == 1)
            setLineColour(vec3(0.0f, 1.0f, 0.0f));
        else if (i == 2)
            setLineColour(vec3(0.0f, 0.0f, 1.0f));
        
        v1 = P * V * v1;
        v2 = P * V * v2;
		if (clip(v1, v2)){
			drawLineInViewPort({v1.x/v1.w, v1.y/v1.w}, {v2.x/v2.w, v2.y/v2.w});
		}
    }
    
    // draw model reference
    for (int i = 0; i < 3; i++) {
        vec4 v1 = model_reference[i].first;
        vec4 v2 = model_reference[i].second;
        
        if ( i == 0)
            setLineColour(vec3(1.0f, 0.0f, 0.0f));
        else if (i == 1)
            setLineColour(vec3(0.0f, 1.0f, 0.0f));
        else if (i == 2)
            setLineColour(vec3(0.0f, 0.0f, 1.0f));
        
        v1 = P * V * M * v1;
        v2 = P * V * M * v2;
		if (clip(v1, v2)){
			drawLineInViewPort({v1.x/v1.w, v1.y/v1.w}, {v2.x/v2.w, v2.y/v2.w});
		}
    }
	
	// draw view port
	setLineColour(vec3(0.7f, 0.7f, 0.1f));
	for (int i = 0; i < 4; i++) {
		vec2 v1 = view_port[i % 4];
		vec2 v2 = view_port[(i + 1) % 4];
		
		drawLine(v1, v2);
	}

}

void A2::construct_view_port()
{
	view_port.clear();
	
	// Top left
	view_port.push_back({view_port_center.x - view_port_side_x, view_port_center.y + view_port_side_y});
	
	// Top right
	view_port.push_back({view_port_center.x + view_port_side_x, view_port_center.y + view_port_side_y});
	
	// Bottom right
	view_port.push_back({view_port_center.x + view_port_side_x, view_port_center.y - view_port_side_y});
	
	// bottom left
	view_port.push_back({view_port_center.x - view_port_side_x, view_port_center.y - view_port_side_y});
}

void A2::update_view_port(const vec2 & p1, const vec2 & p2)
{
	float t = 0.5;
	vec2 center = t * p1 + t * p2;
	view_port_center.x = center.x;
	view_port_center.y = center.y;
	view_port_side_x = center.x - std::min(p1.x, p2.x);
	view_port_side_y = center.y - std::min(p1.y, p2.y);
}

void A2::drawLineInViewPort(const glm::vec2 & v0, const glm::vec2 & v1)
{
	mat4 scale_matrix = scale(mat4(), vec3(view_port_side_x, view_port_side_y, 1));
	mat4 translation_matrix = translate(mat4(), vec3(view_port_center.x, view_port_center.y, 0));
	
	vec4 point1(v0.x, v0.y, 0, 1);
	vec4 point2(v1.x, v1.y, 0, 1);
	
	point1 = translation_matrix * scale_matrix * point1;
	point2 = translation_matrix * scale_matrix * point2;
	
	drawLine({point1.x, point1.y}, {point2.x, point2.y});
}

bool A2::clip(vec4& v1, vec4& v2)
{
	for (int i = 0; i < 6; i++){
		int c1 = 0;
		int c2 = 0;
		
		float bl1 = v1.w + v1.x;
		if (bl1 < 0)
			c1 = c1 << 1 | 1;
		else
			c1 = c1 << 1 | 0;
		
		float br1 = v1.w - v1.x;
		if (br1 < 0)
			c1 = c1 << 1 | 1;
		else
			c1 = c1 << 1 | 0;
		
		float bb1 = v1.w + v1.y;
		if (bb1 < 0)
			c1 = c1 << 1 | 1;
		else
			c1 = c1 << 1 | 0;
		
		float bt1 = v1.w - v1.y;
		if (bt1 < 0)
			c1 = c1 << 1 | 1;
		else
			c1 = c1 << 1 | 0;
		
		float bn1 = v1.w + v1.z;
		if (bn1 < 0)
			c1 = c1 << 1 | 1;
		else
			c1 = c1 << 1 | 0;
		
		float bf1 = v1.w - v1.z;
		if (bf1 < 0)
			c1 = c1 << 1 | 1;
		else
			c1 = c1 << 1 | 0;
		
		float bl2 = v2.w + v2.x;
		if (bl2 < 0)
			c2 = c2 << 1 | 1;
		else
			c2 = c2 << 1 | 0;
		
		float br2 = v2.w - v2.x;
		if (br2 < 0)
			c2 = c2 << 1 | 1;
		else
			c2 = c2 << 1 | 0;
		
		float bb2 = v2.w + v2.y;
		if (bb2 < 0)
			c2 = c2 << 1 | 1;
		else
			c2 = c2 << 1 | 0;
		
		float bt2 = v2.w - v2.y;
		if (bt2 < 0)
			c2 = c2 << 1 | 1;
		else
			c2 = c2 << 1 | 0;
		
		float bn2 = v2.w + v2.z;
		if (bn2 < 0)
			c2 = c2 << 1 | 1;
		else
			c2 = c2 << 1 | 0;
		
		float bf2 = v2.w - v2.z;
		if (bf2 < 0)
			c2 = c2 << 1 | 1;
		else
			c2 = c2 << 1 | 0;
		
		if (c1 & c2) {
			// trivally reject
			return false;
		}
		
		if ((c1 | c2) == 0) {
			// trivally accept
			return true;
		}
		
		// non-trival case. Clip against each plane.
		float t = 0.0;
		switch (i) {
			case 0:
				t = bl1 / (bl1 - bl2);
				if (bl1 < 0) v1 = (1 - t) * v1 + t * v2;
				if (bl2 < 0) v2 = (1 - t) * v1 + t * v2;
				break;
			case 1:
				t = br1 / (br1 - br2);
				if (br1 < 0) v1 = (1 - t) * v1 + t * v2;
				if (br2 < 0) v2 = (1 - t) * v1 + t * v2;
				break;
			case 2:
				t = bt1 / (bt1 - bt2);
				if (bt1 < 0) v1 = (1 - t) * v1 + t * v2;
				if (bt2 < 0) v2 = (1 - t) * v1 + t * v2;
				break;
			case 3:
				t = bb1 / (bb1 - bb2);
				if (bb1 < 0) v1 = (1 - t) * v1 + t * v2;
				if (bb2 < 0) v2 = (1 - t) * v1 + t * v2;
				break;
			case 4:
				t = bn1 / (bn1 - bn2);
				if (bn1 < 0) v1 = (1 - t) * v1 + t * v2;
				if (bn2 < 0) v2 = (1 - t) * v1 + t * v2;
				break;
			case 5:
				t = bf1 / (bf1 - bf2);
				if (bf1 < 0) v1 = (1 - t) * v1 + t * v2;
				if (bf2 < 0) v2 = (1 - t) * v1 + t * v2;
				break;
			default:
				break;
		}
	}
	
    return true;
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
        for (char mode : vector<char>({'O', 'N', 'P', 'R', 'T', 'S', 'V'})) {
            switch (mode) {
                case 'O':
                    ImGui::Text("Rotate View (O):          ");
                    break;
                case 'N':
                    ImGui::Text("Translate View (N):       ");
                    break;
                case 'P':
                    ImGui::Text("Perspective (P):          ");
                    break;
                case 'R':
                    ImGui::Text("Rotate Model (R):         ");
                    break;
                case 'T':
                    ImGui::Text("Translate Model (T):      ");
                    break;
                case 'S':
                    ImGui::Text("Scale Model (S):          ");
                    break;
                case 'V':
                    ImGui::Text("Viewport (V):             ");
                    break;
                default:
                    break;
            }
            std::stringstream ss;
            ss << mode;
            bool selected;
            selected = (curr_mode == mode);
            ImGui::SameLine();
            if (ImGui::RadioButton(ss.str().c_str(), selected)) {
                curr_mode = mode;
            }
        }
    

		// Create Button, and check if it was clicked:
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
	
		if( ImGui::Button( "Reset" ) ) {
			reset();
		}

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );
        ImGui::Text( "Near Plane: %.2f distance away.", near);
        ImGui::Text( "Far  Plane: %.2f distance away.", far);

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
	
//	cout << xPos << " " << yPos << endl;

	// Fill in with event handling code...
    if (!ImGui::IsMouseHoveringAnyWindow()) {
        // Put some code here to handle rotations.  Probably need to
        // check whether we're *dragging*, not just moving the mouse.
        // Probably need some instance variables to track the current
        // rotation amount, and maybe the previous X position (so
        // that you can rotate relative to the *change* in X.
        double difference = xPos - mouse_x_pos;
        if (curr_mode == 'R'){
            
            if (ImGui::IsMouseDown(0)) {
                M = M * rotate(mat4(), vec3(difference * 0.01, 0, 0));
            }
            
            if (ImGui::IsMouseDown(2)) {
                M = M * rotate(mat4(), vec3(0, difference * 0.01, 0));
            }
            
            if (ImGui::IsMouseDown(1)) {
                M = M * rotate(mat4(), vec3(0, 0, difference * 0.01));
            }
        } else if (curr_mode == 'T') {
            if (ImGui::IsMouseDown(0)) {
                M = M * translate(mat4(), vec3(difference * 0.01, 0, 0));
            }
            
            if (ImGui::IsMouseDown(2)) {
                M = M * translate(mat4(), vec3(0, difference * 0.01, 0));
            }
            
            if (ImGui::IsMouseDown(1)) {
                M = M * translate(mat4(), vec3(0, 0, difference * 0.01));
            }
        } else if (curr_mode == 'S') {
            if (ImGui::IsMouseDown(0)) {
                M = M * scale(mat4(), vec3(1 + difference * 0.01, 1, 1));
            }
            
            if (ImGui::IsMouseDown(2)) {
                M = M * scale(mat4(), vec3(1, 1 + difference * 0.01, 1));
            }
            
            if (ImGui::IsMouseDown(1)) {
                M = M * scale(mat4(), vec3(1, 1, 1 + difference * 0.01));
            }
        } else if (curr_mode == 'O') {
            if (ImGui::IsMouseDown(0)) {
                V = inverse(rotate(mat4(), vec3(difference * 0.01, 0, 0))) * V;
            }
            
            if (ImGui::IsMouseDown(2)) {
                V = inverse(rotate(mat4(), vec3(0, difference * 0.01, 0))) * V;
            }
            
            if (ImGui::IsMouseDown(1)) {
                V = inverse(rotate(mat4(), vec3(0, 0, difference * 0.01))) * V;
            }
        } else if (curr_mode == 'N') {
            if (ImGui::IsMouseDown(0)) {
                V = inverse(translate(mat4(), vec3(difference * 0.01, 0, 0))) * V;
            }
            
            if (ImGui::IsMouseDown(2)) {
                V = inverse(translate(mat4(), vec3(0, difference * 0.01, 0))) * V;
            }
            
            if (ImGui::IsMouseDown(1)) {
                V = inverse(translate(mat4(), vec3(0, 0, difference * 0.01))) * V;
            }
        } else if (curr_mode == 'P') {
            
            if (ImGui::IsMouseDown(0)) {
                fov = fov + difference * 0.01;
                if (fov > 160) {
                    fov = 160;
                }
                if (fov < 5) {
                    fov = 5;
                }
			}
            
            if (ImGui::IsMouseDown(2)) {
                float new_near = near + difference * 0.01;
                near = new_near;
            }
            
            if (ImGui::IsMouseDown(1)) {
                float new_far = far + difference * 0.01;
                far = new_far;
            }
			
			if (ImGui::IsMouseDown(0) || ImGui::IsMouseDown(1) || ImGui::IsMouseDown(2)) {
				update_perspective_view();
			}
			
			
		} else if (curr_mode == 'V') {
			if (ImGui::IsMouseDown(0)) {
				if (!shouldViewportResize){
					viewport_v1.first = xPos;
					viewport_v1.second = yPos;
					shouldViewportResize = true;
				} else {
					viewport_v2.first = xPos;
					viewport_v2.second = yPos;
				}
				
			}
			if (!ImGui::IsMouseDown(0)){
				if (shouldViewportResize) {
					viewport_v2.first = xPos;
					viewport_v2.second = yPos;
					shouldViewportResize = false;
				}
			}
			
		}

        mouse_x_pos = xPos;
    }
    
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
                curr_mode = 'T';
				break;
			case GLFW_KEY_R:
				cout << "Model Rotation Mode" << endl;
                curr_mode = 'R';
				break;
			case GLFW_KEY_S:
				cout << "Model Scale Mode" << endl;
                curr_mode = 'S';
				break;
            case GLFW_KEY_N:
                cout << "View Translation Mode " << endl;
                curr_mode = 'N';
                break;
            case GLFW_KEY_O:
                cout << "View Rotation Mode" << endl;
                curr_mode = 'O';
                break;
            case GLFW_KEY_W:
                curr_mode = 'W';
                break;
            case GLFW_KEY_P:
                curr_mode = 'P';
                eventHandled = true;
                break;
			case GLFW_KEY_V:
				curr_mode = 'V';
				break;
			case GLFW_KEY_A:
				reset();
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

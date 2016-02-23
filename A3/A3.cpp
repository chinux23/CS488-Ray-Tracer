#include "A3.hpp"
#include "scene_lua.hpp"
using namespace std;

#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"
#include "GeometryNode.hpp"
#include "JointNode.hpp"

#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <deque>
#include <memory>

using namespace glm;

static bool show_gui = true;

const size_t CIRCLE_PTS = 48;

//----------------------------------------------------------------------------------------
// Constructor
A3::A3(const std::string & luaSceneFile)
	: m_luaSceneFile(luaSceneFile),
	  m_positionAttribLocation(0),
	  m_normalAttribLocation(0),
	  m_vao_meshData(0),
	  m_vbo_vertexPositions(0),
	  m_vbo_vertexNormals(0),
	  m_vao_arcCircle(0),
	  m_vbo_arcCircle(0)
{

}

//----------------------------------------------------------------------------------------
// Destructor
A3::~A3()
{

}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A3::init()
{
	do_picking = false;
	
	option_circle_enabled		= false;
	option_zbuffer_enabled		= true;
	option_backface_culling		= false;
	option_frontface_culling	= false;
	
	curr_mode = Mode_PositionOrientation;

	// Set the background colour.
	glClearColor(0.35, 0.35, 0.35, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao_arcCircle);
	glGenVertexArrays(1, &m_vao_meshData);
	enableVertexShaderInputSlots();

	processLuaSceneFile(m_luaSceneFile);

	// Load and decode all .obj files at once here.  You may add additional .obj files to
	// this list in order to support rendering additional mesh types.  All vertex
	// positions, and normals will be extracted and stored within the MeshConsolidator
	// class.
	unique_ptr<MeshConsolidator> meshConsolidator (new MeshConsolidator{
			getAssetFilePath("cube.obj"),
			getAssetFilePath("sphere.obj"),
			getAssetFilePath("suzanne.obj")
	});


	// Acquire the BatchInfoMap from the MeshConsolidator.
	meshConsolidator->getBatchInfoMap(m_batchInfoMap);

	// Take all vertex data within the MeshConsolidator and upload it to VBOs on the GPU.
	uploadVertexDataToVbos(*meshConsolidator);

	mapVboDataToVertexShaderInputLocations();

	initPerspectiveMatrix();

	initViewMatrix();

	initLightSources();
	
	
	// Exiting the current scope calls delete automatically on meshConsolidator freeing
	// all vertex data resources.  This is fine since we already copied this data to
	// VBOs on the GPU.  We have no use for storing vertex data on the CPU side beyond
	// this point.
}

//----------------------------------------------------------------------------------------
void A3::processLuaSceneFile(const std::string & filename) {
	// This version of the code treats the Lua file as an Asset,
	// so that you'd launch the program with just the filename
	// of a puppet in the Assets/ directory.
	// std::string assetFilePath = getAssetFilePath(filename.c_str());
	// m_rootNode = std::shared_ptr<SceneNode>(import_lua(assetFilePath));

	// This version of the code treats the main program argument
	// as a straightforward pathname.
	m_rootNode = std::shared_ptr<SceneNode>(import_lua(filename));
	if (!m_rootNode) {
		std::cerr << "Could not open " << filename << std::endl;
	}
}

//----------------------------------------------------------------------------------------
void A3::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();

	m_shader_arcCircle.generateProgramObject();
	m_shader_arcCircle.attachVertexShader( getAssetFilePath("arc_VertexShader.vs").c_str() );
	m_shader_arcCircle.attachFragmentShader( getAssetFilePath("arc_FragmentShader.fs").c_str() );
	m_shader_arcCircle.link();
}

//----------------------------------------------------------------------------------------
void A3::enableVertexShaderInputSlots()
{
	//-- Enable input slots for m_vao_meshData:
	{
		glBindVertexArray(m_vao_meshData);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_positionAttribLocation = m_shader.getAttribLocation("position");
		glEnableVertexAttribArray(m_positionAttribLocation);

		// Enable the vertex shader attribute location for "normal" when rendering.
		m_normalAttribLocation = m_shader.getAttribLocation("normal");
		glEnableVertexAttribArray(m_normalAttribLocation);

		CHECK_GL_ERRORS;
	}


	//-- Enable input slots for m_vao_arcCircle:
	{
		glBindVertexArray(m_vao_arcCircle);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_arc_positionAttribLocation = m_shader_arcCircle.getAttribLocation("position");
		glEnableVertexAttribArray(m_arc_positionAttribLocation);

		CHECK_GL_ERRORS;
	}

	// Restore defaults
	glBindVertexArray(0);
}

//----------------------------------------------------------------------------------------
void A3::uploadVertexDataToVbos (
		const MeshConsolidator & meshConsolidator
) {
	// Generate VBO to store all vertex position data
	{
		glGenBuffers(1, &m_vbo_vertexPositions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);

		glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexPositionBytes(),
				meshConsolidator.getVertexPositionDataPtr(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}

	// Generate VBO to store all vertex normal data
	{
		glGenBuffers(1, &m_vbo_vertexNormals);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);

		glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexNormalBytes(),
				meshConsolidator.getVertexNormalDataPtr(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}

	// Generate VBO to store the trackball circle.
	{
		glGenBuffers( 1, &m_vbo_arcCircle );
		glBindBuffer( GL_ARRAY_BUFFER, m_vbo_arcCircle );

		float *pts = new float[ 2 * CIRCLE_PTS ];
		for( size_t idx = 0; idx < CIRCLE_PTS; ++idx ) {
			float ang = 2.0 * M_PI * float(idx) / CIRCLE_PTS;
			pts[2*idx] = cos( ang );
			pts[2*idx+1] = sin( ang );
		}

		glBufferData(GL_ARRAY_BUFFER, 2*CIRCLE_PTS*sizeof(float), pts, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A3::mapVboDataToVertexShaderInputLocations()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao_meshData);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexPositions" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);
	glVertexAttribPointer(m_positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexNormals" into the
	// "normal" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);
	glVertexAttribPointer(m_normalAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;

	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao_arcCircle);

	// Tell GL how to map data from the vertex buffer "m_vbo_arcCircle" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_arcCircle);
	glVertexAttribPointer(m_arc_positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A3::initPerspectiveMatrix()
{
	float aspect = ((float)m_windowWidth) / m_windowHeight;
	m_perpsective = glm::perspective(degreesToRadians(60.0f), aspect, 0.1f, 100.0f);
}


//----------------------------------------------------------------------------------------
void A3::initViewMatrix() {
	m_view = glm::lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f),
			vec3(0.0f, 1.0f, 0.0f));
}

//----------------------------------------------------------------------------------------
void A3::initLightSources() {
	// World-space position
	m_light.position = vec3(-2.0f, 5.0f, 0.5f);
	m_light.rgbIntensity = vec3(0.8f); // White light
}

//----------------------------------------------------------------------------------------
void A3::uploadCommonSceneUniforms() {
	m_shader.enable();
	{
		//-- Set Perpsective matrix uniform for the scene:
		GLint location = m_shader.getUniformLocation("Perspective");
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m_perpsective));
		CHECK_GL_ERRORS;

		location = m_shader.getUniformLocation("picking");
		glUniform1i( location, do_picking ? 1 : 0 );

		if( !do_picking ) {

			//-- Set LightSource uniform for the scene:
			{
				location = m_shader.getUniformLocation("light.position");
				glUniform3fv(location, 1, value_ptr(m_light.position));
				location = m_shader.getUniformLocation("light.rgbIntensity");
				glUniform3fv(location, 1, value_ptr(m_light.rgbIntensity));
				CHECK_GL_ERRORS;
			}

			//-- Set background light ambient intensity
			{
				location = m_shader.getUniformLocation("ambientIntensity");
				vec3 ambientIntensity(0.05f);
				glUniform3fv(location, 1, value_ptr(ambientIntensity));
				CHECK_GL_ERRORS;
			}
		}
	}
	m_shader.disable();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A3::appLogic()
{
	// Place per frame, application logic here ...

	uploadCommonSceneUniforms();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A3::guiLogic()
{
	if( !show_gui ) {
		return;
	}

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
		if (ImGui::BeginMainMenuBar()) {
	        if (ImGui::BeginMenu("Application")) {
	            if (ImGui::MenuItem("Reset Position")) {
					resetPosition();
				}
				
				if (ImGui::MenuItem("Reset Orientation", "O")) {
					resetOrientation();
				}
				
            	if (ImGui::MenuItem("Reset Joints", "N")) {
					resetJoints();
				}
				
        		if (ImGui::MenuItem("Reset All", "A")) {
					resetAll();
				}
    			
    			if (ImGui::MenuItem("Quit", "Q")) {
    				glfwSetWindowShouldClose(m_window, GL_TRUE);
    			}
	            ImGui::EndMenu();
	        }
        
	        if (ImGui::BeginMenu("Edit")) {
	            if (ImGui::MenuItem("Undo", "U")) {
					undo();
				}
	            if (ImGui::MenuItem("Redo", "R")) {
					redo();
				}
	            ImGui::EndMenu();
	        }

	       	if (ImGui::BeginMenu("Options")) {
	            if (ImGui::MenuItem("Circle", "C", &option_circle_enabled, true)) {}
	            if (ImGui::MenuItem("Z-buffer", "Z", &option_zbuffer_enabled, true)) {}
            	if (ImGui::MenuItem("Backface culling", "B", &option_backface_culling, true)) {}
        		if (ImGui::MenuItem("Frontface culling", "F", &option_frontface_culling, true)) {}
	            ImGui::EndMenu();
	        }
	        ImGui::EndMainMenuBar();
    	}

    	bool selected;
		selected = curr_mode == Mode_PositionOrientation;
        if (ImGui::RadioButton("Position/Orientation (P)", selected)) {
			curr_mode = Mode_PositionOrientation;
		}
	
		selected = curr_mode == Mode_Joints;
        if (ImGui::RadioButton("Joints               (J)", selected)) {
			curr_mode = Mode_Joints;
		}
		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();
}

//----------------------------------------------------------------------------------------
// Update mesh specific shader uniforms:
static void updateShaderUniforms(
		const ShaderProgram & shader,
		const GeometryNode & node,
		const glm::mat4 & viewMatrix
) {

	shader.enable();
	{
		//-- Set ModelView matrix:
		GLint location = shader.getUniformLocation("ModelView");
		mat4 modelView = viewMatrix * node.trans;
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(modelView));
		CHECK_GL_ERRORS;

		//-- Set NormMatrix:
		location = shader.getUniformLocation("NormalMatrix");
		mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelView)));
		glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(normalMatrix));
		CHECK_GL_ERRORS;


		//-- Set Material values:
		location = shader.getUniformLocation("material.kd");
		vec3 kd = node.material.kd;
		glUniform3fv(location, 1, value_ptr(kd));
		CHECK_GL_ERRORS;
		location = shader.getUniformLocation("material.ks");
		vec3 ks = node.material.ks;
		glUniform3fv(location, 1, value_ptr(ks));
		CHECK_GL_ERRORS;
		location = shader.getUniformLocation("material.shininess");
		glUniform1f(location, node.material.shininess);
		CHECK_GL_ERRORS;

	}
	shader.disable();

}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A3::draw() {
	
	if (option_zbuffer_enabled) {
		glEnable( GL_DEPTH_TEST );
	}
	
	enableFaceCulling();
	
	renderSceneGraph(*m_rootNode);
	
	if (option_zbuffer_enabled) {
		glDisable( GL_DEPTH_TEST );
	}
	
	disableFaceCulling();
		
	if (option_circle_enabled) {
		renderArcCircle();
	}
}

void A3::enableFaceCulling()
{
	if (option_frontface_culling & !option_backface_culling) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	} else if (option_backface_culling & !option_frontface_culling) {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	} else if (option_frontface_culling & option_backface_culling){
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT_AND_BACK);
	} else {
		glDisable(GL_CULL_FACE);
	}
}

void A3::disableFaceCulling()
{
	glDisable(GL_CULL_FACE);
}

//----------------------------------------------------------------------------------------
void A3::renderSceneGraph(const SceneNode & root) {

	// Bind the VAO once here, and reuse for all GeometryNode rendering below.
	glBindVertexArray(m_vao_meshData);

	// This is emphatically *not* how you should be drawing the scene graph in
	// your final implementation.  This is a non-hierarchical demonstration
	// in which we assume that there is a list of GeometryNodes living directly
	// underneath the root node, and that we can draw them in a loop.  It's
	// just enough to demonstrate how to get geometry and materials out of
	// a GeometryNode and onto the screen.

	// You'll want to turn this into recursive code that walks over the tree.
	// You can do that by putting a method in SceneNode, overridden in its
	// subclasses, that renders the subtree rooted at every node.  Or you
	// could put a set of mutually recursive functions in this class, which
	// walk down the tree from nodes of different types.

	// When traverse the tree, use the stack to keep track of current transformations
	// cout << "Render Scene Graph" << endl;

	std::deque<glm::mat4> trans_stack;

	root.render(m_shader, m_view, m_batchInfoMap, trans_stack);
	
	glBindVertexArray(0);
	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
// Draw the trackball circle.
void A3::renderArcCircle() {
	glBindVertexArray(m_vao_arcCircle);

	m_shader_arcCircle.enable();
		GLint m_location = m_shader_arcCircle.getUniformLocation( "M" );
		float aspect = float(m_framebufferWidth)/float(m_framebufferHeight);
		glm::mat4 M;
		if( aspect > 1.0 ) {
			M = glm::scale( glm::mat4(), glm::vec3( 0.5/aspect, 0.5, 1.0 ) );
		} else {
			M = glm::scale( glm::mat4(), glm::vec3( 0.5, 0.5*aspect, 1.0 ) );
		}
		glUniformMatrix4fv( m_location, 1, GL_FALSE, value_ptr( M ) );
		glDrawArrays( GL_LINE_LOOP, 0, CIRCLE_PTS );
	m_shader_arcCircle.disable();

	glBindVertexArray(0);
	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A3::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A3::cursorEnterWindowEvent (
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
bool A3::mouseMoveEvent (
		double xPos,
		double yPos
) {
	bool eventHandled(false);
	
//	cout << "xPos: " << xPos << " yPos: " << yPos << endl;

	// Fill in with event handling code...
	double xdiff = xPos - mouse_x_pos;
	double ydiff = yPos - mouse_y_pos;
	
	if (curr_mode == Mode_PositionOrientation && sub_mode == SubMode1) {
		// Translate puppet in X Y.
		m_rootNode->translate(vec3(xdiff * 0.01, -ydiff * 0.01, 0));
//		MoveCommand * cmd = (MoveCommand *)curr_cmd.get();
//		cmd->trans = glm::translate(glm::mat4(), vec3(xdiff * 0.01, -ydiff * 0.01, 0)) * cmd->trans;
//		cout << "move command: " << cmd->trans << endl;
//		cout << "curr command: " << ((MoveCommand *)curr_cmd.get())->trans << endl;
	}
	
	if (curr_mode == Mode_PositionOrientation && sub_mode == SubMode2) {
		// Translate pupeet in Z
		m_rootNode->translate(vec3(0, 0, ydiff * 0.01));
//		MoveCommand * cmd = (MoveCommand *)curr_cmd.get();
//		cmd->trans = glm::translate(cmd->trans, vec3(0, 0, ydiff * 0.01));
	}
	
	if (curr_mode == Mode_PositionOrientation && sub_mode == SubMode3) {
		xdiff = -xdiff;
		ydiff = -ydiff;
		glm::vec3 va = arcballVector(mouse_x_pos, mouse_y_pos);
		glm::vec3 vb = arcballVector(xPos, yPos);
		
		// angle in view coordinate
		float angle_in_view_frame = acos(std::min(1.0f, glm::dot(va, vb)));
		
		// normal in view coordinate
		glm::vec3 axis = glm::cross(va, vb);
		glm::vec4 axis_in_viewframe = {axis.x, axis.y, axis.z, 0};	// vector
		
		glm::vec4 axis_in_worldframe = glm::inverse(m_view) * axis_in_viewframe;
		
//		cout << "Rotation angle is " << glm::degrees(angle_in_view_frame) << endl;
//		cout << "Rotating around axis: " << axis_in_worldframe << endl;
		
		glm::mat4 rotation = glm::rotate(glm::mat4(),
										 glm::degrees(angle_in_view_frame),
										 {axis_in_worldframe.x, axis_in_worldframe.y, axis_in_worldframe.z});
		m_rootNode->trans = m_rootNode->trans * rotation;
		
//		RotateCommand * cmd = (RotateCommand *)curr_cmd.get();
//		cmd->trans = cmd->trans * rotation;
	}
    
    if (curr_mode == Mode_Joints && sub_mode == SubMode2) {
        float angle = ydiff;
        for (auto joint : selected_joints) {
            cout << "rotating angle " << angle << endl;
            joint->rotate(angle);
        }
        JointRotateCommand * cmd = (JointRotateCommand *)curr_cmd.get();
        cmd->m_angle += angle;
    }
    
    if (curr_mode == Mode_Joints && sub_mode == SubMode3) {
        
    }

	mouse_x_pos = xPos;
	mouse_y_pos = yPos;
	return eventHandled;
}

glm::vec3 A3::arcballVector(float x, float y)
{
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	glm::vec3 P = glm::vec3(x/width * 2 - 1.0, y / height * 2 - 1.0, 0);
	
	// The following code is referenced from
	// https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Arcball
	float OP_squared = P.x * P.x + P.y * P.y;
	if (OP_squared <= 1*1) {
		P.z = sqrt(1*1 - OP_squared);
	} else {
		P = glm::normalize(P);
	}
	return P;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A3::mouseButtonInputEvent (
		int button,
		int actions,
		int mods
) {
	bool eventHandled(false);

	// Fill in with event handling code...
    
    if (!ImGui::IsMouseHoveringAnyWindow()) {
	
        if (curr_mode == Mode_PositionOrientation) {
            if (button == GLFW_MOUSE_BUTTON_LEFT && actions == GLFW_PRESS) {
                sub_mode = SubMode1;
    //			std::vector<SceneNode *> actors = {m_rootNode.get()};
    //			MoveCommand *move_cmd = new MoveCommand(actors, glm::mat4());
    //			curr_cmd.reset(move_cmd);
            }
            
            if (button == GLFW_MOUSE_BUTTON_LEFT && actions == GLFW_RELEASE) {
                sub_mode = SubMode_Unselected;
    //			cout << "push back curr command: " << ((MoveCommand *)curr_cmd.get())->trans << endl;
    //			commands.push_back(std::move(curr_cmd));
    //			curr_cmd.reset(nullptr);
    //			cout << "pushed back curr command: " << ((MoveCommand *)commands[0].get())->trans << endl;
            }
            
            if (button == GLFW_MOUSE_BUTTON_MIDDLE && actions == GLFW_PRESS) {
                sub_mode = SubMode2;
    //			std::vector<SceneNode *> actors = {m_rootNode.get()};
    //			MoveCommand *move_cmd = new MoveCommand(actors, glm::mat4());
    //			curr_cmd.reset(move_cmd);
            }
            
            if (button == GLFW_MOUSE_BUTTON_MIDDLE && actions == GLFW_RELEASE) {
                sub_mode = SubMode_Unselected;
    //			commands.push_back(std::move(curr_cmd));
    //			curr_cmd.reset(nullptr);
            }
            
            if (button == GLFW_MOUSE_BUTTON_RIGHT && actions == GLFW_PRESS) {
                sub_mode = SubMode3;
    //			std::vector<SceneNode *> actors = {m_rootNode.get()};
    //			RotateCommand *move_cmd = new RotateCommand(actors, glm::mat4());
    //			curr_cmd.reset(move_cmd);
            }
            
            if (button == GLFW_MOUSE_BUTTON_RIGHT && actions == GLFW_RELEASE) {
                sub_mode = SubMode_Unselected;
    //			commands.push_back(std::move(curr_cmd));
    //			curr_cmd.reset(nullptr);
            }
            
        } else if (curr_mode == Mode_Joints) {
            if (button == GLFW_MOUSE_BUTTON_LEFT && actions == GLFW_PRESS) {
                double xpos, ypos;
                glfwGetCursorPos( m_window, &xpos, &ypos );
                
                do_picking = true;
                
                if (do_picking) {
                    m_rootNode->enablePicking();
                } else {
                    m_rootNode->disablePicking();
                }
                
                uploadCommonSceneUniforms();
                glClearColor(1.0, 1.0, 1.0, 1.0 );
                glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
                glClearColor(0.35, 0.35, 0.35, 1.0);
                
                draw();
                
                // Ugly -- FB coordinates might be different than Window coordinates
                // (e.g., on a retina display).  Must compensate.
                xpos *= double(m_framebufferWidth) / double(m_windowWidth);
                // WTF, don't know why I have to measure y relative to the bottom of
                // the window in this case.
                ypos = m_windowHeight - ypos;
                ypos *= double(m_framebufferHeight) / double(m_windowHeight);
                
                GLubyte buffer[ 4 ] = { 0, 0, 0, 0 };
                // A bit ugly -- don't want to swap the just-drawn false colours
                // to the screen, so read from the back buffer.
                glReadBuffer( GL_BACK );
                // Actually read the pixel at the mouse location.
                glReadPixels( int(xpos), int(ypos), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, buffer );
                CHECK_GL_ERRORS;
                
                // Reassemble the object ID.
                unsigned int what = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16);
                
                if (m_rootNode->hasID(what)) {
                    SceneNode *node = m_rootNode->nodeFromID(what);
//                    cout << "Picking node with ID: " << what << " [" << node->m_name << "]" << endl;
                    if (node->parent->m_nodeType == NodeType::JointNode) {
                        JointNode *jnode = (JointNode *)node->parent;
                        jnode->isSelected = !jnode->isSelected;
                        if (jnode->isSelected) {
                            // This lucky joint node is selected.
                            cout << jnode->m_name << " is selected" << endl;
                            selected_joints.insert(jnode);
                        } else {
                            cout << jnode->m_name << " is deselected" << endl;
                            selected_joints.erase(jnode);
                        }
                        
                    }
                }
                
                do_picking = false;
                if (do_picking) {
                    m_rootNode->enablePicking();
                } else {
                    m_rootNode->disablePicking();
                }
                
                CHECK_GL_ERRORS;
            }
            
            if (button == GLFW_MOUSE_BUTTON_RIGHT && actions == GLFW_PRESS) {
                // Rotate all selected joints.
                sub_mode = SubMode2;
                JointRotateCommand *joint_cmd = new JointRotateCommand({selected_joints.begin(), selected_joints.end()}, 0);
                curr_cmd.reset(joint_cmd);
            }
            
            if (button == GLFW_MOUSE_BUTTON_RIGHT && actions == GLFW_RELEASE) {
                sub_mode = SubMode_Unselected;
                commands.push_back(std::move(curr_cmd));
                curr_cmd.reset(nullptr);
            }
            
            if (button == GLFW_MOUSE_BUTTON_MIDDLE && actions == GLFW_PRESS) {
                // Rotate head.
                sub_mode = SubMode3;
            }
            
            if (button == GLFW_MOUSE_BUTTON_MIDDLE && actions == GLFW_RELEASE) {
                sub_mode = SubMode_Unselected;
            }
            
        }
            
    }

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A3::mouseScrollEvent (
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
bool A3::windowResizeEvent (
		int width,
		int height
) {
	bool eventHandled(false);
	initPerspectiveMatrix();
	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A3::keyInputEvent (
		int key,
		int action,
		int mods
) {
	bool eventHandled(false);

	if( action == GLFW_PRESS ) {
		if( key == GLFW_KEY_M ) {
			show_gui = !show_gui;
			eventHandled = true;
		}
		
		if ( key == GLFW_KEY_U) {
			undo();
		}
		
		if ( key == GLFW_KEY_R) {
			redo();
		}
	}
	// Fill in with event handling code...

	return eventHandled;
}

void A3::resetPosition()
{
	
}

void A3::resetOrientation()
{
	
}

void A3::resetJoints()
{
	
}

void A3::resetAll()
{
	resetPosition();
	resetOrientation();
	resetJoints();
}

void A3::undo()
{
//	cout << "calling undo " << endl;
//	cout << "undo command: " << ((MoveCommand *)commands[0].get())->trans << endl;
	if (commands.size()) {
		std::unique_ptr<Command> cmd = std::move(commands.back());
		commands.pop_back();
		cmd->undo();
	}

}

void A3::redo()
{
	cout << "calling redo " << endl;
}


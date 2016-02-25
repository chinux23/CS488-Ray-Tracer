#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include "cs488-framework/MeshConsolidator.hpp"

#include "SceneNode.hpp"
#include "Command.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <deque>
#include <unordered_set>


struct LightSource {
	glm::vec3 position;
	glm::vec3 rgbIntensity;
};

typedef enum {
	Option_Circle,
	Option_Z_Buffer,
	Option_Backface_Culling,
	Option_Frontface_Culling
} OPTIONS_TYPE;

typedef enum {
	Mode_PositionOrientation,
	Mode_Joints
} MODE_TYPE;

typedef enum {
	SubMode_Unselected,
	SubMode1,
	SubMode2,
	SubMode3
} SUBMODE_TYPE;


class A3 : public CS488Window {
public:
	A3(const std::string & luaSceneFile);
	virtual ~A3();

protected:
	virtual void init() override;
	virtual void appLogic() override;
	virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	//-- Virtual callback methods
	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;

	//-- One time initialization methods:
	void processLuaSceneFile(const std::string & filename);
	void createShaderProgram();
	void enableVertexShaderInputSlots();
	void uploadVertexDataToVbos(const MeshConsolidator & meshConsolidator);
	void mapVboDataToVertexShaderInputLocations();
	void initViewMatrix();
	void initLightSources();

	void initPerspectiveMatrix();
	void uploadCommonSceneUniforms();
	void renderSceneGraph(const SceneNode &node);
	void renderArcCircle();

	glm::mat4 m_perpsective;
	glm::mat4 m_view;

	LightSource m_light;

	//-- GL resources for mesh geometry data:
	GLuint m_vao_meshData;
	GLuint m_vbo_vertexPositions;
	GLuint m_vbo_vertexNormals;
	GLint m_positionAttribLocation;
	GLint m_normalAttribLocation;
	ShaderProgram m_shader;

	//-- GL resources for trackball circle geometry:
	GLuint m_vbo_arcCircle;
	GLuint m_vao_arcCircle;
	GLint m_arc_positionAttribLocation;
	ShaderProgram m_shader_arcCircle;

	// BatchInfoMap is an associative container that maps a unique MeshId to a BatchInfo
	// object. Each BatchInfo object contains an index offset and the number of indices
	// required to render the mesh with identifier MeshId.
	BatchInfoMap m_batchInfoMap;

	std::string m_luaSceneFile;

	std::shared_ptr<SceneNode> m_rootNode;

private:
	bool do_picking;	// indicate whether we should pick
	
private:
	bool option_circle_enabled;
	bool option_zbuffer_enabled;
	bool option_backface_culling;
	bool option_frontface_culling;
	
	void enableFaceCulling();
	void disableFaceCulling();
	
	MODE_TYPE curr_mode;
	SUBMODE_TYPE sub_mode;		// Submodes inside Position/Orientation or Joints.
	
	// mouse position
	double mouse_x_pos;
	double mouse_y_pos;
	
	// computer the vector in view coordinates.
	glm::vec3 arcballVector(float x, float y);
	
public:
	void resetPosition();
	void resetOrientation();
	void resetJoints();
	void resetAll();
	void undo();
	void redo();
	
	
private:
	std::unique_ptr<Command> curr_cmd;
	std::deque<std::unique_ptr<Command>> commands;
	std::deque<std::unique_ptr<Command>> redo_queue;
    std::unordered_set<JointNode*> selected_joints;
	
	glm::mat4 translation;
	glm::mat4 rotation;
	
	SceneNode *headNode;
	void findHead();
	float headAngle;
};

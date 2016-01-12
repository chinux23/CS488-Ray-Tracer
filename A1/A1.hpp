#pragma once

#include <glm/glm.hpp>

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include "grid.hpp"

#include "cube.hpp"
#include <memory>
#include <chrono>


using CubeStack = std::vector<std::shared_ptr<Cube::Cube>>;
using Clock		= std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;


class A1 : public CS488Window {
public:
	A1();
	virtual ~A1();

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

private:
	void initGrid();

	// Fields related to the shader and uniforms.
	ShaderProgram m_shader;
	GLint P_uni; // Uniform location for Projection matrix.
	GLint V_uni; // Uniform location for View matrix.
	GLint M_uni; // Uniform location for Model matrix.
	GLint col_uni;   // Uniform location for cube colour.

	// Fields related to grid geometry.
	GLuint m_grid_vao; // Vertex Array Object
	GLuint m_grid_vbo; // Vertex Buffer Object

	// Matrices controlling the camera and projection.
	glm::mat4 proj;
	glm::mat4 view;

	float colour[3];
	float colors[8][3];
	int current_col;
	

	/* ------------- Mod ----------------- */
	
	CubeStack cubes;
	
	// grid_of_cubes[x][z] will points to a specific square.
	// first index [x] represents the X-axis. Second index [z] represents the Z-axis.
	std::vector<std::vector<CubeStack>> grid_of_cubes;
	
	// position of active cell. (in XZ plan)
	std::pair<GLint, GLint> active_cell_position;
	
public:
	void shrinkStack();
	void extendStack();
	
	// Adjust current stack either by expanding if size is positive or shrinking if size is negative.
	void adjustCurrentStackSize(long size);
	
	void moveActiveCellUp();
	void moveActiveCellDown();
	void moveActiveCellLeft();
	void moveActiveCellRight();
	
	void updateActiveCellColor();
	
	CubeStack& activeStack();
	
private:
	void debugPrintActiveCell();
	bool isCubeStackActive(const CubeStack& cube);
	TimePoint t_start;

	void enableShiftCopy();
	void disableShiftCopy();
	int isCopyEnabled;
};

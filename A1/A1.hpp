#pragma once

#include <glm/glm.hpp>

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include "grid.hpp"

#include "cube.hpp"
#include <memory>
#include <chrono>


using CubeStack = std::vector<std::shared_ptr<Cube>>;
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
	float colors[8][3];		// Used to store all 8 colors.
	int current_col;		// Indicate current color selected.
	

	/* ------------- Mod ----------------- */
	// Create a single stack of cubes.
	CubeStack cubes;
	
	// grid_of_cubes[x][z] will points to a specific square.
	// first index [x] represents the X-axis. Second index [z] represents the Z-axis.
	std::vector<std::vector<CubeStack>> grid_of_cubes;
	
	// position of active cell. (in XZ plan)
	std::pair<GLint, GLint> active_cell_position;
	
public:

	// Remove 1 cube from active cell if there are cubes there. Otherwise no op.
	void shrinkStack();

	// Add 1 cube to active cell.
	void extendStack();
	
	// Adjust current stack either by expanding if size is positive or shrinking if size is negative.
	void adjustCurrentStackSize(long size);
	
	// Move active cell up. Direction: negative z axis.
	void moveActiveCellUp();

	// Move active cell down. Direction: postive Z axis
	void moveActiveCellDown();

	// Move active cell to the left by 1. Direction: negative X axis.
	void moveActiveCellLeft();

	// Move active cell to the right by 1. DirectioN: positive X axis.
	void moveActiveCellRight();
	
	// Change all cubes in active cell to the color specified by current_col.
	void updateActiveCellColor();
	
	// Reference the cube stack in the active cell.
	CubeStack& activeStack();
	
	// Reset grid.
	void reset();

	// When active cell moves, whether it should copy from current active cell.
	int isCopyEnabled;
	
private:

	// print active cell position
	void debugPrintActiveCell();

	// Test whether the cube stack is in the active cell.
	bool isCubeStackActive(const CubeStack& cube);

	// TimePoint to indicate the start of the program.
	TimePoint t_start;

	// Since there are two Shift keys. Either one of them is pressed, Shift-Copy is enabled.
	// Enable shift copy. Intenrally it just increment isCopyEnabled by 1.
	void enableShiftCopy();

	// Disable shift copy. Internally it just decrement isCopyEnabled by 1.
	void disableShiftCopy();

	// Stores current X position of the mouse.
	double mouse_x_pos;

	// Stores current rotation speed.
	float rotation_degree;

	// Stores current scaling factor.
	float scale_factor;
};

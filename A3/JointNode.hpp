#pragma once

#include "SceneNode.hpp"

class JointNode : public SceneNode {
public:
	JointNode(const std::string & name);
	virtual ~JointNode();

	void set_joint_x(double min, double init, double max);
	void set_joint_y(double min, double init, double max);

	struct JointRange {
		double min, init, max;
	};
	
	JointRange m_joint_x, m_joint_y;
	float angle_x, angle_y;

public:

	// Override rotate to take account of limitation on rotation.
	virtual void rotate(char axis, float angle);

//	 virtual void render(
//	 	const ShaderProgram & shader,
//	 	const glm::mat4 & viewMatrix,
//	 	BatchInfoMap & m_batchInfoMap,
//	 	std::deque<glm::mat4> & stack) const;
//
//	 virtual void updateShaderUniforms(
//	 	const ShaderProgram & shader,
//	 	const glm::mat4 & viewMatrix, 
//	 	std::deque<glm::mat4> & stack) const;
    
    // Assume one joint is rotating around one axis. There is an active Axis.
    char activeAxis();
    
    // Rotate around active Axis;
    virtual void rotate(float angle);
	
	void update_transformation();
};

#include "JointNode.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "cs488-framework/MathUtils.hpp"

using namespace glm;

//---------------------------------------------------------------------------------------
JointNode::JointNode(const std::string& name)
	: SceneNode(name)
{
	m_nodeType = NodeType::JointNode;
	angle_x = 0;
	angle_y = 0;
}

//---------------------------------------------------------------------------------------
JointNode::~JointNode() {

}
 //---------------------------------------------------------------------------------------
void JointNode::set_joint_x(double min, double init, double max) {
	m_joint_x.min = min;
	m_joint_x.init = init;
	m_joint_x.max = max;

	angle_x = init;
	trans = glm::rotate(degreesToRadians(angle_x), vec3(1,0,0)) * trans;
}

//---------------------------------------------------------------------------------------
void JointNode::set_joint_y(double min, double init, double max) {
	m_joint_y.min = min;
	m_joint_y.init = init;
	m_joint_y.max = max;

	angle_y = init;
	trans = glm::rotate(degreesToRadians(angle_y), vec3(0,1,0)) * trans;
}

void JointNode::rotate(char axis, float angle) {
	vec3 rot_axis;

	switch (axis) {
		case 'x':
			rot_axis = vec3(1,0,0);
			if ((angle + angle_x) > m_joint_x.max) {
				angle_x = m_joint_x.max;
			} else if ((angle + angle_x) < m_joint_x.min) {
				angle_x = m_joint_x.min;
			} else {
				angle_x = angle_x + angle;
			}
			break;
		case 'y':
			rot_axis = vec3(0,1,0);
			if ((angle + angle_y) > m_joint_y.max) {
				angle_y = m_joint_y.max;
			} else if ((angle + angle_y) < m_joint_y.min) {
				angle_y = m_joint_y.min;
			} else {
				angle_y = angle_y + angle;
			}
	        break;
		case 'z':
			rot_axis = vec3(0,0,1);
	        break;
		default:
			break;
	}
    std::cout << "Rotating along x " << angle_x << std::endl;
    std::cout << "Rotating along y " << angle_y << std::endl;
    
	update_transformation();
}

char JointNode::activeAxis()
{
    if (m_joint_x.min == m_joint_x.max) {
        return 'y';
    } else {
        return 'x';
    }
}

void JointNode::rotate(float angle)
{
    char axis = activeAxis();
    rotate(axis, angle);
}

void JointNode::update_transformation()
{
	if (activeAxis() == 'x') {
		trans = glm::rotate(degreesToRadians(angle_x), vec3(1,0,0));
	} else {
		trans = glm::rotate(degreesToRadians(angle_y), vec3(0,1,0));
	}
}


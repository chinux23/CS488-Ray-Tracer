//
//  Command.cpp
//  CS488-Projects
//
//  Created by Chen Huang on 2/22/16.
//  Copyright © 2016 none. All rights reserved.
//

#include "Command.hpp"

MoveCommand::MoveCommand(std::vector<SceneNode *> actors, glm::mat4 transformation):
	Command(actors), trans(transformation)
{
	
}

void MoveCommand::execute()
{
	for (auto actor : _actors) {
		actor->trans = trans * actor->trans;
	}
}

void MoveCommand::undo()
{
	for (auto actor : _actors) {
		actor->trans = glm::inverse(trans) * actor->trans;
	}
}


RotateCommand::RotateCommand(std::vector<SceneNode *> actors, glm::mat4 transformation):
    Command(actors), trans(transformation)
{
	
}

void RotateCommand::execute()
{
	for (auto actor : _actors) {
		actor->trans = actor->trans * trans;
	}
}

void RotateCommand::undo()
{
	for (auto actor : _actors) {
		actor->trans = actor->trans * glm::inverse(trans);
	}
}

JointRotateCommand::JointRotateCommand(std::vector<SceneNode *> actors, float angle):
    Command(actors), m_angle(angle)
{
    for (auto actor : _actors) {
        if (((JointNode *)actor)->activeAxis() == 'x') {
            old_angles.push_back(((JointNode*)actor)->angle_x);
        } else {
            old_angles.push_back(((JointNode*)actor)->angle_y);
        }
    }
}

void JointRotateCommand::execute()
{
    for (int i = 0; i < _actors.size(); i++) {
        JointNode *joint_node = (JointNode *)_actors[i];
        joint_node->rotate(m_angle);
    }
}

void JointRotateCommand::undo()
{
    for (int i = 0; i < _actors.size(); i++) {
        JointNode *joint_node = (JointNode *)_actors[i];
        if (joint_node->activeAxis() == 'x') {
            joint_node->angle_x = old_angles[i];
        } else {
            joint_node->angle_y = old_angles[i];
        }
		joint_node->update_transformation();
    }
}

void HeadRotateCommand::execute()
{
	
}

void HeadRotateCommand::undo()
{
	
}

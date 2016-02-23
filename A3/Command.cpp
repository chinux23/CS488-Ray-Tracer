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

void JointRotateCommand::execute()
{
	
}

void JointRotateCommand::undo()
{
	
}

void HeadRotateCommand::execute()
{
	
}

void HeadRotateCommand::undo()
{
	
}

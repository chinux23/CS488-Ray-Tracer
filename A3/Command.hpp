//
//  Command.hpp
//  CS488-Projects
//
//  Created by Chen Huang on 2/22/16.
//  Copyright © 2016 none. All rights reserved.
//

#ifndef Command_hpp
#define Command_hpp

#include <stdio.h>
#include "SceneNode.hpp"
#include "JointNode.hpp"

#include <vector>

// The idea of this module is originally referenced from:
// http://gameprogrammingpatterns.com/command.html

class Command {
public:
	Command(std::vector<SceneNode *> actors) : _actors(actors) {};
	
	virtual void execute() = 0;
	virtual void undo() = 0;
	virtual ~Command() {};

protected:
	std::vector<SceneNode *> _actors;	// The actor command will be execute on.
};

class MoveCommand : public Command {
public:
	MoveCommand(std::vector<SceneNode *> actors, glm::mat4 transformation);
	
	virtual void execute();
	virtual void undo();
	glm::mat4 trans;
};

class RotateCommand : public Command {
public:
	RotateCommand(std::vector<SceneNode *> actors, glm::mat4 transformation);
	
	virtual void execute();
	virtual void undo();
	glm::mat4 trans;
};

class JointRotateCommand : public Command {
public:
    JointRotateCommand(std::vector<SceneNode *> actors, float angles);
	
	virtual void execute();
	virtual void undo();
    float m_angle;

private:
    std::vector<float> old_angles;

};

class HeadRotateCommand : public Command {
public:
	HeadRotateCommand(std::vector<SceneNode *> actors, float angle, float *headangle);
	virtual void execute();
	virtual void undo();
	float m_angle;
	float head_angle;
	float old_head_angle;
	float *_head_angle;
};



#endif /* Command_hpp */

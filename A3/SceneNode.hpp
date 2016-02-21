#pragma once

#include "Material.hpp"

#include <glm/glm.hpp>

#include <list>
#include <string>
#include <iostream>
#include <deque>
#include <map>
#include <unordered_map>

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include "cs488-framework/MeshConsolidator.hpp"


enum class NodeType {
	SceneNode,
	GeometryNode,
	JointNode
};


class SceneNode {
public:
    SceneNode(const std::string & name);

	SceneNode(const SceneNode & other);

    virtual ~SceneNode();
    
	int totalSceneNodes() const;
    
    const glm::mat4& get_transform() const;
    const glm::mat4& get_inverse() const;
    
    void set_transform(const glm::mat4& m);
    
    void add_child(SceneNode* child);
    
    void remove_child(SceneNode* child);

	//-- Transformations:
    virtual void rotate(char axis, float angle);
    virtual void scale(const glm::vec3& amount);
    virtual void translate(const glm::vec3& amount);


	friend std::ostream & operator << (std::ostream & os, const SceneNode & node);

	bool isSelected;
    
    // Transformations
    glm::mat4 trans;
    glm::mat4 invtrans;
    
    std::list<SceneNode*> children;

	NodeType m_nodeType;
	std::string m_name;
	unsigned int m_nodeId;

#pragma mark - Picking
protected:
	// The number of SceneNode instances.
	static unsigned int							nodeInstanceCount;
	
	// a shared data structure to store all nodes.
    static std::unordered_map<unsigned int, SceneNode *>	Nodes;

	// The id of this node.
	unsigned int								id;
	
	// shared flag to indicate that we are doing picking.
	static bool									do_picking;
	
public:
    // Render the object
    virtual void render(
        const ShaderProgram & shader,
        const glm::mat4 & viewMatrix,
        BatchInfoMap & m_batchInfoMap,
        std::deque<glm::mat4> & stack) const;

    virtual void updateShaderUniforms(
        const ShaderProgram & shader,
        const glm::mat4 & viewMatrix,
        std::deque<glm::mat4> & stack) const;
	
	void enablePicking() const;
	void disablePicking() const;
	
	bool hasID(unsigned int) const;
	SceneNode *nodeFromID(unsigned int i) const;
};

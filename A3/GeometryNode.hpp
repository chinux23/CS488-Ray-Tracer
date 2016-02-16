#pragma once

#include "SceneNode.hpp"
#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include "cs488-framework/MeshConsolidator.hpp"
#include <glm/glm.hpp>

#include <deque>

class GeometryNode : public SceneNode {
public:
	GeometryNode(
		const std::string & meshId,
		const std::string & name
	);

	Material material;

	// Mesh Identifier. This must correspond to an object name of
	// a loaded .obj file.
	std::string meshId;
	
public:
	virtual void render(
		const ShaderProgram & shader,
		const glm::mat4 & viewMatrix,
		BatchInfoMap & m_batchInfoMap,
		std::deque<glm::mat4> & stack) const;

	virtual void updateShaderUniforms(
		const ShaderProgram & shader,
		const glm::mat4 & viewMatrix, 
		std::deque<glm::mat4> & stack) const;
};

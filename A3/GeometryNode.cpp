#include "GeometryNode.hpp"

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
		const std::string & meshId,
		const std::string & name
)
	: SceneNode(name),
	  meshId(meshId)
{
	m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::render()
{
//	updateShaderUniforms(m_shader, *geometryNode, m_view);
//
//	// Get the BatchInfo corresponding to the GeometryNode's unique MeshId.
//	BatchInfo batchInfo = m_batchInfoMap[geometryNode->meshId];
//
//	//-- Now render the mesh:
//	m_shader.enable();
//	glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
//	m_shader.disable();
}

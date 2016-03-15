#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"

class PhongMaterial : public Material {
public:
	PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess);
	PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess, double reflective_index);
	virtual ~PhongMaterial();

	glm::vec3	m_kd;
	glm::vec3	m_ks;
	double		m_refractive_index;

	double m_shininess;
	
	static PhongMaterial *AirMaterial;

public:
	PhongMaterial & operator=(const PhongMaterial & other);
	PhongMaterial & operator=(PhongMaterial other);
	
};

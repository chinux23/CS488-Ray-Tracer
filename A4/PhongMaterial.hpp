#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"

class PhongMaterial : public Material {
public:
	PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess);
	PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess, double reflective_index);
	PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess, double reflective_index, double extinct_coeff);
	virtual ~PhongMaterial();

	glm::vec3	m_kd;
	glm::vec3	m_ks;
	double		m_refractive_index;
	double		m_extinction_coefficient;

	double m_shininess;
	
	static PhongMaterial *Air;
	static PhongMaterial *Glass;
	static PhongMaterial *Water;

public:
	PhongMaterial & operator=(const PhongMaterial & other);
	PhongMaterial & operator=(PhongMaterial other);
	
};

#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"

class PhongMaterial : public Material {
public:
	PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess);
	PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess, double reflective_index);
	PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess, double reflective_index, double extinct_coeff);
	PhongMaterial(const glm::vec3& kd,
				  const glm::vec3& ks,
				  double shininess,
				  double refractive_index,
				  double extinct_coeff,
				  const glm::vec4& glossy_coefficients	// (reflectance, reflectance exponent, transmission, transmission exponent)
				  );
	
	virtual ~PhongMaterial();

	glm::vec3	m_kd;
	glm::vec3	m_ks;
	double		m_refractive_index;
	double		m_extinction_coefficient;
	glm::dvec4  m_glossy_coefficients;

	double m_shininess;
	
	static PhongMaterial *Air;
	static PhongMaterial *Glass;
	static PhongMaterial *Water;

public:
	PhongMaterial & operator=(const PhongMaterial & other);
	PhongMaterial & operator=(PhongMaterial other);
	
};

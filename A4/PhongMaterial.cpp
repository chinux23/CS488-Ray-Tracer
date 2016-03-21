#include "PhongMaterial.hpp"

static PhongMaterial air = PhongMaterial({0, 0, 0}, {0, 0, 0}, 0, 1.0, 0);
static PhongMaterial glass = PhongMaterial({0, 0, 0}, {0, 0, 0}, 0, 1.5075, 9.9359e-9);
static PhongMaterial water = PhongMaterial({0, 0, 0}, {0, 0, 0}, 0, 1.327, 0.00000289);


PhongMaterial * PhongMaterial::Air = &air;
PhongMaterial * PhongMaterial::Glass = &glass;
PhongMaterial * PhongMaterial::Water = &water;

PhongMaterial::PhongMaterial(const glm::vec3& kd,
			  const glm::vec3& ks,
			  double shininess,
			  double refractive_index,
			  double extinct_coeff,
			  const glm::vec4& glossy_coefficients	// (reflectance, reflectance exponent, transmission, transmission exponent)
)
{
	m_glossy_coefficients = glossy_coefficients;
}

PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess )
	: m_kd(kd), m_ks(ks), m_shininess(shininess), m_refractive_index(1.0)

{

}

PhongMaterial::PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess, double reflective_index)
: m_kd(kd), m_ks(ks), m_shininess(shininess), m_refractive_index(reflective_index)

{
	m_extinction_coefficient = 0.0;

}

PhongMaterial::PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess, double reflective_index, double extinct_coeff)
: m_kd(kd), m_ks(ks), m_shininess(shininess), m_refractive_index(reflective_index), m_extinction_coefficient(extinct_coeff)
{
	
}

PhongMaterial::~PhongMaterial()
{}


PhongMaterial & PhongMaterial::operator=(const PhongMaterial & other)
{
	this->m_kd = other.m_kd;
	this->m_ks = other.m_ks;
	this->m_shininess = other.m_shininess;
	this->m_refractive_index = other.m_refractive_index;
	
	return *this;
}

PhongMaterial & PhongMaterial::operator=(PhongMaterial other)
{
	this->m_kd = other.m_kd;
	this->m_ks = other.m_ks;
	this->m_shininess = other.m_shininess;
	this->m_refractive_index = other.m_refractive_index;
	
	return *this;
}

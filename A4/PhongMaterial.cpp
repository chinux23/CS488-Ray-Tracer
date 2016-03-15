#include "PhongMaterial.hpp"

static PhongMaterial air = PhongMaterial({0, 0, 0}, {0, 0, 0}, 0, 1.0);

PhongMaterial * PhongMaterial::AirMaterial = &air;

PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess )
	: m_kd(kd), m_ks(ks), m_shininess(shininess), m_refractive_index(1.0)

{}

PhongMaterial::PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess, double reflective_index)
: m_kd(kd), m_ks(ks), m_shininess(shininess), m_refractive_index(reflective_index)

{}

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

#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess )
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
{}

PhongMaterial::~PhongMaterial()
{}


PhongMaterial & PhongMaterial::operator=(const PhongMaterial & other)
{
	this->m_kd = other.m_kd;
	this->m_ks = other.m_ks;
	this->m_shininess = other.m_shininess;
	
	return *this;
}

PhongMaterial & PhongMaterial::operator=(PhongMaterial other)
{
	this->m_kd = other.m_kd;
	this->m_ks = other.m_ks;
	this->m_shininess = other.m_shininess;
	
	return *this;
}

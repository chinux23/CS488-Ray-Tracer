#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include "Ray.hpp"


void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
);

double calculate_d(double h, double fovy);
glm::dvec4 calculate_p_in_view_coordinates(double x, double y, double w, double h, double d);
glm::dmat4 R3(glm::vec3 up, glm::vec3 eye, glm::vec3 view);
glm::dmat4 T4(glm::vec3 eye);

Ray createRay(double x, double y,
			  const glm::dmat4 & r3,
			  const glm::dmat4 & t4,
			  const double & fovy,
			  const double & d,
			  const glm::vec3 & eye);


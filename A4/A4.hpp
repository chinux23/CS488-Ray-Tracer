#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"

struct HitColor {
	bool hit;
	glm::dvec3 color;
};


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
glm::dvec4 calculate_p_in_world(double x, double y, const glm::dmat4 & trans);

glm::dmat4 T1(double nx, double ny, double d);
glm::dmat4 S2(double w, double h, double nx, double ny);
glm::dmat4 R3(glm::vec3 up, glm::vec3 eye, glm::vec3 view);
glm::dmat4 T4(glm::vec3 eye);

// create a Ray from image pixel.
Ray createRay(glm::dvec4 p1, glm::dvec4 p2);

// Test to see if r hit anything.
Intersection hit(const Ray & r, SceneNode * root);

// Return the color given
HitColor rayColor(const Ray & r, int counter, const std::list<Light*> & lights);

// Cast shadow ray and get the color for the shadow ray.
glm::dvec3 directLight(const std::list<Light*> & lights,
                       const Intersection & primary_intersect);

glm::dvec3 specularHighlight(const std::list<Light *> & lights,
                             const Intersection & primary_intersect);

glm::dvec3 backgroundColor(int x, int y);



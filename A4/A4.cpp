#include <glm/ext.hpp>

#include "A4.hpp"

static double IMAGEWIDTH;
static double IMAGEHEIGHT;


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
) {

  // Fill in raytracing code here...

  std::cout << "Calling A4_Render(\n" <<
		  "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
		  "\t" << "view: " << glm::to_string(view) << std::endl <<
		  "\t" << "up:   " << glm::to_string(up) << std::endl <<
		  "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
		  "\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;

	size_t h = image.height();
	size_t w = image.width();
	IMAGEWIDTH = image.width();
	IMAGEHEIGHT = image.height();
	
	
#pragma mark - test
	double d = calculate_d(h, fovy);
	std::cout << "distance d: " << d << std::endl;
	double x = 128, y = 128;
	glm::dvec4 point_in_view_coord = calculate_p_in_view_coordinates(x, y, image.width(), image.height(), d);
	std::cout << "Point [" << x << ", " << y << "] (in device)" << " become (in view): ";
	std::cout << glm::to_string(point_in_view_coord) << std::endl;
	
	auto r3 = R3(up, eye, view);
	std::cout << "R3: " << std::endl << glm::to_string(r3) << std::endl;
	
	auto t4 = T4(eye);
	std::cout << "T4: " << std::endl << glm::to_string(t4) << std::endl;
	
	glm::dvec4 point_in_world = t4 * r3 * point_in_view_coord;
	std::cout << "Point [" << x << ", " << y << "] (in device)" << " become (in world): ";
	std::cout << glm::to_string(point_in_world) << std::endl;
	

#pragma mark - rendering starts
	
	for (uint y = 0; y < h; ++y) {
		for (uint x = 0; x < w; ++x) {
			
			Ray r = createRay(x, y, r3, t4, fovy, d, eye);
			
			glm::dvec3 color(0, 0, 0);
			
			Intersection primary_intersect = hit(r, root);
			
			if (primary_intersect.hit) {
//				std::cout << "[ " << x << " " << y << " ]" << " hit " << primary_intersect.node->m_name << std::endl;
				
				// Calculate ambient
				color = ambient;		// TO-DO get the K_e and multiply it with ambient.
				
				// Calculate diffusion first
				glm::dvec4 hitPoint = r.origin + primary_intersect.t * r.direction;
				
				std::cout << "primary ray hitPoint: " << glm::to_string(hitPoint) << std::endl;
				
				// Test with all lights
				for (auto light : lights) {
					glm::dvec4 shadow_ray_direction = glm::dvec4(light->position, 1) - hitPoint;
					
//					std::cout << "Shadow Ray direction " << glm::to_string(shadow_ray_direction) << std::endl;
					Ray shadow_ray = Ray(hitPoint + glm::normalize(shadow_ray_direction), shadow_ray_direction);
					
					double shadow_ray_length = glm::length(shadow_ray_direction);
					std::cout << "shadow_ray_length: " << shadow_ray_length << std::endl;
					
					Intersection shadow_intersect = hit(shadow_ray, root);
					
					if (shadow_intersect.hit)
						assert(shadow_intersect.t > 0);
					
					// if no object or hitpoint is longer than the light
					if (shadow_intersect.hit) // &&
//						glm::length(shadow_intersect.t * shadow_ray.direction) < shadow_ray_length)
					{
						glm::dvec4 s_hitPoint = shadow_ray.origin + shadow_intersect.t * shadow_ray.direction;
						std::cout << "Shadow ray hitPoint: " << glm::to_string(s_hitPoint) << std::endl;
						// Blocked
//						std::cout << "Shadow ray hit " << shadow_intersect.node->m_name << std::endl;
//						std::cout << "ShadowRay hit t " << shadow_intersect.t << std::endl;
						
					} else {
						std::cout << "Calculating diffused color " << std::endl;
						
						// Get the diffused color
						glm::vec3 kd = primary_intersect.material->m_kd;
						color += kd * light->colour / (light->falloff[0] +
										 light->falloff[1] * shadow_ray_length +
										 light->falloff[2] * shadow_ray_length * shadow_ray_length);
						
					}
					
				}
				
				// Get reflected color
				
				
				// Set color for the pixel.
				image(x, y, 0) = color.r;
				image(x, y, 1) = color.g;
				image(x, y, 2) = color.b;
				
			} else {
				// Red: increasing from top to bottom
				image(x, y, 0) = (double)y / h;
				// Green: increasing from left to right
				image(x, y, 1) = (double)x / w;
				// Blue: in lower-left and upper-right corners
				image(x, y, 2) = ((y < h/2 && x < w/2)
								  || (y >= h/2 && x >= w/2)) ? 1.0 : 0.0;
			}

		}
	}

}

double calculate_d(double h, double fovy)
{
	// Caclulate distance d from h and field of view fovy.
	double tan_theta_over_2 = glm::tan(glm::radians(fovy/2));
	double d = h / 2 / tan_theta_over_2;
	return d;
}

glm::dvec4 calculate_p_in_view_coordinates(double x, double y, double w, double h, double d) {
	glm::dvec4 p_in_device_coord = glm::dvec4(x, y, 0, 1);
	glm::dvec4 p_in_view_coord = glm::translate(glm::mat4(), glm::vec3(-w/2, -h/2, d)) * p_in_device_coord;
	p_in_device_coord = glm::scale(glm::mat4(), glm::vec3(-1, 1, 1)) * p_in_view_coord;
	
	return p_in_device_coord;
}

glm::dmat4 R3(glm::vec3 up, glm::vec3 eye, glm::vec3 view) {
	glm::vec3 w = view - eye;
	w = glm::normalize(w);
	up = glm::normalize(up);
	
	glm::vec3 u = glm::cross(w, up);
	u = glm::normalize(u);
	
	glm::vec3 v = glm::cross(u, w);
	
	glm::dmat4 r3({
		{u.x, u.y, u.z, 0},
		{v.x, v.y, v.z, 0},
		{w.x, w.y, w.z, 0},
		{0, 0, 0, 1}});

	return r3;
}

glm::dmat4 T4(glm::vec3 eye) {
	glm::dmat4 t4({
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{eye.x, eye.y, eye.z, 1}
	});
	return t4;
}

Ray createRay(double x, double y,
			  const glm::dmat4 & r3,
			  const glm::dmat4 & t4,
			  const double & fovy,
			  const double & d,
			  const glm::vec3 & eye)
{
	glm::dvec4 point_in_view_coord = calculate_p_in_view_coordinates(x, y, IMAGEWIDTH, IMAGEHEIGHT, d);
	glm::dvec4 point_in_world = t4 * r3 * point_in_view_coord;
	
	glm::dvec4 origin({eye.x, eye.y, eye.z, 1});
	
	Ray r = Ray(origin, point_in_world - origin);
	return r;
}

Intersection hit(const Ray & r, SceneNode * root) {
	// Brute force approach to test all SceneNodes.
	// Return the hit which is closest to the ray's origin.
	
	Intersection result(r, 0);
	
	for (auto node : root->children) {
		if (node->m_nodeType == NodeType::GeometryNode) {
			Intersection intersection = node->intersect(r);
			if (intersection.hit && !result.hit) {
				result = intersection;
			} else if (intersection.hit && result.hit && intersection.t < result.t) {
				assert(intersection.t > 0);
				result = intersection;
			}
		}
	}
	
	return result;
}











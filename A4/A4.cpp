#include <glm/ext.hpp>

#include "A4.hpp"

static double IMAGEWIDTH;
static double IMAGEHEIGHT;

#define DISTANCE 800.0

static SceneNode *Scene;


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

	size_t ny = image.height();
	size_t nx = image.width();
	IMAGEWIDTH = image.width();
	IMAGEHEIGHT = image.height();
	
    double w = 2 * DISTANCE * glm::tan(glm::radians(fovy/2));
    double h = 2 * DISTANCE * glm::tan(glm::radians(fovy/2));
	
	Scene = root;
    
    std::cout << "w : " << w << std::endl;
    std::cout << "h : " << h << std::endl;
	
#pragma mark - test
//	double d = calculate_d(h, fovy);
//	std::cout << "distance d: " << d << std::endl;
//	double x = 128, y = 128;
//	glm::dvec4 point_in_view_coord = calculate_p_in_view_coordinates(x, y, image.width(), image.height(), d);
//	std::cout << "Point [" << x << ", " << y << "] (in device)" << " become (in view): ";
//	std::cout << glm::to_string(point_in_view_coord) << std::endl;
    
    auto t1 = T1(nx, ny, DISTANCE);
    auto s2 = S2(w, h, nx, ny);
	auto r3 = R3(up, eye, view);
	auto t4 = T4(eye);
	
//	glm::dvec4 point_in_world = t4 * r3 * point_in_view_coord;
//	std::cout << "Point [" << x << ", " << y << "] (in device)" << " become (in world): ";
//	std::cout << glm::to_string(point_in_world) << std::endl;
    
    glm::dmat4 device_to_world_trans = t4 * r3 * s2 * t1;
    auto x = 256, y = 256;
    auto p_world = calculate_p_in_world(x, y, device_to_world_trans);
    std::cout << "[ " << x << ", " << y << " ]" << " become (in world):" << glm::to_string(p_world) << std::endl;


#pragma mark - rendering starts

	double color_max = 1.0;
	double color_min = 0;
	
	for (uint y = 0; y < ny; ++y) {
		for (uint x = 0; x < nx; ++x) {
            auto p_world = calculate_p_in_world(x, y, device_to_world_trans);
            Ray r = createRay(glm::dvec4(eye, 1), p_world);
			glm::dvec3 color(0, 0, 0);
			
			HitColor hc = rayColor(r, 0, lights);
			
			if (hc.hit) {
				image(x, y, 0) = glm::clamp(hc.color.r, color_min, color_max);
				image(x, y, 1) = glm::clamp(hc.color.g, color_min, color_max);
				image(x, y, 2) = glm::clamp(hc.color.b, color_min, color_max);
			} else {
				glm::dvec3 color = backgroundColor(x, y);
				image(x, y, 0) = color.r;
				image(x, y, 1) = color.g;
				image(x, y, 2) = color.b;
			}
		}
	}
	
	std::cout << "Done" << std::endl;

}

glm::dvec4 calculate_p_in_view_coordinates(double x, double y, double w, double h, double d) {
	glm::dvec4 p_in_device_coord = glm::dvec4(x, y, 0, 1);
	glm::dvec4 p_in_view_coord = glm::translate(glm::mat4(), glm::vec3(-w/2, -h/2, d)) * p_in_device_coord;
	p_in_device_coord = glm::scale(glm::mat4(), glm::vec3(-1, 1, 1)) * p_in_view_coord;
	
	return p_in_device_coord;
}

glm::dmat4 T1(double nx, double ny, double d) {
    return glm::translate(glm::dmat4(), glm::dvec3(-nx/2, -ny/2, d));
}

glm::dmat4 S2(double w, double h, double nx, double ny) {
    return glm::scale(glm::dmat4(), glm::dvec3(-h/ny, w/nx, 1));
}

glm::dmat4 R3(glm::vec3 up, glm::vec3 eye, glm::vec3 view) {
	glm::vec3 w = view;
	w = glm::normalize(w);
	up = glm::normalize(up);
	
	glm::vec3 u = glm::cross(up, w);
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

Ray createRay(glm::dvec4 p1, glm::dvec4 p2)
{
	Ray r = Ray(p1, p2 - p1);
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

glm::dvec4 calculate_p_in_world(double x, double y, const glm::dmat4 & trans)
{
    glm::dvec4 p_in_device_coord = glm::dvec4(x, y, 0, 1);
    auto p_in_world = trans * p_in_device_coord;
    return p_in_world;
}

glm::dvec3 directLight(const glm::dvec4 & point, std::list<Light*> lights)
{
	glm::vec3 color(0, 0, 0);
	
	for (auto light : lights) {
		glm::dvec4 shadow_ray_direction = glm::dvec4(light->position, 1) - point;
		
		Ray shadow_ray = Ray(point + glm::normalize(shadow_ray_direction), shadow_ray_direction);
		
		double shadow_ray_length = glm::length(shadow_ray_direction);
		//					std::cout << "shadow_ray_length: " << shadow_ray_length << std::endl;
		
		Intersection shadow_intersect = hit(shadow_ray, Scene);
		
		if (shadow_intersect.hit)
			assert(shadow_intersect.t > 0);
		
		// if no object or hitpoint is longer than the light
		if (shadow_intersect.hit &&
			glm::length(shadow_intersect.t * shadow_ray.direction) < shadow_ray_length)
		{
			//						glm::dvec4 s_hitPoint = shadow_ray.origin + shadow_intersect.t * shadow_ray.direction;
			//						std::cout << "Shadow ray hitPoint: " << glm::to_string(s_hitPoint) << std::endl;
			// Blocked
			//						std::cout << "Shadow ray hit " << shadow_intersect.node->m_name << std::endl;
			//						std::cout << "ShadowRay hit t " << shadow_intersect.t << std::endl;
			
		} else {
			//						std::cout << "Calculating diffused color " << std::endl;
			
			// Get the diffused color
			color += light->colour / (light->falloff[0] +
									  light->falloff[1] * shadow_ray_length +
								      light->falloff[2] * shadow_ray_length * shadow_ray_length);
			
		}
		
	}
	return color;
}

HitColor rayColor(const Ray & r, int counter, const std::list<Light*> & lights)
{
	glm::dvec3 color(0, 0, 0);
	if (counter > 5) {
		return {false, color};
	}
	
	Intersection primary_intersect = hit(r, Scene);
	
	if (primary_intersect.hit) {
		glm::dvec4 hitPoint = r.origin + primary_intersect.t * r.direction;
		glm::dvec3 kd = primary_intersect.material->m_kd;
		color += kd * directLight(hitPoint, lights);
		
	}

	return {primary_intersect.hit, color};
}

glm::dvec3 backgroundColor(int x, int y)
{	
	glm::dvec3 color(0, 0, 0);
	// Red: increasing from top to bottom
	color.r += 0;
	// Green: increasing from left to right
	color.g += 0;
	// Blue: in lower-left and upper-right corners
	color.b += (double)y / IMAGEHEIGHT;
	
	return color;
}










//
//  UnitTest.m
//  UnitTest
//
//  Created by Chen Huang on 3/16/16.
//  Copyright © 2016 none. All rights reserved.
//

#import <XCTest/XCTest.h>
#include <random>
#import "A4.hpp"
#include <glm/gtc/noise.hpp>
#include <glm.hpp>

#define Epsilon 1.0e-10

@interface UnitTest : XCTestCase

@end

@implementation UnitTest

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

//- (void)testPerformanceExample {
//	glm::dvec4 origin {0, 0, 0, 1};
//	glm::dvec4 ray_point {-0.5, glm::sin(glm::radians((double)60.0)), 0, 1};
//	glm::dvec4 ray_direction = origin - ray_point;
//	__block Ray i = Ray(ray_point, ray_direction);
//	i.material = PhongMaterial::Air;
//	
//	__block Intersection intersec = Intersection(i, 0, false);
//	intersec.fromMaterial = PhongMaterial::Air;
//	intersec.material = PhongMaterial::Glass;
//	intersec.normal = glm::dvec4({0, 1, 0, 0});
//	intersec.t = 1;
//	intersec.hit = true;
//	
//	__block Ray t{glm::dvec4(), glm::dvec4()};
//    // This is an example of a performance test case.
//    [self measureBlock:^{
//        // Put the code you want to measure the time of here.
//		for (int count = 0; count < 1024 * 1024 * 10; count++) {
//			t = refractedRay(i, intersec);
//		}
//    }];
//}

- (void)testRefractedRay {
	glm::dvec4 origin {0, 0, 0, 1};
	glm::dvec4 ray_point {-0.5, glm::sin(glm::radians((double)60.0)), 0, 1};
	glm::dvec4 ray_direction = origin - ray_point;
	Ray i = Ray(ray_point, ray_direction);
	i.material = PhongMaterial::Air;
	
	Intersection intersec = Intersection(i, 0, false);
	intersec.fromMaterial = PhongMaterial::Air;
	intersec.material = PhongMaterial::Glass;
	intersec.normal = glm::dvec4({0, 1, 0, 0});
	intersec.t = 1;
	intersec.hit = true;
	
	Ray t = refractedRay(i, intersec);
	double thetaT = glm::degrees(glm::acos(glm::dot(t.direction, -intersec.normal)));
	XCTAssert(std::abs(thetaT - 19.3704702235) < Epsilon);
}

- (void)testTIR
{
	// see from glass into water with 60 degree of angle, should generate a TIR
	
	glm::dvec4 origin {0, 0, 0, 1};
	glm::dvec4 ray_point {-glm::sin(glm::radians((double)60.0)), glm::cos(glm::radians((double)60.0)), 0, 1};
	glm::dvec4 ray_direction = origin - ray_point;
	Ray i = Ray(ray_point, ray_direction);
	i.material = PhongMaterial::Glass;
	
	Intersection intersec = Intersection(i, 0, false);
	intersec.fromMaterial = PhongMaterial::Glass;
	intersec.material = PhongMaterial::Air;
	intersec.normal = glm::dvec4({0, 1, 0, 0});
	intersec.t = 1;
	intersec.hit = true;
	
	Ray t = refractedRay(i, intersec);
	XCTAssert(!t.isValid());
	
}

- (void)testSimplifiedFresnelModelTIR
{
	glm::dvec4 origin {0, 0, 0, 1};
	glm::dvec4 ray_point {-glm::sin(glm::radians((double)60.0)), glm::cos(glm::radians((double)60.0)), 0, 1};
	glm::dvec4 ray_direction = origin - ray_point;
	Ray i = Ray(ray_point, ray_direction);
	i.material = PhongMaterial::Glass;
	
	Intersection intersec = Intersection(i, 0, false);
	intersec.fromMaterial = PhongMaterial::Glass;
	intersec.material = PhongMaterial::Air;
	intersec.normal = glm::dvec4({0, 1, 0, 0});
	intersec.t = 1;
	intersec.hit = true;
	
	double r = simplifiedFresnelModel(intersec.normal, i.direction, i.material->m_refractive_index, intersec.material->m_refractive_index);
	
	assert(r == 1.0);
}

- (void)testSimplifiedFresnelModel_Normal
{
	glm::dvec4 origin {0, 0, 0, 1};
	glm::dvec4 ray_point {-glm::sin(glm::radians((double)30.0)), glm::cos(glm::radians((double)30.0)), 0, 1};
	glm::dvec4 ray_direction = origin - ray_point;
	Ray i = Ray(ray_point, ray_direction);
	i.material = PhongMaterial::Air;
	
	Intersection intersec = Intersection(i, 0, false);
	intersec.fromMaterial = PhongMaterial::Air;
	intersec.material = PhongMaterial::Glass;
	intersec.normal = glm::dvec4({0, 1, 0, 0});
	intersec.t = 1;
	intersec.hit = true;
	
	double r = simplifiedFresnelModel(intersec.normal, i.direction, i.material->m_refractive_index, intersec.material->m_refractive_index);
	
	assert(std::abs(r - 0.042501157101411904) < Epsilon);
}

- (void)testSimplifiedFresnelModel_Revrse
{
	glm::dvec4 origin {0, 0, 0, 1};
	glm::dvec4 ray_point {-glm::sin(glm::radians((double)30.0)), glm::cos(glm::radians((double)30.0)), 0, 1};
	glm::dvec4 ray_direction = origin - ray_point;
	Ray i = Ray(ray_point, ray_direction);
	i.material = PhongMaterial::Glass;
	
	Intersection intersec = Intersection(i, 0, false);
	intersec.fromMaterial = PhongMaterial::Glass;
	intersec.material = PhongMaterial::Air;
	intersec.normal = glm::dvec4({0, 1, 0, 0});
	intersec.t = 1;
	intersec.hit = true;
	
	double r = simplifiedFresnelModel(intersec.normal, i.direction, i.material->m_refractive_index, intersec.material->m_refractive_index);
	
	assert(std::abs(r - 0.056818273527026959) < Epsilon);
}

- (void)testSphericalLight
{
	glm::dvec3 pos {10, 30, 40};
	
	auto light = SphericalLight(pos, glm::vec3(1, 1, 1), 100);
	
	for (int i = 0; i < 10; i++) {
		glm::dvec3 point = light.randomPoint();
		
		XCTAssert(glm::length(point - pos) - 100 < Epsilon);
		std::cout << glm::to_string(point) << std::endl;
		std::cout << "length: " << glm::length(point - pos) << std::endl;
	}
	
}

- (void)testRandom
{
	std::random_device generator;
	std::uniform_real_distribution<double> distribution(0.0,1.0);
	
	for (int i = 0; i < 100; i++) {
		double num = distribution(generator);
//		std::cout << num << std::endl;
		XCTAssert(num < 1.0 && num > 0);
	}
	
}

- (void)testperturbe
{
	std::vector<glm::dvec3> Rs = {{0, 1, 0}, {0, 0, 1}, {0, 0, 1}, {1, 1, 1}, {1, 1, 0}, {0, 1, 1}, {2, 3, 4}};
	// generate random value
	std::random_device generator;
	std::uniform_real_distribution<double> distribution(0.0,1.0);
	
	
	for (auto R : Rs) {
		for (int i = 0; i < 10; i++) {
			R = glm::normalize(R);
			
			// Give a Vector R, pertube it with N
			double N = 10;
			
			glm::dvec3 e0 {0, 1, 0};
			glm::dvec3 e1 {0, 0, 1};
			
			glm::dvec3 W = R;
			
			glm::dvec3 U = glm::cross(W, e0);
			if (glm::length(U) < 0.1) {
				U = glm::cross(W, e1);
			}
			glm::dvec3 V = glm::cross(W, U);
			
			
			
			double phi = distribution(generator) * 2 * glm::pi<double>();
			double cosine_theta = glm::pow(distribution(generator), (double)1.0/(N+1));
			double sine_theta = glm::sqrt(1 - cosine_theta * cosine_theta);
			double cosine_phi = glm::cos(phi);
			double sine_phi = glm::sin(phi);
			
			glm::dvec3 A = W * cosine_theta + U * cosine_phi * sine_theta + V * sine_phi * sine_theta;
			
			std::cout << glm::degrees(glm::acos(glm::dot(R, A))) << std::endl;
			std::cout << "Perturbed vector: " << glm::to_string(A) << std::endl;
		}
	}
}

- (void) testPerlinNoise
{
    for (int i = 0; i < 10; i++) {
        float noise = glm::perlin(glm::dvec2(-0.5, -0.5));
        std::cout << noise << std::endl;
    }
}


@end

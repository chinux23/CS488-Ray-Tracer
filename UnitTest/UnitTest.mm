//
//  UnitTest.m
//  UnitTest
//
//  Created by Chen Huang on 3/16/16.
//  Copyright © 2016 none. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "A4.hpp"

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


@end

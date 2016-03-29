-- test for hierarchical ray-tracers.
-- Thomas Pflaum 1996

gold = gr.material(	 {0.9, 0.8, 0.4}, 	{0.8, 0.8, 0.4}, 	25, 0.0)
gold2 = gr.material( {1.0, 1.0, 0.3},   {1.0, 1.0, 1.0},	25, 0.0)
grass = gr.material( {0.1, 0.7, 0.1}, 	{0.0, 0.0, 0.0}, 	0,  0.0)
blue = gr.material(  {0.2, 0.2, 0.9}, 	{0.1, 0.1, 0.5}, 	25, 1e-5)
glass = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, 1.5)			--enable optics
water = gr.material( {0.05, 0.05, 0.05}, {0.6, 0.6, 0.6}, 25, 1.3333)
red_wall = gr.material( {0.6, 0.267, 0.3686}, {0.3, 0.05, 0.05}, 25, 0.0)
green_wall = gr.material( {0.2686, 0.2627, 0.6}, {0.1, 0.05, 0.5}, 25, 0.0)
grey_wall = gr.material( {0.6649, 0.623, 0.7002}, {0.3, 0.3, 0.3}, 25, 0.0)
reflective_wall = gr.material( {0, 0, 0}, {0.9, 0.9, 0.9}, 25, 1e-10)
white_ceiling = gr.material({0.9, 0.9, 0.9}, {0.5, 0.5, 0.5}, 25, 0.0)
mat4  = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, 0.0)			--disable optics
mat1  = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0.0)  		--disable optics
mat3  = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 0.0)			--disable optics
reflective = gr.material({0.0, 0.0, 0.0}, {0.7, 0.7, 0.7}, 25, 0.0)		--disable optics and disfusion
glossy_reflective = {1.0, 10, 0.0, 100}					--glossy reflection
glossy_refractive = {0.0, 1000, 1.0, 100}					--glossy refraction
mirror1 = gr.material( {0.2, 0.2, 0.2},   {1.0, 1.0, 1.0},	25, 0.0)
mirror2 = gr.material( {0.2, 0.2, 0.2},   {1.0, 1.0, 1.0},	25, 0.0)
glass2 = gr.material( {0.05, 0.05, 0.05}, {0.7, 0.7, 0.7},  25, 1.5)			--enable optics

marble = gr.material( {0.7, 0.7, 0.7}, {1.0, 1.0, 1.0}, 25, 0.0)
hardwoodfloor = gr.material({0.7, 0.7, 0.7}, {0.1, 0.1, 0.1}, 25, 0.0)



scene = gr.node('scene')

-- reflective sphere
-- reflectiveSphere2 = gr.sphere('reflective2')
-- scene:add_child(reflectiveSphere2)
-- reflectiveSphere2:set_material(blue)
-- reflectiveSphere2:set_glossy({1.0, 10, 0.0, 100})
-- reflectiveSphere2:scale(2, 2, 2)
-- reflectiveSphere2:translate(-6, -6, -5)

-- reflectiveSphere3 = gr.sphere('reflective3')
-- scene:add_child(reflectiveSphere3)
-- reflectiveSphere3:set_material(mirror2)
-- reflectiveSphere3:set_glossy({1.0, 10000, 0.0, 100})
-- reflectiveSphere3:scale(2, 2, 2)
-- reflectiveSphere3:translate(6, -8, -5)

transparentSphere = gr.sphere('transparentSphere')
scene:add_child(transparentSphere)
transparentSphere:set_material(glass2)
-- transparentSphere:set_glossy({0.0, 10, 1.0, 1000})
transparentSphere:scale(2, 2, 2)
transparentSphere:translate(0, -3, 4)

transparentCylinder = gr.sphere('transparentCylinder')
transparentSphere:add_child(transparentCylinder)
transparentCylinder:set_material(water)
-- transparentSphere:set_glossy({0.0, 10, 1.0, 1000})
transparentCylinder:scale(1/4, 1/4, 1/4)
-- transparentCylinder:translate(0.9, -2.5, 0)

transparentCylinder2 = gr.cylinder('transparentCylinder2')
scene:add_child(transparentCylinder2)
transparentCylinder2:set_material(glass)
transparentCylinder2:set_glossy({0.0, 10, 1.0, 100})
transparentCylinder2:scale(1, 2, 1)
transparentCylinder2:translate(0, -7, 4)

-- transparentCone = gr.cone('transparentCone')
-- scene:add_child(transparentCone)
-- transparentCone:set_material(water)
-- -- transparentSphere:set_glossy({0.0, 10, 1.0, 1000})
-- transparentCone:scale(1, 2, 1)
-- transparentCone:translate(1, -4, 4)

-- b1 = gr.cube('b1')
-- scene:add_child(b1)
-- b1:set_material(mat4)
-- b1:scale(3, 3, 3)
-- b1:translate(-5, -10, 1)

-- s1 = gr.sphere('s1')
-- scene:add_child(s1)
-- s1:set_material(mat3)
-- -- s1:scale(2, 2, 2)
-- s1:translate(-1, -6, -8)

floor = gr.plane('floor')
scene:add_child(floor)
floor:set_material(marble)
floor:set_texture(1, "Assets/hardwoodfloor.png")
floor:set_glossy({0.2, 10000, 0.0, 100})
floor:scale(10, 10, 30)
floor:translate(0, -10, 0)

backwall = gr.plane('backwall')
scene:add_child(backwall)
backwall:set_material(grey_wall)
backwall:set_texture(1, "Assets/brickwall.png")
backwall:rotate('X', 90)
backwall:scale(10, 10, 1)
backwall:translate(0, 0, -10)

leftwall = gr.plane('leftwall')
scene:add_child(leftwall)
leftwall:set_material(reflective_wall)
-- leftwall:set_texture(1, "Assets/brickwall.png")
-- leftwall:set_glossy({1.0, 10000, 0.0, 100})
leftwall:rotate('Z', -90)
leftwall:rotate('X', 90)
leftwall:scale(10, 10, 30)
leftwall:translate(-10, 0, 0)

rightwall = gr.plane('rightwall')
scene:add_child(rightwall)
rightwall:set_material(reflective_wall)
-- rightwall:set_texture(1, "Assets/brickwall.png")
-- rightwall:set_glossy({1.0, 10000, 0.0, 100})
rightwall:rotate('Z', 90)
rightwall:rotate('X', 90)
rightwall:scale(10, 10, 30)
rightwall:translate(10, 0, 0)

topwall = gr.plane('topwall')
scene:add_child(topwall)
topwall:set_material(white_ceiling)
-- topwall:set_texture(1, "Assets/Ceiling.png")
topwall:rotate('X', 180)
topwall:scale(10, 10, 30)
topwall:translate(0, 9.9, 0)

-- The lights
l1 = gr.light({0, 50, 50}, {0.8, 0.8, 0.8}, {1, 0, 0})
white_light = gr.light({2.0, 2.0, 3}, {0.8, 0.8, 0.8}, {1, 0, 0})
green_white_light = gr.light({0.0, 0.0, 400.0}, {0.5, 0.5, 0.5}, {1, 0, 0})
l2 = gr.light({0, 5, -20}, {0.4, 0.4, 0.8}, {1, 0, 0})
white_spherical_light = gr.sphericallight({4, 8, 4}, {1, 1, 1}, {1, 0, 0}, 0.5)

gr.render(scene, 'room.png', 1024, 1024,
	  {0, 3, -4}, {1, 0, 0}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {white_spherical_light})

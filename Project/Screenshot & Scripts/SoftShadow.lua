-- test for hierarchical ray-tracers.
-- Thomas Pflaum 1996

gold = gr.material(	 {0.9, 0.8, 0.4}, 	{0.8, 0.8, 0.4}, 	25, 0.0)
grass = gr.material( {0.1, 0.7, 0.1}, 	{0.0, 0.0, 0.0}, 	0,  0.0)
blue = gr.material(  {0.7, 0.6, 1}, 	{0.5, 0.4, 0.8}, 	25, 0.0)

glass = gr.material({0.05, 0.05, 0.05}, {0.7, 0.7, 0.7}, 25, 1.5)			--enable optics
red_wall = gr.material( {0.94, 0.2627, 0.3686}, {0.3, 0.05, 0.05}, 25, 0.0)
green_wall = gr.material( {0.2627, 0.94, 0.3686}, {0.05, 0.3, 0.05}, 25, 0.0)
grey_wall = gr.material( {0.4649, 0.4823, 0.5002}, {0.8649, 0.88, 0.90}, 25, 0.0)

mat4  = gr.material({0.7, 0.6, 1.0}, {0.5, 0.4, 0.8}, 25, 0.0)			--disable optics
mat1  = gr.material({0.7, 1.0, 0.7}, {0.5, 0.7, 0.5}, 25, 0.0)  		--disable optics
mat3  = gr.material({1.0, 0.6, 0.1}, {0.5, 0.7, 0.5}, 25, 0.0)			--disable optics

scene = gr.node('scene')

-- -- reflective sphere
reflectiveSphere = gr.sphere("reflective")
scene:add_child(reflectiveSphere)
reflectiveSphere:set_material(glass)
reflectiveSphere:scale(2, 2, 2)
reflectiveSphere:translate(1, -4, 4)

b1 = gr.cube('b1')
scene:add_child(b1)
b1:set_material(mat4)
b1:scale(3, 3, 3)
b1:translate(-5, -9, 1)

s1 = gr.sphere('s1')
scene:add_child(s1)
s1:set_material(mat3)
-- s1:scale(2, 2, 2)
s1:translate(-1, -6, -8)

floor = gr.plane('floor')
scene:add_child(floor)
floor:set_material(grey_wall)
floor:scale(10, 10, 30)
floor:translate(0, -10, 0)

backwall = gr.plane('backwall')
scene:add_child(backwall)
backwall:set_material(grey_wall)
backwall:rotate('X', 90)
backwall:scale(10, 10, 100)
backwall:translate(0, 0, -10)

leftwall = gr.plane('leftwall')
scene:add_child(leftwall)
leftwall:set_material(red_wall)
leftwall:rotate('Z', -90)
leftwall:scale(10, 10, 100)
leftwall:translate(-10, 0, -10)

rightwall = gr.plane('rightwall')
scene:add_child(rightwall)
rightwall:set_material(green_wall)
rightwall:rotate('Z', 90)
rightwall:scale(10, 10, 100)
rightwall:translate(10, 0, -10)

topwall = gr.plane('topwall')
scene:add_child(topwall)
topwall:set_material(grey_wall)
topwall:rotate('X', 180)
topwall:scale(10, 10, 30)
topwall:translate(0, 9.9, 0)

-- The lights
l1 = gr.light({0, 50, 50}, {0.8, 0.8, 0.8}, {1, 0, 0})
white_light = gr.light({2.0, 2.0, 3}, {0.8, 0.8, 0.8}, {1, 0, 0})
green_white_light = gr.light({0.0, 0.0, 400.0}, {0.5, 0.5, 0.5}, {1, 0, 0})
l2 = gr.light({0, 5, -20}, {0.4, 0.4, 0.8}, {1, 0, 0})
white_spherical_light = gr.sphericallight({2, 2, 2}, {0.8, 0.8, 0.8}, {1, 0, 0}, 0.5)

gr.render(scene, 'room.png', 1024, 1024,
	  {0, 0, 30}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {white_spherical_light})

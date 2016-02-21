-- a3mark.lua
-- A very simple scene creating a trivial hierarchical puppet.
-- We'll use this scene as part of testing your assignment.
-- See a3mark.png for an example of what this file produces.

rootnode = gr.node('root')

red = gr.material({1.0, 0.0, 0.0}, {0.1, 0.1, 0.1}, 10)
blue = gr.material({0.0, 0.0, 1.0}, {0.1, 0.1, 0.1}, 10)
green = gr.material({0.0, 1.0, 0.0}, {0.1, 0.1, 0.1}, 10)
white = gr.material({1.0, 1.0, 1.0}, {0.1, 0.1, 0.1}, 10)

s0 = gr.mesh('cube','s0')
rootnode:add_child(s0)
s0:set_material(white)

j1 = gr.joint('leftShoulderJoint', {0, 0, 0}, {-180, 30, 30})
s0:add_child(j1)

j0 = gr.joint('rightShoulderJoint', {0, 0, 0}, {-180, -30, 30})
s0:add_child(j0)

right_arm = gr.mesh('cube','right_arm')
j0:add_child(right_arm)
right_arm:translate(1.0, 2.0, 0.0)
right_arm:scale(1.0, 0.1, 0.1)
right_arm:set_material(red)

left_arm = gr.mesh('cube','left_arm')
j1:add_child(left_arm)
left_arm:translate(-1.0, 2.0, 0.0)
left_arm:scale(1.0, 0.1, 0.1)
left_arm:set_material(green)

-- s2 = gr.mesh('sphere','s2')
-- j1:add_child(s2)
-- s2:scale(1.0, 1.0, 1.0)
-- -- s2:rotate('z', 90.0)
-- s2:translate(0.0, -1.0, 0.0)
-- s2:set_material(blue)

-- s3 = gr.mesh('sphere','s3')
-- s2:add_child(s3)
-- s3:scale(1.0, 0.1, 2.0)
-- s3:translate(0.0, -2.0, 2.0)
-- s3:set_material(green)

rootnode:translate(-2.75, 0.0, -10.0)

return rootnode
